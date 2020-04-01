/*********************************************************************************
 * Author: Richard Mwaba
 * Title: Producer/Consumer Uroblem Using Semaphores main.cpp
 * Date: Monday, March 20, 2020
 * Description: An implementation of the bounded buffer producer consumer
 * problem.  In this implementation there may be any number of producers and
 * any number of consumers.  The program is run with four arguments: (1) the number 
 * of items to be placed in the buffer,  (2) the number of producers to produce 
 * the items (3) number of consumers to consume the items and (4) the size of the 
 * buffer. 
 *********************************************************************************/

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

void * produce( void * arg);
void * consume( void * arg);
int insert(char item, int threadId);
int remove(int threadId);

using namespace std;

sem_t emptyBuffer, fullBuffer, mutex;
char * buffer;
int bufferSize = 0, produced = 0, consumed = 0, nextIn = 0, nextOut = 0;
int numberOfConsumerThreads = 0, numberOfProducerThreads = 0, numberOfItems = 0;
struct thread_data_t {
  int threadNum;
};


int insert(char item, int threadId) {
    if(produced < numberOfItems) {
        buffer[nextIn] = item;
        cout << "p:<" << threadId << ">,item: " << item << ", at " << nextIn << endl;
        nextIn = (nextIn + 1) % bufferSize;
        produced++;
        return 0;
    }else{
        return -1;  
    }
}

int remove(int threadId) {
    if(consumed < numberOfItems){
        char item = buffer[nextOut];
        cout << "c:<" << threadId << ">,item: " << item << ", at " << nextOut << endl;
        nextOut = (nextOut + 1) % bufferSize;
        consumed++;
        return 0;
    }else{
        return -1;
    }
}

void * produce( void * arg)
{
    int *producerThreadId = (int *)arg;
    // cout << "The Producer Thread Id Received is: " << *producerThreadId << endl;
    while(1)
    {
        sem_wait(&emptyBuffer);
        sem_wait(&mutex);
        // insert X into the first available slot in the buffer
        char item = 'X';
        if(insert(item, *producerThreadId) != 0){
           sem_post(&mutex);
           sem_post(&fullBuffer);
           return NULL;
        }
        sem_post(&mutex);
        sem_post(&fullBuffer);
    }
}

void * consume( void * arg)
{
    int *consumerThreadId = (int *)arg;
    // cout << "The Consumer Thread Id Received is: " << *consumerThreadId << endl;
    while(1)
    {
        sem_wait(&fullBuffer);
        sem_wait(&mutex);
        // remove X from the last used slot in the buffer
        if(remove(*consumerThreadId) != 0){
            sem_post(&mutex);
            sem_post(&emptyBuffer);
            return NULL;
        }
        sem_post(&mutex);
        sem_post(&emptyBuffer);
    }
}

int main(int argc, char *argv[]) {
    pthread_t *producerThreads, *consumerThreads;
    thread_data_t *p_thread_data;
    thread_data_t *c_thread_data;
    string switchB = "-b", switchC = "-c", switchP = "-p", switchI = "-i";

    if(argc > 8) {
        for(int i = 1; i < argc; i++){
            string opt = argv[i];
            if(opt.substr(0, 1).compare("-") == 0) {
                if((switchB.compare(argv[i])) == 0) {
                    bufferSize = atoi(argv[i + 1]);
                    buffer = new char[bufferSize];
                } else if((switchC.compare(argv[i])) == 0){
                    numberOfConsumerThreads = atoi(argv[i + 1]);
                    consumerThreads = new pthread_t[numberOfConsumerThreads];
                    c_thread_data = new thread_data_t[numberOfConsumerThreads];
                } else if ((switchP.compare(argv[i])) == 0) {
                    numberOfProducerThreads = atoi(argv[i + 1]);
                    producerThreads = new pthread_t[numberOfProducerThreads];
                    p_thread_data = new thread_data_t[numberOfProducerThreads];
                } else if ((switchI.compare(argv[i])) == 0) {
                    numberOfItems = atoi(argv[i + 1]);
                } else {
                    int pos = opt.find("-"); 
                
                    cout << "invalid option -- '" << opt.substr(pos + 1) <<"'" << endl;
                    cout << "Usage: ./part1 -b 1 -p 5 -c 5 -i 10" << endl;
                    return -1;
                }
            }
        }

        sem_init(&emptyBuffer, 0, bufferSize);
        sem_init(&fullBuffer, 0, 0);
        sem_init(&mutex, 0, 1);

        for(int i=0; i < numberOfProducerThreads; i++)
        {
            p_thread_data[i].threadNum = i + 1;
            pthread_create(&producerThreads[i], NULL, produce, &p_thread_data[i].threadNum);
        }

        for(int j=0; j < numberOfConsumerThreads; j++)
        {
            c_thread_data[j].threadNum = j + 1;
            pthread_create( &consumerThreads[j], NULL, consume, &c_thread_data[j].threadNum);
        }

        for(int i=0; i < numberOfProducerThreads; i++)
        {
            pthread_join(producerThreads[i], NULL);
        }

        for(int j=0; j < numberOfConsumerThreads; j++)
        {
            pthread_join(consumerThreads[j], NULL);
            
        }
        sem_destroy(&mutex);
        sem_destroy(&emptyBuffer);
        sem_destroy(&fullBuffer);

        return 0;
    } else {
        cout << "Requires 8 arguments" << endl;
        cout << "Usage: ./part1 -b 1 -p 5 -c 5 -i 10" << endl;
        return -1;
    }
    // Delete all dynamically allocated memory
    delete[] buffer;
    delete[] producerThreads;
    delete[] consumerThreads;

    return 0;
}
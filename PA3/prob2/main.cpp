#include <iostream>
#include <string>
#include <semaphore.h>

#include "main.hpp"
#include "producer_consumer_monitor.hpp"

using namespace std;

const int MAX = 52;



int bufferSize = 0;
int numberOfConsumerThreads = 0, numberOfProducerThreads = 0, numberOfItems = 0;
struct thread_data_t {
  int threadNum;
};

int main(int argc, char *argv[]) {
    pthread_t *producerThreads, *consumerThreads;
    thread_data_t *p_thread_data;
    thread_data_t *c_thread_data;
    string switchB = "-b", switchC = "-c", switchP = "-p", switchI = "-i";
    srand(time(NULL));

    if(argc > 8) {
        for(int i = 1; i < argc; i++){
            string opt = argv[i];
            if(opt.substr(0, 1).compare("-") == 0) {
                if((switchB.compare(argv[i])) == 0) {
                    bufferSize = atoi(argv[i + 1]);
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
    }else{
        cout << "Requires 8 arguments" << endl;
        cout << "Usage: ./part1 -b 1 -p 5 -c 5 -i 10" << endl;
        return -1; 
    }

    // Initialize monitor
    initialize_monitor(bufferSize, numberOfItems);

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

    // Destroy monitor
    destroy_monitor();
    delete[] producerThreads;
    delete[] consumerThreads;

    return 0;

}

void * produce(void * arg){
    char alpha;
    int *producerThreadId = (int *)arg;
    while(1) {
        alpha = generate_random_alphabet(1);
        mon_insert(alpha, *producerThreadId);
    }

    return EXIT_SUCCESS;
}

void * consume(void * arg){
    // char result;
    int *consumerThreadId = (int *)arg;
    while(1) {
        mon_remove(*consumerThreadId);
    }

    return EXIT_SUCCESS;
}

char generate_random_alphabet(int n) { 
	char alphabet[MAX] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 
                        'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 
                        'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 
                        'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 
						'V', 'W', 'X', 'Y', 'Z' }; 

	char res; 
	for (int i = 0; i < n; i++) {
		res = res + alphabet[rand() % MAX]; 
    }
	
	return res; 
} 


// ===== Monitor.cpp ===== //
struct Monitor{
    sem_t mutex, nextThread;
    int totalNumberOfItems = 0, nextThreadCounter = 0, bufferMaxSize = 0;
};

struct Condition{
    sem_t semaphore;
    int conditionCounter = 0;
};

char * buffer;
int nextIn = 0, nextOut = 0;
int currentItemsCount = 0, totalItemsCount;

Condition fullBuffer, emptyBuffer;
Monitor monitor;

// Wait for condition variable
void wait(Condition *cv);

// Signal condition variable
void signal(Condition *cv);

int variableCount(Condition *conditionVariable){
    return conditionVariable->conditionCounter;
}

void initialize_monitor(int bufferSize, int numOfItems) {
    
    buffer = new char[bufferSize];
    monitor.bufferMaxSize = bufferSize;
    monitor.totalNumberOfItems = numOfItems;
    sem_init(&fullBuffer.semaphore, 0, 0);
    sem_init(&emptyBuffer.semaphore, 0, 0);
    sem_init(&monitor.nextThread, 0, 0);
    sem_init(&monitor.mutex, 0, 1);
}

void wait(Condition *conditionVariable){
    conditionVariable->conditionCounter++;
    if(monitor.nextThreadCounter > 0){
        sem_post(&monitor.nextThread);
    }else{
        sem_post(&monitor.mutex);
    }
    sem_wait(&conditionVariable->semaphore);
    conditionVariable->conditionCounter--;
}

void signal(Condition *conditionVariable){
    if(variableCount(conditionVariable) > 0) {
        monitor.nextThreadCounter++;
        sem_post(&conditionVariable->semaphore);
        sem_wait(&monitor.nextThread);
        monitor.nextThreadCounter--;
    }
}

void mon_insert(char alpha, int id){
    sem_wait(&monitor.mutex);

    if(totalItemsCount >= monitor.totalNumberOfItems){
        sem_post(&monitor.mutex);
        pthread_exit(0);
    }

    while(currentItemsCount == monitor.bufferMaxSize){
        wait(&fullBuffer);
    }

    if(totalItemsCount < monitor.totalNumberOfItems && currentItemsCount < monitor.bufferMaxSize){
        buffer[nextIn] = alpha;
        cout << "p:<" << id << ">,item: " << alpha << ", at " << nextIn << endl;
        nextIn = (nextIn + 1) % monitor.bufferMaxSize;
        currentItemsCount++;
        totalItemsCount++;
    }

    signal(&emptyBuffer);

    if(monitor.nextThreadCounter > 0){
        sem_post(&monitor.nextThread);
    }else{
        sem_post(&monitor.mutex);
    }
    
}

void mon_remove(int id){
    char result;

    sem_wait(&monitor.mutex);

    if(totalItemsCount >= monitor.totalNumberOfItems && currentItemsCount == 0){
        exit(EXIT_SUCCESS);
    }

    while(currentItemsCount == 0){
        wait(&emptyBuffer);
    }

    if(totalItemsCount <= monitor.totalNumberOfItems && currentItemsCount > 0){
        result = buffer[nextOut];
        cout << "c:<" << id << ">,item: " << result << ", at " << nextOut << endl;
        nextOut = (nextOut + 1) % monitor.bufferMaxSize;
        currentItemsCount--;
    }

    signal(&fullBuffer);

    if(monitor.nextThreadCounter > 0){
        sem_post(&monitor.nextThread);
    }else{
        sem_post(&monitor.mutex);
    }
}

void destroy_monitor() {
    sem_destroy(&fullBuffer.semaphore);
    sem_destroy(&emptyBuffer.semaphore);
    sem_destroy(&monitor.mutex);
    sem_destroy(&monitor.nextThread);
    delete[] buffer;
}

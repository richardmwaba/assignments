#ifndef PRODUCER_CONSUMER_MONOTOR_HPP
#define PRODUCER_CONSUMER_MONOTOR_HPP

#include <iostream>
#include <semaphore.h>


// Initiaize monitor
void initialize_monitor(int bufferSize, int totalNumberOfItems);

// Destroy monitor
void destroy_monitor();

// // Wait for condition variable
// void wait(Condition cv);

// // Signal condition variable
// void signal(Condition cv);

// Insert item into buffer
void mon_insert(char alpha, int id);

// Remove from buffer
void mon_remove(int id);
#endif
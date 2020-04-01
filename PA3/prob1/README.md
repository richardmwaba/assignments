# Programming Assignment 3 - PA3

This programming assignment is to get us familiar with the synchronization 
constructs we discussed in class – mutexes, semaphores, and monitors. There 
are two problems in this assignment: solving the producer/consumer problem 
with 1) semaphores, and 2) a custom monitor in C++.


## Getting Started

These instructions will get you a copy of the project up and running on your 
target machine for testing purposes.

## Prerequisites

g++ compiler
C++ libraries
C++ pthread libraries

## Compilation and Cleaning

You will need to do the following to compile the program.

The program comes with a make file that is used to compile the program.

1. Run make in your terminal in root directory of the project as below

```sh
[username@localhost dir]$ make
```

This generates "part1" file and other files that you will use to run the 
program.

2. To clean the program (delete all generated files), run make clean as below

```sh
[username@localhost dir]$ make clean
```

This deletes all generated files from running make. The target files are 
described in the Makefile. You are free to make changes and break the program.
:).

## Running the Program

This program can be run against arguments as shown below. Failure to supply the 
the correct arguments will result in unexpected output and behavior (Disclaimer :))


```sh
[username@localhost dir]$./part1 -b 1 -p 5 -c 5 -i 10
```
The above command will be interpreted as;
    -b 1 = create a buffer of size 1
    -p 5 = create 5 producer threads to insert items into the buffer
    -c 5 = create 5 consumer threads to consume items from the buffer
    -i 10 = produce and insert only 10 items into the buffer

The items to be inserted are specified inside the program.


## Challenges Faced

The biggest challenge was getting the producers to stop producing and the consumers
to stop consuming when the required number of items has been reached. After I got
that right, everything else was a breeze.
 

## Lessons learnt

In this assignment, I learnt how to properly synchronize threads using semaphores. I
made use of the properties of semaphores to solve the producer-consumer problem. I
I also learnt that it's easy to get the synchronization wrong even with semaphores.

## Acknowledgements

Dr. Justin Bradley for his guidelines and tips during class and in provided document.

The TAs also really helped whenever I got stuck as well as my classmates on Piazza

# Programming Assignment 2 - PA2

This project consists of a C++ program to serve as a shell interface that 
accepts user commands and executes each command in a separate process. 
A shell interface gives the user a prompt, after which the next command is 
entered. The example below illustrates the prompt osh> and the user’s next 
command: cat prog.c. (This command displays the file prog.c on the terminal 
using the Unix cat command.)

```sh
osh> cat prog.c
```

## Getting Started

These instructions will get you a copy of the project up and running on your 
target machine for testing purposes.

## Prerequisites

- g++ compiler
- C++ libraries

## Compilation and Cleaning

You will need to do the following to compile the program.

The program comes with a make file that is used to compile the program.

1. Run make in your terminal in root directory of the project as below

```sh
[username@localhost dir]$ make
```

This generates an "osh" file and other files that you will use to run the 
program.

2. To clean the program (delete all generated files), run make clean as below

```sh
[username@localhost dir]$ make clean
```

This deletes all generated files from running make. The target files are 
described in the Makefile. You are free to make changes and break the program.
:).

## Running the Program

This program can be run in 2 modes. Choose what fits you.

1. The program can be run as a prompt that will be expecting input from the 
terminal. To execute given input, press enter on your keyboard.

```sh
[username@localhost dir]$./osh
```
The prompt will be as shown below

```sh
osh>
```
2. The program can be run against arguments, for which however, a switch -t
is required. Failure to supply the switch -t will result in unexpected output 
and behavior (Disclaimer :)).

```sh
[username@localhost dir]$./osh -t < myCommands.txt
```
The above command will show the ouput of running commands, from myCommands.txt, in 
the terminal. These can however be redirected to a file as shown below

```sh
[username@localhost dir]$./osh -t < path/myCommands.txt >& path/targetFile
```

Note that only files containing commands can be redirected to the program. Passing 
commands directly will not work.


## Challenges Faced

The biggest challenge was managing file descriptors of the pipes. Closing the wrong 
descriptor just messes up everything. It took me a while and a lot consultation to 
finally get it right.
 

## Lessons learnt

In this assignment, I learnt how to efficeiently have processes communicate with each
other using unnamed pipes. This solidified my understaning of processes, parent-child 
relationship and file descriptors.`

## Acknowledgements

Dr. Justin Bradley for his guidelines and tips during class and in provided document.

Yutaka Tsutano for his parser which is the backbone of this program. It really helped 
me. It can be found on his <a href="https://github.com/ytsutano/osh-parser">github repo</a>

The TAs also really helped whenever I got stuck as well as my classmates on Piazza

Ashraful Islam really helped me understand how pipes function. I kept confusing the
points at which the file descriptors were to be closed but then he gave me a great 
guideline

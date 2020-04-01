# Programming Assignment 1 - PA2

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

g++ compiler
C++ libraries

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
[username@localhost dir]$./osh -t < path/myCommands.txt > path/targetFile
```

Note that only files containing commands can be redirected to the program. Passing 
commands directly will not work.


## Challenges Faced

The biggest challenge I faced was getting started. Every time I read through the
guidelines, I felt like I was being asked to build a spaceship. Things were not 
making sense until I decided to just start.
Implementing logical operators was also a bit challenging as the guidelines focused
more on the default given parser which had a totally different implentation from the
alternative by Yutaka Tsutano. Therefore, a different approach had to be used.
 

## Lessons learnt

Coming into this class, I had only a vague idea of how exactly processes work in 
the background and the details therein. I had no knowledge of the fact that we can
explicitly cause them to be created through our code as I have mostly worked with
high-level programming languages where everything is done for us. I can confidently 
say, I have learnt how to create processes, execute them and extract useful 
information. I have also learnt how to use system functions to execute processes, 
open files, allocate memory and manage it and also understand return values of those 
functions.

## Acknowledgements

Dr. Justin Bradley for his guidelines and tips during class and in provided document.

Yutaka Tsutano for his parser which is the backbone of this program. It really helped 
me. It can be found on his <a href="https://github.com/ytsutano/osh-parser">github repo</a>

The TAs also really helped whenever I got stuck as well as my classmates on Piazza

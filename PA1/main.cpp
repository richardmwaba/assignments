/*********************************************************************************
 * Author: Richard Mwaba
 * Title: main.cpp
 * Date: Monday, February 3, 2020
 * Description: Run the program
 * 
 *********************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "command.hpp"
#include "parser.hpp"
#include "executor.hpp"


int runCommands(std::vector<shell_command> shell_commands, int exit_status){
    for (size_t i = 0; i < shell_commands.size(); i++) {   
        pid_t pid;
        pid = fork();
        if (pid < 0) { /* error occurred */
            fprintf(stderr, "Fork Failed"); 
            return 1;
        }
        else if (pid == 0) { /* child process */
            int *file_desc = new int;
            mode_t mode = S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH;
            if(exit_status == 0 && shell_commands[i-1].next_mode == next_command_mode::on_success) {
                switch (shell_commands[i].cout_mode)
                {
                case ostream_mode::file:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_APPEND | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, 1) ;
                    break;
                case ostream_mode::append:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_APPEND | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, 1) ;
                    break;
                default:
                    break;
                }
                if(shell_commands[i].cin_mode == istream_mode::file) {
                    *file_desc = open(const_cast<char*>(shell_commands[i].cin_file.c_str()), O_RDONLY);
                    dup2(*file_desc, STDIN_FILENO);
                }
            } else if(exit_status > 0 && shell_commands[i-1].next_mode == next_command_mode::on_fail){
                
                switch (shell_commands[i].cout_mode)
                {
                case ostream_mode::file:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_APPEND | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, 1) ;
                    break;
                case ostream_mode::append:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_APPEND | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, 1) ;
                    break;
                default:
                    break;
                }
                if(shell_commands[i].cin_mode == istream_mode::file) {
                    *file_desc = open(const_cast<char*>(shell_commands[i].cin_file.c_str()), O_RDONLY);
                    dup2(*file_desc, STDIN_FILENO);
                }
            } else if(exit_status > 0 && shell_commands[i-1].next_mode == next_command_mode::on_success) {
                exit(1);
                continue;
            } else if (exit_status == 0 && shell_commands[i-1].next_mode == next_command_mode::on_fail) {
                exit(1);
                continue;
            }else {
                
                switch (shell_commands[i].cout_mode)
                {
                case ostream_mode::file:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_APPEND | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, 1) ;
                    break;
                case ostream_mode::append:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_APPEND | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, STDOUT_FILENO) ;
                    break;
                default:
                    break;
                }
                if(shell_commands[i].cin_mode == istream_mode::file) {
                    *file_desc = open(const_cast<char*>(shell_commands[i].cin_file.c_str()), O_RDONLY);
                    dup2(*file_desc, STDIN_FILENO);
                }
            }
            
            if(shell_commands[i].cin_mode == istream_mode::file) {
                exec2(shell_commands[i].cmd, shell_commands[i].cin_file);
            }
            exec(shell_commands[i].cmd, shell_commands[i].args);
            close(*file_desc);
            delete(file_desc);
            exit(1);
        }
        else { /* parent process */
        /* parent will wait for the child to complete */
            // wait(NULL);
            int status;
            wait(&status);
            if ( WIFEXITED(status) ) 
            { 
                exit_status = WEXITSTATUS(status);         
            }
        }                    
    }
    return 0;
}

int main(int argc, char* argv[])
{
    std::string input_line;
    std::string switchT = "-t";
    // pid_t pid;
    int exit_status = 0;


    if(argc >= 2 && switchT.compare(argv[1]) == 0) {
        for (std::string input_line; std::getline(std::cin, input_line); ) {
            if (input_line == "exit") {
                break;
            }
            try {
                std::vector<shell_command> shell_commands = parse_command_string(input_line);
                runCommands(shell_commands, exit_status);
            }
            catch (const std::runtime_error& e) {
                std::cout <<  e.what() << "\n";
            }
        }
    }else{
        for (;;) {
            // Print the prompt.
            std::cout << "osh> " << std::flush;
            // Read a single line.
            if (!std::getline(std::cin, input_line) || input_line == "exit") {
                break;
            }
            try {
                // Parse the input line.
                std::vector<shell_command> shell_commands
                        = parse_command_string(input_line);
    
                if(!shell_commands.empty()){
                    runCommands(shell_commands, exit_status);
                }
    
            }
            catch (const std::runtime_error& e) {
                std::cout << e.what() << "\n";
            }
        }
    }
    return 0;
}

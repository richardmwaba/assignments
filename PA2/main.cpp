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
#define WRITE_END 1
#define READ_END 0


int runCommands(std::vector<shell_command> shell_commands, int exit_status, int pipefd[2], int previouspipe){
    for (size_t i = 0; i < shell_commands.size(); i++) {
        if(shell_commands[i].cout_mode == ostream_mode::pipe) {
            if(pipe(pipefd) < 0) {
                fprintf(stderr, "Pipe Failed"); 
                return 1;
            }
        }

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
                if(shell_commands[i].cin_mode == istream_mode::file) {
                    *file_desc = open(const_cast<char*>(shell_commands[i].cin_file.c_str()), O_RDONLY);
                    dup2(*file_desc, STDIN_FILENO);
                }else if(shell_commands[i].cin_mode == istream_mode::pipe) {
                    dup2(previouspipe, STDIN_FILENO);
                    close(previouspipe);
                    close(pipefd[READ_END]);
                    if(shell_commands[i].cout_mode == ostream_mode::pipe){
                        dup2(pipefd[WRITE_END], STDOUT_FILENO);
                        close(pipefd[WRITE_END]);
                    }
                }
                switch (shell_commands[i].cout_mode)
                {
                case ostream_mode::file:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, STDOUT_FILENO) ;
                    break;
                case ostream_mode::append:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_APPEND | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, STDOUT_FILENO) ;
                    break;
                case ostream_mode::pipe:
                    if(shell_commands[i].cin_mode != istream_mode::pipe) {
                        dup2(pipefd[WRITE_END], STDOUT_FILENO);
                        close(pipefd[WRITE_END]);
                        close(pipefd[READ_END]);
                    }
                    break;
                default:
                    break;
                }
            } else if(exit_status > 0 && shell_commands[i-1].next_mode == next_command_mode::on_fail){
                if(shell_commands[i].cin_mode == istream_mode::file) {
                    *file_desc = open(const_cast<char*>(shell_commands[i].cin_file.c_str()), O_RDONLY);
                    dup2(*file_desc, STDIN_FILENO);
                }else if(shell_commands[i].cin_mode == istream_mode::pipe) {
                    dup2(previouspipe, STDIN_FILENO);
                    close(previouspipe);
                    close(pipefd[READ_END]);
                    if(shell_commands[i].cout_mode == ostream_mode::pipe){
                        dup2(pipefd[WRITE_END], STDOUT_FILENO);
                        close(pipefd[WRITE_END]);
                    }
                }
                switch (shell_commands[i].cout_mode)
                {
                case ostream_mode::file:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, STDOUT_FILENO) ;
                    break;
                case ostream_mode::append:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_APPEND | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, STDOUT_FILENO) ;
                    break;
                case ostream_mode::pipe:
                    if(shell_commands[i].cin_mode != istream_mode::pipe) {
                        dup2(pipefd[WRITE_END], STDOUT_FILENO);
                        close(pipefd[WRITE_END]);
                        close(pipefd[READ_END]);
                    }
                    break;
                default:
                    break;
                }
            } else if(exit_status > 0 && shell_commands[i-1].next_mode == next_command_mode::on_success) {
                exit(1);
                continue;
            } else if (exit_status == 0 && shell_commands[i-1].next_mode == next_command_mode::on_fail) {
                exit(0);
                continue;
            }else {
                if(shell_commands[i].cin_mode == istream_mode::file) {
                    *file_desc = open(const_cast<char*>(shell_commands[i].cin_file.c_str()), O_RDONLY);
                    dup2(*file_desc, STDIN_FILENO);
                }else if(shell_commands[i].cin_mode == istream_mode::pipe) {
                    dup2(previouspipe, STDIN_FILENO);
                    close(previouspipe);
                    close(pipefd[READ_END]);
                    if(shell_commands[i].cout_mode == ostream_mode::pipe){
                        dup2(pipefd[WRITE_END], STDOUT_FILENO);
                        close(pipefd[WRITE_END]);
                    }
                }
                switch (shell_commands[i].cout_mode)
                {
                case ostream_mode::file:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, STDOUT_FILENO) ;
                    break;
                case ostream_mode::append:
                    *file_desc = open(const_cast<char*>(shell_commands[i].cout_file.c_str()), O_WRONLY | O_APPEND | O_CREAT, mode);
                    // here the newfd is the file descriptor of stdout (i.e. 1) 
                    dup2(*file_desc, STDOUT_FILENO) ;
                    break;
                case ostream_mode::pipe:
                    if(shell_commands[i].cin_mode != istream_mode::pipe) {
                        dup2(pipefd[WRITE_END], STDOUT_FILENO);
                        close(pipefd[WRITE_END]);
                        close(pipefd[READ_END]);
                    }
                    break;
                default:
                    break;
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
            if(shell_commands[i].cout_mode == ostream_mode::pipe){
                if(shell_commands[i].cin_mode == istream_mode::pipe) {
                    close(previouspipe);
                }
                previouspipe = pipefd[READ_END];
                close(pipefd[WRITE_END]);
            }else{
                // close(previouspipe);
                close(pipefd[WRITE_END]);
            }

            if(shell_commands[i].cout_mode != ostream_mode::pipe) {
                int status;
                waitpid(pid,&status,0);
                if ( WIFEXITED(status) ) 
                { 
                    exit_status = WEXITSTATUS(status);         
                }
            }
            
            wait(NULL); //Reap all zombies
        }                    
    }
    return 0;
}

int main(int argc, char* argv[])
{
    std::string input_line;
    std::string switchT = "-t";
    int pipefd[2], previouspipe = 0;
    int exit_status = 0;

    if(argc >= 2 && switchT.compare(argv[1]) == 0) {
        for (std::string input_line; std::getline(std::cin, input_line); ) {
            if (input_line == "exit") {
                break;
            }
            try {
                std::vector<shell_command> shell_commands = parse_command_string(input_line);

                runCommands(shell_commands, exit_status, pipefd, previouspipe);
            }
            catch (const std::runtime_error& e) {
                std::cout <<  e.what() << "\n";
            }
        }
    }else{
        for (int i = 0;i < 25;i++) {
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
                    runCommands(shell_commands, exit_status, pipefd, previouspipe);
                }
    
            }
            catch (const std::runtime_error& e) {
                std::cout << e.what() << "\n";
            }
        }
    }
    return 0;
}

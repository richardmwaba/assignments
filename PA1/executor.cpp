/*********************************************************************************
 * Author: Richard Mwaba
 * Title: main.cpp
 * Date: Sunday, February 2, 2020
 * Description: Get the string arguments from the command vector, cast them into 
 * C-style as args and pass them to the execvp for execution.
 * 
 * Largely inspired by Yutaka Tsutano's hints on executing commands from his parser
 * which is used in this program
 * 
 *********************************************************************************/

#include <string>
#include <vector>
#include <unistd.h>


#include "executor.hpp"

int exec(const std::string& cmd, const std::vector<std::string>& args)
{
    // Make an ugly C-style args array.
    std::vector<char*> c_args = {const_cast<char*>(cmd.c_str())};
    for (const auto& a : args) {
        c_args.push_back(const_cast<char*>(a.c_str()));
    }
    c_args.push_back(nullptr);

    return execvp(cmd.c_str(), c_args.data());
}

int exec2(const std::string& cmd, const std::string& cin_file)
{
    // Make an ugly C-style args array.
    std::vector<char*> c_args = {const_cast<char*>(cmd.c_str())};
    c_args.push_back(const_cast<char*>(cin_file.c_str()));
    
    c_args.push_back(nullptr);

    return execvp(cmd.c_str(), c_args.data());
}
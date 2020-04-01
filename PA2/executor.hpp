/*********************************************************************************
 * Author: Richard Mwaba
 * Title: main.cpp
 * Date: Sunday, February 2, 2020
 * Description: Get the string arguments from the command vector, cast them into 
 * C-style as args and pass them to the execvp for execution.
 * 
 * Largely inspired by Yutaka Tsutano's hints on executing commands from his parser
 * which is being used in this program
 * 
 *********************************************************************************/

#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <vector>
#include <string>

/// Convert input to C-style args array and prepare for execution.
///
/// @return the execvp function for execution
int exec(const std::string& cmd, const std::vector<std::string>& args);

/// Convert input to C-style args array and prepare for execution.
///
/// @return the execvp function for execution
int exec2(const std::string& cmd, const std::string& cin_file);

#endif
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { 

    return pid_nr_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {

        return LinuxParser::ActiveJiffies(pid_nr_);    }
    


// DONE: Return the command that generated this process
string Process::Command() {
    
     return LinuxParser::Command(pid_nr_); }

// DONE: Return this process's memory utilization
string Process::Ram() {
  
     return LinuxParser::Ram(pid_nr_); }

// DONE: Return the user (name) that generated this process
string Process::User() { 
       
    return LinuxParser::User(pid_nr_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 



    return LinuxParser::UpTime(pid_nr_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
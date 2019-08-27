#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { 
   // int temp_pid;
    //temp_pid = LinuxParser::Pids();
    return pid_nr_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    
    std::vector<string> cpu_pids = LinuxParser::CpuUtilization();
    float pid_cpu_usage;

    for (int i =0; i< cpu_pids.size(); i++){

        pid_cpu_usage = std::stof(cpu_pids[i]);
        return  pid_cpu_usage;
            }
    
}

// TODO: Return the command that generated this process
string Process::Command() {
    
     return LinuxParser::Command(pid_nr_); }

// TODO: Return this process's memory utilization
string Process::Ram() {
    
    string ram_utylization, ram_utylization_mb;

    //ram_utylization = LinuxParser::Ram(pid_nr_);
    //float ram_mb = std::stof(ram_utylization);
    //ram_mb=0.001 * ram_mb;
    //ram_utylization_mb = std::to_string(ram_mb); //pid);
    
    
     return LinuxParser::Ram(pid_nr_); }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    
    //call linux_parser - User()
    string uid_user;

    uid_user = LinuxParser::User(pid_nr_);
    
    return uid_user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 

    return LinuxParser::UpTime(pid_nr_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
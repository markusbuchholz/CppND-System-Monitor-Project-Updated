#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

#include<iostream> //remove
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  
      vector<int> pids = LinuxParser::Pids();
   
      for (int i =0; i<pids.size(); i++){
       
        processes_.push_back(pids[i]);
    }
  return processes_; }

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
    //std::cout<<"your kernel is ::" << string() <<std::endl;
    std::string os_kernel;
    os_kernel = LinuxParser::Kernel();
     return os_kernel; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  float mem_utilization;
  mem_utilization = LinuxParser::MemoryUtilization();
  return mem_utilization; }

// DONE: Return the operating system name
std::string System::OperatingSystem() {
  std::string os_name;  
    os_name = LinuxParser::OperatingSystem();
     return os_name; } 

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() {
  int number_of_running_proc;
  number_of_running_proc = LinuxParser::RunningProcesses();
  return number_of_running_proc;
}

// DONEO: Return the total number of processes on the system
int System::TotalProcesses() {
  int total_number_of_proc;
  total_number_of_proc = LinuxParser::TotalProcesses();
   return total_number_of_proc; }

// DONE: Return the number of seconds since the system started running
int long System::UpTime() {

  long system_uptime= LinuxParser::UpTime();
  //std::cout<<"function called :: "<<system_uptime<<std::endl;
   return LinuxParser::UpTime();} 
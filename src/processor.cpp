/*

#include "processor.h"

// TODO: Return the aggregate CPU utilization
//https://github.com/Leo-G/DevopsWiki/wiki/How-Linux-CPU-Usage-Time-and-Percentage-is-calculated
float Processor::Utilization() { return 0.0; }

*/

#include <thread>
#include <chrono>
#include "processor.h"
#include "linux_parser.h"

#include <iostream>
 //#include <unistd.h> // for usleep

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 

    std::vector<std::string> system_state_1 = LinuxParser::CpuUtilization();
    float activeTime_1 = LinuxParser::ActiveJiffies(system_state_1);
    float idleTime_1 = LinuxParser::IdleJiffies(system_state_1);
    //https://stackoverflow.com/questions/4184468/sleep-for-milliseconds

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // Wait 100ms
	//usleep(100000); // in microseconds
   

    std::vector<std::string> system_state_2 = LinuxParser::CpuUtilization();
    float activeTime_2 = LinuxParser::ActiveJiffies(system_state_2);
    float idleTime_2 = LinuxParser::IdleJiffies(system_state_2);

    float  activeTime  = activeTime_2 - activeTime_1;

    float  idleTime  = idleTime_2 - idleTime_1;
   
    float  totalTime = activeTime + idleTime;
 
    float  result = (activeTime / totalTime);

 
    return result;
}
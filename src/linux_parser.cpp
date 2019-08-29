#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include<algorithm>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, ver, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> ver >> kernel;
  }

  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization

//https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
//Total used memory = MemTotal - MemFree

float LinuxParser::MemoryUtilization(){
  string line;
  string line2;
  string key;
  float mem_total;
  float mem_free;
  float mem_value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

      std::istringstream linestream(line);
      while (linestream >> key >> mem_value) {
        if (key == "MemTotal:") {

          mem_total = mem_value;
        }
      }
      
          while (std::getline(filestream, line2)) {

      std::istringstream linestream2(line2);
      while (linestream2 >> key >> mem_value) {
        if (key == "MemFree:") {

          mem_free = mem_value;
          
        }
      }
      }
     
    }
  }
  
  return (mem_total - mem_free)/mem_total;

}


// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
 // std::cout<<"CALLED FUNCTION"<<std::endl;
  string line;
  float timer1, timer2;
  int timer12;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
    while (std::getline(filestream, line)) {

      std::istringstream linestream(line);
      while (linestream >> timer1 >> timer2) {
        //std::cout<<" :: " << timer1+timer2<<std::endl;

        timer12 = int(timer1);
      return long(timer12);
      }
    }
  
  
  return long(timer12);
}



// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function





long LinuxParser::ActiveJiffies(int pid) {
  
  string pid_string;
  string line;

  string nr1, nr2, nr3, nr4, nr5, nr6, nr7, nr8, nr9, nr10,nr11, nr12, nr13, nr14, nr15,nr16, nr17, nr18, nr19, nr20,nr21, nr22;
  double total_time, utime, stime, cutime, cstime, uptime, seconds,starttime, hertz, pid_cpu_usage;
  //pid = 16737;

  //std::cout<<"function :: "<<__FUNCTION__ << "called" <<std::endl;
  pid_string = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + "/" + pid_string + kStatFilename);
  // if(filestream.is_open()){
  while (std::getline(filestream, line)) {
    std::istringstream linestream_proc(line);
    while (linestream_proc >> nr1 >>nr2 >>nr3 >>nr4>> nr5>> nr6>> nr7>> nr8>> nr9>> nr10>>nr11>> nr12>> nr13>> nr14>> nr15>> nr16>> nr17>> nr18>> nr19>> nr20>>nr21>> nr22) {
      utime = std::stof(nr14);
      stime = std::stof(nr15);
      cutime = std::stof(nr16);
      cstime = std::stof(nr17);
      starttime = std::stof(nr22);
      uptime = LinuxParser::UpTime();
      hertz = sysconf(_SC_CLK_TCK);

      total_time = utime + stime;
      total_time = total_time + cutime + cstime;
      seconds = uptime - (starttime/hertz);
      pid_cpu_usage = 1 * ((total_time/hertz)/seconds); 
          
      //std::cout<<"CPU :: "<< pid_cpu_usage<<std::endl;
      return pid_cpu_usage;      
      }
      
    }
    
   }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::Jiffies() { 
  std::vector<std::string> values = LinuxParser::CpuUtilization();
  float activeTime = LinuxParser::ActiveJiffies(values);
  float idleTime = LinuxParser::IdleJiffies(values);  
  return activeTime+idleTime; 
}

// TODO: Read and return the number of idle jiffies for the system
// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(std::vector<std::string> values) { 
 // std::cout<<"size of vector ::" <<values.size()<<std::endl;
 // return 10.0;}
  
 long active_jiffies_value =  std::stof(values[CPUStates::kUser_]) +
            std::stof(values[CPUStates::kNice_]) +
            std::stof(values[CPUStates::kSystem_]) +
            std::stof(values[CPUStates::kIRQ_]) +
            std::stof(values[CPUStates::kSoftIRQ_]) +
            std::stof(values[CPUStates::kSteal_]) +
            std::stof(values[CPUStates::kGuest_]) +
            std::stof(values[CPUStates::kGuestNice_]);
return active_jiffies_value;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(std::vector<std::string> values) { 
  return (std::stof(values[CPUStates::kIdle_]) +
            std::stof(values[CPUStates::kIOwait_])); 
}


//https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  string name = "cpu";  
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(),name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            return values;
        }
    }  
  }
  return {}; 
}
  


// TODO: Read and return the total number of processes
//int LinuxParser::TotalProcesses() { return 0; }

int LinuxParser::TotalProcesses(){
  string line;
  string key;
  int total_processes;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

      std::istringstream linestream(line);
      while (linestream >> key >> total_processes) {
        if (key == "processes") {

          return total_processes;
        }
      }
    }
  }
  
  return total_processes;
}

// TODO: Read and return the number of running processes
//int LinuxParser::RunningProcesses() { return 0; }
int LinuxParser::RunningProcesses(){
  string line;
  string key;
  int running_processes;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

      std::istringstream linestream(line);
      while (linestream >> key >> running_processes) {
        if (key == "procs_running") {
          return running_processes;
        }
      }
    }
  }
  return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  
  string line;
  string key;
  std::string pid_string = std::to_string(pid); //pid);
  std::ifstream filestream_proc(kProcDirectory + "/" + pid_string + kCmdlineFilename);

    while (std::getline(filestream_proc, line)) {

      std::istringstream linestream_proc(line);
      while (linestream_proc >> key ) {
       
          //std::cout<<"####"<<nr1<<std::endl;
          //std::cout<<" cmd :: "<< key << std::endl;  
          return key;            
      }
    }
    
    
}
  


// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function



string LinuxParser::Ram(int pid) { 
  
  string ram_value;
  string line;
  string key;
  float ram_value_f;
  std::string pid_string = std::to_string(pid); //pid);
  
  std::ifstream filestream(kProcDirectory + "/" + pid_string + kStatusFilename);

    while (std::getline(filestream, line)) {

      std::istringstream linestream(line);
      while (linestream>> key >> ram_value) {
        if (key == "VmSize:") {
          //std::cout<<"####"<<nr1<<std::endl;
          ram_value_f = std::stof(ram_value)*0.001024;
          //ram_value_f = 0.001024 * ram_value_f;

           //return std::to_string(ram_value_f); // here take a value
          //nr1_return = User_aux(nr1_return);
          //return nr1_return;
        }
      }
    }
     return std::to_string(ram_value_f).substr(0,7);
    
    //nr1_return = User_aux(nr1_return);
    //return ram_value;
  
}

/*

//DONE:Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string name = "VmSize";
  string value;
  double result;
  std::ifstream stream(kProcDirectory + to_string(pid)  + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
        if (line.compare(0, name.size(),name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result = (stof(values[1])/double(1024));
            break;
        }
    }
  }
  return to_string(result).substr(0,7);
}
*/



// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }





// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  //string uid;
  //AAAAAAAAAAAAAAAAAAA
  string nr1, nr2, nr3, nr4, nr1_return;
  string line;
  string key;
  std::string pid_string = std::to_string(pid); //pid);
  std::ifstream filestream_proc(kProcDirectory + "/" + pid_string + kStatusFilename);

    while (std::getline(filestream_proc, line)) {

      std::istringstream linestream_proc(line);
      while (linestream_proc >> key >> nr1 >>nr2 >>nr3 >>nr4 ) {
        if (key == "Uid:") {
          //std::cout<<"####"<<nr1<<std::endl;
          nr1_return = nr1; // here take a value
          //nr1_return = User_aux(nr1_return);
          //return nr1_return;
        }
      }
    }
    
    nr1_return = User_aux(nr1_return);
    return nr1_return;
}


string LinuxParser::User_aux(string uid_string){

//std::cout<<"AUX called"<<std::endl;

  string nr1, nr2, nr3, nr4, nr5, nr6;
  string line;
  string key;

  std::ifstream filestream_proc(kPasswordPath);
  string aux_string = ":" + uid_string + ":";
  std::vector<std::string> temp_string;
  string loop_string;
  int i = 0;


    while (std::getline(filestream_proc, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');


      std::istringstream linestream_proc(line);
      linestream_proc >> nr1>>nr2>> nr3>>nr4>> nr5>>nr6;
      if(nr3 == uid_string){
        //std::cout<<nr1<< " :: "<<uid_string << std::endl;
      return nr1;
      }

      }

      //return nr1;

}







//std::cout<<"222222"<<std::endl;
 // return nr1;
  
   //}

   




// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function

long LinuxParser::UpTime(int pid) {
  
  string nr1, nr2, nr3, nr4, nr5, nr6, nr7, nr8, nr9, nr10,nr11, nr12, nr13, nr14, nr15,nr16, nr17, nr18, nr19, nr20,nr21, nr22;
  string line;
  long proc_uptime;
  //pid = 13678;
  std::string pid_string = std::to_string(pid); //pid);
  //std::cout<<"PID :: "<<pid_string<<std::endl;
  std::ifstream filestream_proc(kProcDirectory + "/" + pid_string + kStatFilename);

    while (std::getline(filestream_proc, line)) {
      //std::cout<<"CALL TIME" <<std::endl;

      std::istringstream linestream_proc(line);
      while (linestream_proc >> nr1 >>nr2 >>nr3 >>nr4>> nr5>> nr6>> nr7>> nr8>> nr9>> nr10>>nr11>> nr12>> nr13>> nr14>> nr15>> nr16>> nr17>> nr18>> nr19>> nr20>>nr21>> nr22) {
      //std::cout<<"PID UP TIME :: "<< nr22 <<std::endl;
      proc_uptime = long(std::stoi(nr22)/sysconf(_SC_CLK_TCK));
      return proc_uptime;
      
      //return proc_uptime/sysconf(_SC_CLK_TCK);
          //nr1_return = User_aux(nr1_return);
          //return nr1_return;
        }
        
      }
    
    
    
}

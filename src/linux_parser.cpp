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

// DONE: Read and return the system memory utilization

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


// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
 // std::cout<<"CALLED FUNCTION"<<std::endl;
  string line;
  float timer1, timer2;
  int timer12;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

      std::istringstream linestream(line);
      while (linestream >> timer1 >> timer2) {
        //std::cout<<" :: " << timer1+timer2<<std::endl;

        timer12 = int(timer1);
      return long(timer12);
      }
    }
  
  }
  return long(timer12);
}



// DONE: Read and return the number of active jiffies for a PID

long LinuxParser::ActiveJiffies(int pid) {
  
  string pid_string;
  string line;

  string nr1, nr2, nr3, nr4, nr5, nr6, nr7, nr8, nr9, nr10,nr11, nr12, nr13, nr14, nr15,nr16, nr17, nr18, nr19, nr20,nr21, nr22;
  double total_time, utime, stime, cutime, cstime, uptime, seconds,starttime, hertz, pid_cpu_usage;
  //pid = 16365;
  //pid = 2012;

  //std::cout<<"function :: "<<__FUNCTION__ << "called" <<std::endl;
  pid_string = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + "/" + pid_string + kStatFilename);
   if(filestream.is_open()){
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
      pid_cpu_usage = 100 * ((total_time/hertz)/seconds); 
          
      //std::cout<<"CPU :: "<< pid_cpu_usage<<std::endl;
      return pid_cpu_usage;      
      }
      
    }
   }
   }

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::Jiffies() {

  std::vector<std::string> values = LinuxParser::CpuUtilization();

  float activeTime = LinuxParser::ActiveJiffies(values);
  float idleTime = LinuxParser::IdleJiffies(values);  
  return activeTime+idleTime; 
}


// DONE: Read and return the number of active jiffies for the system

long LinuxParser::ActiveJiffies(std::vector<std::string> vector_jiffies) { 

long active_jiffies_value = std::stof(vector_jiffies[0]) + std::stof(vector_jiffies[1]) + std::stof(vector_jiffies[2]) +
                            std::stof(vector_jiffies[5]) + std::stof(vector_jiffies[6]) + std::stof(vector_jiffies[7]) +
                            std::stof(vector_jiffies[8]) + std::stof(vector_jiffies[9]);


return active_jiffies_value;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(std::vector<std::string> vector_jiffies) { 
 
  return (std::stof(vector_jiffies[3]) + std::stof(vector_jiffies[4])); // 5.0;//(std::stof(values[CPUStates::kIdle_]) + std::stof(values[CPUStates::kIOwait_])); 
}


//https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599


// DONE: Read and return CPU utilization

//cpu_vetor = {"215371", "782", "64744", "11161394", "1210", "0", "14752", "0", "0", "0"};

vector <string> LinuxParser::CpuUtilization() {
  
 
  string line;
  string key, nr1, nr2, nr3, nr4, nr5, nr6, nr7, nr8, nr9, nr10;
  vector <string> cpu_vector;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

      std::istringstream linestream(line);
      //while (linestream >> key >>nr1 >>nr2 >>nr3 >>nr4>> nr5>> nr6>> nr7>> nr8>> nr9>> nr10 ) {
      linestream >> key >>nr1 >>nr2 >>nr3 >>nr4>> nr5>> nr6>> nr7>> nr8>> nr9>> nr10;  
        if (key == "cpu") {

          vector <string> cpu_vector;
          cpu_vector.push_back(nr1);
          cpu_vector.push_back(nr2);
          cpu_vector.push_back(nr3);
          cpu_vector.push_back(nr4);
          cpu_vector.push_back(nr5);
          cpu_vector.push_back(nr6);
          cpu_vector.push_back(nr7);
          cpu_vector.push_back(nr8);
          cpu_vector.push_back(nr9);
          cpu_vector.push_back(nr10);
         
          return cpu_vector;
        }
        
      //}
    
    }

   

  }
  
}

// DONE: Read and return the total number of processes


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

// DONE: Read and return the number of running processes

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

// DONE: Read and return the command associated with a process

string LinuxParser::Command(int pid) { 
  
  string line;
  string key;
  std::string pid_string = std::to_string(pid); 
  std::ifstream filestream_proc(kProcDirectory + "/" + pid_string + kCmdlineFilename);
if (filestream_proc.is_open()) {
    while (std::getline(filestream_proc, line)) {

      std::istringstream linestream_proc(line);
      while (linestream_proc >> key ) {

          return key;            
      }
    }
} 
    
}
  
// DONE: Read and return the memory used by a process




string LinuxParser::Ram(int pid) { 
  
  string ram_value;
  string line;
  string key;
  float ram_value_f;
  std::string pid_string = std::to_string(pid); //pid);
  
  std::ifstream filestream(kProcDirectory + "/" + pid_string + kStatusFilename);
  if (filestream.is_open()) {

    while (std::getline(filestream, line)) {

      std::istringstream linestream(line);
      while (linestream>> key >> ram_value) {
        if (key == "VmSize:") {
         
          ram_value_f = std::stof(ram_value)*0.001024;

        }
      }
    }
     return std::to_string(ram_value_f).substr(0,7);
    
  }
  
}

// DONE: Read and return the user associated with a process

string LinuxParser::User(int pid) { 

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

        }
      }
    }
    
    nr1_return = LinuxParser::Uid(nr1_return); // User_aux(nr1_return);
    return nr1_return;
}

  // DONE: Read and return the user ID associated with a process

string LinuxParser::Uid(string pid_string) {


  string uid, nr2, nr3, nr4, nr5, nr6;
  string line;
  string key;

  std::ifstream filestream_proc(kPasswordPath);



    while (std::getline(filestream_proc, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');


      std::istringstream linestream_proc(line);
      linestream_proc >> uid>>nr2>> nr3>>nr4>> nr5>>nr6;
      if(nr3 == pid_string){
          
      return uid;
      }

      }



}


// DONE: Read and return the uptime of a process


long LinuxParser::UpTime(int pid) {
  
  string nr1, nr2, nr3, nr4, nr5, nr6, nr7, nr8, nr9, nr10,nr11, nr12, nr13, nr14, nr15,nr16, nr17, nr18, nr19, nr20,nr21, nr22;
  string line;
  long proc_uptime;
  //pid = 13678;
  std::string pid_string = std::to_string(pid); //pid);
  //std::cout<<"PID :: "<<pid_string<<std::endl;
  std::ifstream filestream_proc(kProcDirectory + "/" + pid_string + kStatFilename);

    while (std::getline(filestream_proc, line)) {

      std::istringstream linestream_proc(line);
      while (linestream_proc >> nr1 >>nr2 >>nr3 >>nr4>> nr5>> nr6>> nr7>> nr8>> nr9>> nr10>>nr11>> nr12>> nr13>> nr14>> nr15>> nr16>> nr17>> nr18>> nr19>> nr20>>nr21>> nr22) {
    
      proc_uptime = long(std::stoi(nr22)/sysconf(_SC_CLK_TCK));
      return proc_uptime;

        }
        
      }
    
    
    
}

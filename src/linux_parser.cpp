#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <iostream>
#include <typeinfo>

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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
float LinuxParser::MemoryUtilization() { 
  float memTotal, memFree, memAvailable, buffers, cached;
  string line, key, value;

  vector<string> keys;
  vector<string> vals;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  // kMeminfoFilename
  if (filestream.is_open()) {
    for (int i=0; i<5; i++) {
      std::getline(filestream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      keys.push_back(key);
      vals.push_back(value);
    }
  }

  memTotal = std::stof(vals[0]);
  memFree = std::stof(vals[1]);
  buffers = std::stof(vals[3]);
  cached = std::stof(vals[4]);

  string moo = typeid(memTotal).name();
  // memAvailable = vals[2];
  // float memUtil = (memTotal - memFree) / memTotal;  
  
  float memUtil = (memTotal - (memFree + buffers + cached)) / memTotal;    
  // from: https://sites.google.com/a/thetnaing.com/therunningone/how-to-calculate-systems-memory-utilization

  return memUtil; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string up_time, idle_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time >> idle_time;
  }
  return stol(up_time); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  auto jiffies = LinuxParser::CpuUtilization();
  long total_jiffies = stol(jiffies[kUser_]) + stol(jiffies[kNice_])+ stol(jiffies[kSystem_])
  + stol(jiffies[kIdle_])+ stol(jiffies[kIOwait_])+ stol(jiffies[kIRQ_])+ stol(jiffies[kSoftIRQ_])
  + stol(jiffies[kSteal_])+ stol(jiffies[kGuest_])+ stol(jiffies[kGuestNice_]);

  return total_jiffies;   
  // return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  // auto vals = LinuxParser::CpuUtilization(pid);
//   #14 utime - CPU time spent in user code, measured in clock ticks
// #15 stime - CPU time spent in kernel code, measured in clock ticks
  // int ind_utime = 14+1;
  // int ind_stime = 15+1;

  // long active = vals[ind_utime] + vals[ind_stime];
// #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
// #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
// #22 starttime - Time when the process started, measured in clock ticks
  
  return 0; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  auto jiffies = LinuxParser::CpuUtilization();
  long active_jiffies = stol(jiffies[kUser_]) + stol(jiffies[kNice_])+ stol(jiffies[kSystem_]);
  return active_jiffies; 
  // return 0; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto jiffies = LinuxParser::CpuUtilization();
  long idle_jiffies = stol(jiffies[kIdle_]) + stol(jiffies[kIOwait_]);
  return idle_jiffies; 
}

// TODO: Read and return CPU utilization - https://www.idnt.net/en-GB/kb/941772
vector<string> LinuxParser::CpuUtilization() { 
  string user, nice, system, val;
  vector<string> jiffies;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> val){
      jiffies.push_back(val);
    }
  }
  jiffies.erase(jiffies.begin()); // https://stackoverflow.com/questions/40656871/remove-from-the-beginning-of-stdvector
  
  return jiffies; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, value;
  int proc_val;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") 
        linestream >> value;
    }
  }
  proc_val = stoi(value);
  // return stoi(proc_val);
  return proc_val;
  // return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, value;
  int proc_val;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running")
        linestream >> value;
    }
  }
  proc_val = stoi(value);
  // return stoi(proc_val);
  return proc_val;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { 
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
  // return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { 
  string key, value;
  int proc_val;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:") 
        linestream >> value;
        // break;
    }
  }
  // value;
  float mem = stol(value)/1000;

  return to_string(mem);   
  // return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { 
  string key, value;
  int proc_val;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") 
        linestream >> value;
        // break;
    }
  }
  // value;
  return value; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { 
  string name, pass, uid, line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> pass >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          return name;
        }
      }
    }
  }
  return name;  
  // return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { 
  auto data = LinuxParser::CpuUtilization(pid);
  long up_time = stol(data[23])/sysconf(_SC_CLK_TCK);
  return up_time;
  // return 0; 
}


//CPUUtilization
vector<string> LinuxParser::CpuUtilization(int pid[[maybe_unused]]) 
{ 
  string line, val;
  vector<string> values;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> val){
      values.push_back(val);
    }
  }
  return values;
}

#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <iostream>
#include <typeinfo>

#include <iomanip>      // std::setprecision

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::cleanStr(string str){
  // check if it is empty or not before transforming it into long using stol method
  if (str == "" || str == "/t")   
    str = "0";
  // remove all characters that arent digits
  auto it = std::remove_if(str.begin(), str.end(), [](char const &c) {
      return !std::isdigit(c);
  });
  str.erase(it, str.end());

  return str;
}

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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string memTotal = "MemTotal:";
  string memFree = "MemFree:";
  float Total = findValueByKey<float>(memTotal, kMeminfoFilename);// "/proc/memInfo"
  float Free = findValueByKey<float>(memFree, kMeminfoFilename);
  return (Total - Free) / Total;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string up_time = std::string(getValueOfFile<std::string>(kUptimeFilename));
  return stol(up_time); 
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  auto jiff = LinuxParser::CpuUtilization();
  vector<string> jiffies{};
  for(string s : jiff)
    jiffies.emplace_back(cleanStr(s));

  long total_jiffies = stol(jiffies[kUser_]) + stol(jiffies[kNice_])+ stol(jiffies[kSystem_])
  + stol(jiffies[kIdle_])+ stol(jiffies[kIOwait_])+ stol(jiffies[kIRQ_])+ stol(jiffies[kSoftIRQ_])
  + stol(jiffies[kSteal_])+ stol(jiffies[kGuest_])+ stol(jiffies[kGuestNice_]);

  return total_jiffies;   
  // return 0;
}

// Read and return the number of active jiffies for a PID
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

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  auto jiffies = LinuxParser::CpuUtilization();
  long active_jiffies = stol(jiffies[kUser_]) + stol(jiffies[kNice_])+ stol(jiffies[kSystem_]);
  return active_jiffies; 
  // return 0; 
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto jiffies = LinuxParser::CpuUtilization();
  long idle_jiffies = stol(jiffies[kIdle_]) + stol(jiffies[kIOwait_]);
  return idle_jiffies; 
}

// Read and return CPU utilization - https://www.idnt.net/en-GB/kb/941772
vector<string> LinuxParser::CpuUtilization() { 
  string user, nice, system, val;
  vector<string> jiffies;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> val){
      jiffies.push_back(cleanStr(val));
    }
  }
  jiffies.erase(jiffies.begin()); // https://stackoverflow.com/questions/40656871/remove-from-the-beginning-of-stdvector
  
  return jiffies; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string value = findValueByKey<string>("processes", kStatFilename);
  return stoi(value);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string value = findValueByKey<string>("procs_running", kStatFilename);
  return stoi(value);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  return std::string(getValueOfFile<std::string>(std::to_string(pid) + kCmdlineFilename));
}
// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmRSS:"){
        linestream >> value;
        value = cleanStr(value);
        break;
      }
    }
    stol(value);  // initiate stol in  if (stream.is_open())
  }

  // used VmRSS, instead of VmSize
  // string value = findValueByKey<string>("VmRSS:", to_string(pid) + kStatusFilename);
  string str = cleanStr(value);
  int mem = stol(value)/1000; // initiate stol
  return to_string(mem);   
  // return string(); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string value = findValueByKey<string>("Uid:", to_string(pid) + kStatusFilename);
  return value; 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
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

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  auto data = LinuxParser::CpuUtilization(pid);
  string up_time_str = cleanStr(data[21]);
  long up_time = stol(up_time_str)/sysconf(_SC_CLK_TCK);
  return up_time;
  // return 0; 
}


//CPUUtilization
vector<string> LinuxParser::CpuUtilization(int pid) 
{ 
  string line, val;
  vector<string> values;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> val){
      string valStr = cleanStr(val);  //clean and add if its a string
      values.emplace_back(valStr);
    }
  }
  
  return values;
}

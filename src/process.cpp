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

Process::Process(int pid) : pid_(pid){}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    // long active = LinuxParser::ActiveJiffies(pid_);
    // long up_time = LinuxParser::UpTime(pid_);
    // utilization_ = active / up_time;

    // try {
    //     utilization_ = float(active) / float(up_time);
    // } catch (...) {
    //     utilization_ = 0;
    // }
    // return utilization_; 
    return 0.0;
}

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_);
    // return string();
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { 
    return true; 
}
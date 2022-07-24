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

Process::Process(int pid) : pid_(pid){
    utilization_ = LinuxParser::CpuUtilization(pid);
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
//   https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
//   vector<string> vals = LinuxParser::CpuUtilization(pid_);
vector<string> vals = utilization_;
// #14 utime - CPU time spent in user code, measured in clock ticks
// #15 stime - CPU time spent in kernel code, measured in clock ticks
// #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
// #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
    long hz = sysconf(_SC_CLK_TCK);
    //adding one to get the correct index
    int ind_utime = 15;
    int ind_stime = 16;
    int ind_cutime = 17;
    int ind_cstime = 18;

// #22 starttime - Time when the process started, measured in clock ticks
    int ind_starttime = 23;

    long start_time, up_time, total_time;

    float seconds;
   // In most cases, getconf CLK_TCK can be used to return the number of clock ticks.
    // The sysconf(_SC_CLK_TCK) C function call may also be used to return the hertz value.
    start_time = stol(LinuxParser::cleanStr(vals[ind_starttime]));

    up_time = LinuxParser::UpTime(); 

    total_time = stol(LinuxParser::cleanStr(vals[ind_utime])) + stol(LinuxParser::cleanStr(vals[ind_stime])) +
     stol(LinuxParser::cleanStr(vals[ind_cutime])) + stol(LinuxParser::cleanStr(vals[ind_cstime]));

    seconds = up_time - (start_time / hz);

    float test = (float(total_time) / float(hz)) / seconds;

    cpu_usage_ = test;

    try
    {
        /* code */
        cpu_usage_ = (float(total_time) / float(hz)) / seconds;
    }
    catch(...)
    {
        cpu_usage_ = 0.0;
    }
    

    return cpu_usage_;
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_);
    // return string(); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    string user = LinuxParser::User(pid_);
    return user;
    // return string();
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { 
    return a.cpu_usage_ < cpu_usage_; 
}
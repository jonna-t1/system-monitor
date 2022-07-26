#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

Processor::Processor() :
    prevTotal_(LinuxParser::Jiffies()),
    prevActive_(LinuxParser::ActiveJiffies()) {}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // https://www.idnt.net/en-GB/kb/941772
    long active = LinuxParser::ActiveJiffies();
    long total = LinuxParser::Jiffies();

    float delta_active;
    float delta_total;

    if (active > prevActive_){
        delta_active = active - prevActive_;
    }else {
        delta_active = 0;
    }

    if (total > prevTotal_){
        delta_total = total - prevTotal_;
    }else{
        delta_total = 1;
    }

    float utilization_ = delta_active / delta_total;
    // float utilization_;

    try {
        utilization_ = delta_active / delta_total;
    } catch (...) {
        utilization_ = 0;
    }
    return utilization_; 

}
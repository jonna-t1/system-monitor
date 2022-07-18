#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

Processor::Processor() :
    prevActive_(LinuxParser::ActiveJiffies()),
    prevTotal_(LinuxParser::Jiffies()) {}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // https://www.idnt.net/en-GB/kb/941772
    long active = LinuxParser::ActiveJiffies();
    long total = LinuxParser::Jiffies();

    float delta_active = active - prevActive_;
    float delta_total = total - prevTotal_;

    float utilization = (delta_active)*1.0 / delta_total;
    // LinuxParser 
    return utilization; 
}
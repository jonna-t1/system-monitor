#include <string>
#include <cmath>
#include <stdexcept>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
int count_digit(int number) {
   return int(log10(number) + 1);
}

string Format::ElapsedTime(long seconds[[maybe_unused]]) { 
    int minutes, hours;
    hours = seconds / 3600;
    seconds = seconds % 3600;
    minutes = seconds / 60;
    seconds = seconds % 60;

    if (count_digit(seconds) > 2 || seconds > 60){
        throw std::invalid_argument( "received seconds > 60" );
    }
    if (count_digit(minutes) > 2 || minutes > 60){
        throw std::invalid_argument( "received minutes > 60" );
    }
    
    return std::to_string(hours) + "h, " + 
            std::to_string(minutes) + "m, " +
            std::to_string(seconds) + "s";
    // return string(); 
}
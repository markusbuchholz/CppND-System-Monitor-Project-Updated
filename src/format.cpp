#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

/*
string Format::ElapsedTime(long seconds) {
    long hours;
    long minutes;
    long sec;
    string hours_string, minutes_string, sec_string; 

    hours = seconds/3600;
    minutes = (seconds%3600)/60;
    sec = seconds - hours*3600 - minutes*60;
    hours_string = std::to_string(hours);
    minutes_string = std::to_string(minutes);
    sec_string = std::to_string(sec);

    return (hours_string + ":" + minutes_string + ":" + sec_string); }

    */

   string Format::ElapsedTime(long seconds) { 
    long minutes = seconds / 60;
    long hours = minutes / 60;
    long lseconds = int(seconds%60);
    minutes = int(minutes%60);
    string result = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(lseconds);
    return result;
}
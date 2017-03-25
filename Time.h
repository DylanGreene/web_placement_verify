// Class header for a Time class which gets the current UTC time

#ifndef TIME_H
#define TIME_H

#include <ctime> // std::gmtime
#include <chrono> // std::chrono
#include <iostream> // std::cout
#include <string> // std::string
#include <sstream> // std::ostringstream
#include <iomanip> // std::setfill and std::setw

class Time{
public:
    // Constructor sets initial time
    Time();

    // Get functions all return respective time aspect in UTC
    int getMilliSecond();

    // Function to return a string to test output
    // Format: YYYY:MM:DD HH:MM:SS:MMM
    std::string timeString();

private:
    // Updates the time
    void updateTime();

    // Stores the milliseconds of the current second
    std::chrono::milliseconds ms;

    // Stores all of the other time and date data
    struct std::tm tm;
};

#endif

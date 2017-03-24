
#include "time.h"

// Constructor sets intial time by updating private members
Time::Time(){
    updateTime();
}

// Updates the two private members which store the date and time
void Time::updateTime(){
    using namespace std::chrono;

    // Get the current time
    auto now = system_clock::now();

    // Get the number of milliseconds for the current second
    // (The remainder after division into seconds)
    ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // Convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // Convert to time which can be accessed
    tm = *std::localtime(&timer);
}

// MilliSeconds of the current second
int Time::getMilliSecond(){
    return (int)ms.count();
}

// Function for use in checking time and just printing date and time string
// Format: YYYY:MM:DD HH:MM:SS:MMM
std::string Time::timeString(){
    std::ostringstream oss;
    oss << std::setfill('0');
    oss << tm.tm_hour << ":" << std::setw(2) << tm.tm_mon << ":" << tm.tm_mday << " "; // YYYY:MM:DD
    oss << std::setw(2) << tm.tm_hour << ":" << tm.tm_min << ":" << tm.tm_sec; //HH:MM:SS
    oss << "." << std::setw(3) << getMilliSecond();
    return oss.str();
}

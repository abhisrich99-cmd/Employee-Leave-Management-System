
#include "../include/Leave.h"

#include <ctime>
#include <iomanip>
#include <sstream>

Leave::Leave(int leaveId,
             const std::string& startDate,
             const std::string& endDate,
             const std::string& reason)
    : leaveId(leaveId),
      startDate(startDate),
      endDate(endDate),
      reason(reason) {
}

int Leave::calculateDays() const {
    std::tm start = {}; //std::tm creates containers for storing values in y-m-d form
    std::tm end = {};

    std::istringstream startInput(startDate); //istringstream takes input from already 
    std::istringstream endInput(endDate);     //stored string instead of keyboard

    startInput >> std::get_time(&start, "%Y-%m-%d"); //get_time takes string values in
    endInput >> std::get_time(&end, "%Y-%m-%d");     //year month date format

    if (startInput.fail() || endInput.fail()) {
        return 0;
    }

    start.tm_isdst = -1;
    end.tm_isdst = -1;

    std::time_t startTime = std::mktime(&start); //Converts into time format for taking 
    std::time_t endTime = std::mktime(&end);     //difference

    if (startTime == static_cast<std::time_t>(-1) || //Checks if time conversion fails
        endTime == static_cast<std::time_t>(-1)) {
        return 0;
    }

    double seconds = std::difftime(endTime, startTime);  //Gives difference in times.
    int days = static_cast<int>(seconds / (60 * 60 * 24));

    return days + 1;
}

int Leave::getLeaveId() const {
    return leaveId;
}

std::string Leave::getStartDate() const {
    return startDate;
}

std::string Leave::getEndDate() const {
    return endDate;
}

std::string Leave::getReason() const {
    return reason;
}

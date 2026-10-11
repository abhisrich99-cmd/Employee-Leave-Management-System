#include "../include/Leave.h"

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
    return 1;  //This is temporarily returning 1, because, we havn't written the days 
               //calculation logic yet. We'll implement the proper calculation later
               //when we handle date validation.
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
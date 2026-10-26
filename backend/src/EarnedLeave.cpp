
#include "../include/EarnedLeave.h"
#include <iostream>

EarnedLeave::EarnedLeave(int leaveId,
                         const std::string& startDate,
                         const std::string& endDate,
                         const std::string& reason)
    : Leave(leaveId, startDate, endDate, reason) {}

std::string EarnedLeave::getLeaveType() const {
    return "Earned Leave";
}

int EarnedLeave::getMaximumDays() const {
    return 15;
}
void EarnedLeave::display() const {
    std::cout << "Leave Type: " << getLeaveType() << '\n';
    std::cout << "Start Date: " << startDate << '\n';
    std::cout << "End Date: " << endDate << '\n';
    std::cout << "Reason: " << reason << '\n';
}
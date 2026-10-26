
#include "../include/SickLeave.h"
#include <iostream>

SickLeave::SickLeave(int leaveId,
                     const std::string& startDate,
                     const std::string& endDate,
                     const std::string& reason)
    : Leave(leaveId, startDate, endDate, reason) {}

std::string SickLeave::getLeaveType() const {
    return "Sick Leave";
}

void SickLeave::display() const {
    std::cout << "Leave Type: " << getLeaveType() << '\n';
    std::cout << "Start Date: " << startDate << '\n';
    std::cout << "End Date: " << endDate << '\n';
    std::cout << "Reason: " << reason << '\n';
}

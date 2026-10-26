#include "../include/CasualLeave.h"
#include <iostream>

CasualLeave::CasualLeave(int leaveId,
                         const std::string& startDate,
                         const std::string& endDate,
                         const std::string& reason)
    : Leave(leaveId, startDate, endDate, reason) {
}

std::string CasualLeave::getLeaveType() const {
    return "Casual Leave";
}

int CasualLeave::getMaximumDays() const {
    return 3;
}
void CasualLeave::display() const {
    std::cout << "Leave ID: " << leaveId << '\n';
    std::cout << "Leave Type: " << getLeaveType() << '\n';
    std::cout << "Start Date: " << startDate << '\n';
    std::cout << "End Date: " << endDate << '\n';
    std::cout << "Reason: " << reason << '\n';
}
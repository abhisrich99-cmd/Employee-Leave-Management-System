
#include "../include/LeaveBalance.h"

#include <iostream>

LeaveBalance::LeaveBalance(int casual, int sick, int earned)
    : casualLeave(casual),
      sickLeave(sick),
      earnedLeave(earned) {
}

int LeaveBalance::getBalance(const std::string& leaveType) const {
    if (leaveType == "Casual Leave") {
        return casualLeave;
    }

    if (leaveType == "Sick Leave") {
        return sickLeave;
    }

    if (leaveType == "Earned Leave") {
        return earnedLeave;
    }

    return -1;
}

bool LeaveBalance::deductLeave(
    const std::string& leaveType, int days) {

    if (days <= 0) {
        return false;
    }

    int* balance = nullptr;

    if (leaveType == "Casual Leave") {
        balance = &casualLeave;
    } else if (leaveType == "Sick Leave") {
        balance = &sickLeave;
    } else if (leaveType == "Earned Leave") {
        balance = &earnedLeave;
    } else {
        return false;
    }

    if (*balance < days) {
        return false;
    }

    *balance -= days;
    return true;
}

void LeaveBalance::displayBalance() const {
    std::cout << "Casual Leave: " << casualLeave << " days\n";
    std::cout << "Sick Leave: " << sickLeave << " days\n";
    std::cout << "Earned Leave: " << earnedLeave << " days\n";
}

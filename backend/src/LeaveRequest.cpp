
#include "../include/LeaveRequest.h"
#include <iostream>

LeaveRequest::LeaveRequest(
    long long requestId,
    const std::string& employeeId,
    const std::string& leaveType,
    const std::string& startDate,
    const std::string& endDate,
    int duration,
    const std::string& reason)
    : requestId(requestId),
      employeeId(employeeId),
      leaveType(leaveType),
      startDate(startDate),
      endDate(endDate),
      duration(duration),
      reason(reason),
      status("PENDING") {
}

long long LeaveRequest::getRequestId() const {
    return requestId;
}

std::string LeaveRequest::getEmployeeId() const {
    return employeeId;
}

std::string LeaveRequest::getLeaveType() const {
    return leaveType;
}

int LeaveRequest::getDuration() const {
    return duration;
}

std::string LeaveRequest::getStatus() const {
    return status;
}

bool LeaveRequest::approve() {
    if (status != "PENDING") {
        return false;
    }

    status = "APPROVED";
    return true;
}

bool LeaveRequest::reject() {
    if (status != "PENDING") {
        return false;
    }

    status = "REJECTED";
    return true;
}

void LeaveRequest::display() const {
    std::cout << "Request ID: " << requestId << '\n';
    std::cout << "Employee ID: " << employeeId << '\n';
    std::cout << "Leave Type: " << leaveType << '\n';
    std::cout << "Start Date: " << startDate << '\n';
    std::cout << "End Date: " << endDate << '\n';
    std::cout << "Duration: " << duration << " day(s)\n";
    std::cout << "Reason: " << reason << '\n';
    std::cout << "Status: " << status << '\n';
}

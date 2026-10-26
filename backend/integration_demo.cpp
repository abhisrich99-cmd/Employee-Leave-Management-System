
#include "include/CasualLeave.h"
#include "include/SickLeave.h"
#include "include/EarnedLeave.h"

#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <chrono>
#include <memory>

bool isValidDate(const std::string& date) {
    if (date.size() != 10 ||
        date[4] != '-' ||
        date[7] != '-') {
        return false;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            continue;
        }

        if (!std::isdigit(
                static_cast<unsigned char>(date[i]))) {
            return false;
        }
    }

    int year = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    if (year < 1 || month < 1 || month > 12) {
        return false;
    }

    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    bool isLeapYear =
        (year % 400 == 0) ||
        (year % 4 == 0 && year % 100 != 0);

    if (isLeapYear) {
        daysInMonth[1] = 29;
    }

    return day >= 1 && day <= daysInMonth[month - 1];
}

int main(int argc, char* argv[]) {
    // Arguments:
    // 1: Employee ID
    // 2: Leave type
    // 3: Start date
    // 4: End date
    // 5: Reason

    if (argc != 6) {
        std::cout << "Error: Missing leave details.\n";
        return 1;
    }

    std::string employeeId = argv[1];
    std::string leaveType = argv[2];
    std::string startDate = argv[3];
    std::string endDate = argv[4];
    std::string reason = argv[5];

    if (employeeId.empty() || leaveType.empty() ||
        startDate.empty() || endDate.empty() ||
        reason.empty()) {
        std::cout << "Error: All fields are required.\n";
        return 1;
    }

    if (reason.find(',') != std::string::npos) {
        std::cout << "Error: Please do not use commas in the reason.\n";
        return 1;
    }

    if (!isValidDate(startDate)) {
        std::cout << "Error: Invalid start date. Use YYYY-MM-DD format.\n";
        return 1;
    }

    if (!isValidDate(endDate)) {
        std::cout << "Error: Invalid end date. Use YYYY-MM-DD format.\n";
        return 1;
    }

    if (startDate > endDate) {
        std::cout << "Error: Start date must not be after start date.\n";
        return 1;
    }

    long long requestId =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

    int leaveId = static_cast<int>(requestId % 2147483647);

    std::unique_ptr<Leave> leave;

    if (leaveType == "Casual Leave") {
        leave = std::make_unique<CasualLeave>(
            leaveId, startDate, endDate, reason);
    } else if (leaveType == "Sick Leave") {
        leave = std::make_unique<SickLeave>(
            leaveId, startDate, endDate, reason);
    } else if (leaveType == "Earned Leave") {
        leave = std::make_unique<EarnedLeave>(
            leaveId, startDate, endDate, reason);
    } else {
        std::cout << "Error: Invalid leave type.\n";
        return 1;
    }

    int duration = leave->calculateDays();

    std::ifstream checkFile("data/leave_requests.csv");
    bool needsHeader = !checkFile.good();
    checkFile.close();

    std::ofstream file(
        "data/leave_requests.csv",
        std::ios::app
    );

    if (!file) {
        std::cout << "Error: Could not open CSV storage.\n";
        return 1;
    }

    if (needsHeader) {
        file << "RequestID,EmployeeID,LeaveType,StartDate,"
                "EndDate,Duration,Reason,Status\n";
    }

    file << requestId << ','
         << employeeId << ','
         << leave->getLeaveType() << ','
         << startDate << ','
         << endDate << ','
         << duration << ','
         << reason << ",PENDING\n";

    file.close();

    if (!file) {
        std::cout << "Error: Could not save leave request.\n";
        return 1;
    }

    std::cout << "Leave request saved successfully!\n";
    std::cout << "Request ID: " << requestId << '\n';
    std::cout << "Employee ID: " << employeeId << '\n';
    std::cout << "Leave Type: " << leave->getLeaveType() << '\n';
    std::cout << "Duration: " << duration << " day(s)\n";
    std::cout << "Status: PENDING\n";

    return 0;
}

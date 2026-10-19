#include "include/CasualLeave.h"

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip> //This provides formatting tools that can help parse dates.
#include <sstream> //This lets us treat a string like an input stream, which we 
                   //can use to parse the date.
#include <cctype>

bool isValidDate(const std::string& date) {
    // Check the length and hyphen positions.
    if (date.size() != 10 ||
        date[4] != '-' ||
        date[7] != '-') {
        return false;
    }

    // Check that all other characters are digits.
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            continue;
        }

       if (!std::isdigit(date[i])) {
    return false;
        }
    }

    // Extract year, month, and day.
    int year = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    // Check the month.
    if (month < 1 || month > 12) {
        return false;
    }

    // Number of days in each month.
    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    // Check whether the year is a leap year.
    bool isLeapYear =
        (year % 400 == 0) ||
        (year % 4 == 0 && year % 100 != 0);

    if (isLeapYear) {
        daysInMonth[1] = 29;
    }

    // Check whether the day exists in that month.
    if (day < 1 || day > daysInMonth[month - 1]) {
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cout << "Error: Missing leave details.\n";
        return 1;
    }

    std::string employeeId = argv[1];
    std::string startDate = argv[2];
    std::string endDate = argv[3];
    std::string reason = argv[4];

    if (employeeId.empty() || startDate.empty() ||
        endDate.empty() || reason.empty()) {
        std::cout << "Error: All fields are required.\n";
        return 1;
    }

          
    if (!isValidDate(startDate)) {
        std::cout << "Error: Invalid start date. "
              << "Use YYYY-MM-DD format.\n";
        return 1;
    }

    if (!isValidDate(endDate)) {
        std::cout << "Error: Invalid end date. "
              << "Use YYYY-MM-DD format.\n";
        return 1;
    }


    if (startDate > endDate) {
        std::cout << "Error: Start date must not be after end date.\n";
        return 1;
    }

    std::ifstream checkFile("data/leave_requests.csv");
    bool needsHeader = !checkFile.good();
    checkFile.close();

    std::ofstream file("data/leave_requests.csv", std::ios::app);

    if (!file) {
        std::cout << "Error: Could not open CSV storage.\n";
        return 1;
    }

    if (needsHeader) {
        file << "RequestID,EmployeeID,LeaveType,StartDate,"
                "EndDate,Duration,Reason,Status\n";
    }

    long long requestId =
        static_cast<long long>(std::time(nullptr));

    CasualLeave leave( //This is casual leave object.
        static_cast<int>(requestId % 2147483647),
        startDate,
        endDate,
        reason
    );

    int duration = leave.calculateDays();

    // Keep this prototype's CSV fields simple:
    // avoid commas in the reason.
    if (reason.find(',') != std::string::npos) {
        std::cout << "Error: Please do not use commas in the reason.\n";
        return 1;
    }

    file << requestId << ','
         << employeeId << ','
         << leave.getLeaveType() << ','
         << startDate << ','
         << endDate << ','
         << duration << ","
         << reason << ",PENDING\n";

    file.close();

    std::cout << "Leave request saved successfully!\n";
    std::cout << "Request ID: " << requestId << '\n';
    std::cout << "Employee ID: " << employeeId << '\n';
    std::cout << "Leave Type: " << leave.getLeaveType() << '\n';
    std::cout << "Duration: " << duration << " days\n";
    std::cout << "Status: PENDING\n";

    return 0;
}
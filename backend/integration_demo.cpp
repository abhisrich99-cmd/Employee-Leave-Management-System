#include "include/CasualLeave.h"

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

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
                "EndDate,Reason,Status\n";
    }

    long long requestId =
        static_cast<long long>(std::time(nullptr));

    CasualLeave leave(
        static_cast<int>(requestId % 2147483647),
        startDate,
        endDate,
        reason
    );

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
         << reason << ",PENDING\n";

    file.close();

    std::cout << "Leave request saved successfully!\n";
    std::cout << "Request ID: " << requestId << '\n';
    std::cout << "Employee ID: " << employeeId << '\n';
    std::cout << "Leave Type: " << leave.getLeaveType() << '\n';
    std::cout << "Status: PENDING\n";

    return 0;
}
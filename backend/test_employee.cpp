
#include "include/Employee.h"
#include <iostream>

int main() {
    Employee employee(
        "E101",
        "Arun",
        "password123",
        "arun@example.com",
        "9876543210",
        "CSE"
    );

    std::cout << "=== Employee Profile ===\n";
    employee.displayProfile();

    std::cout << "\n=== Leave Balances ===\n";
    employee.displayLeaveBalance();

    std::cout << "\n=== Deducting Leave ===\n";

    if (employee.deductLeave("Casual Leave", 3)) {
        std::cout << "3 days of Casual Leave deducted.\n";
    } else {
        std::cout << "Insufficient leave balance.\n";
    }

    std::cout << "\nUpdated Casual Leave Balance: "
              << employee.getLeaveBalance("Casual Leave")
              << " days\n";

    std::cout << "\n=== Testing Insufficient Balance ===\n";

    if (employee.deductLeave("Sick Leave", 20)) {
        std::cout << "Leave deducted.\n";
    } else {
        std::cout << "Insufficient Sick Leave balance.\n";
    }

    return 0;
}

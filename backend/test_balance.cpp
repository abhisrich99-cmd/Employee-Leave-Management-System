
#include "include/LeaveBalance.h"
#include <iostream>

int main() {
    LeaveBalance balance;

    std::cout << "Initial balances:\n";
    balance.displayBalance();

    std::cout << "\nApproving 3 days of Casual Leave...\n";

    if (balance.deductLeave("Casual Leave", 3)) {
        std::cout << "Approved successfully.\n";
    } else {
        std::cout << "Insufficient balance.\n";
    }

    std::cout << "\nUpdated balances:\n";
    balance.displayBalance();

    std::cout << "\nTrying to deduct 20 days of Sick Leave...\n";

    if (balance.deductLeave("Sick Leave", 20)) {
        std::cout << "Approved successfully.\n";
    } else {
        std::cout << "Rejected: insufficient balance.\n";
    }

    return 0;
}

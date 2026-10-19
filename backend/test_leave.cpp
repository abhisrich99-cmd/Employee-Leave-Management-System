
#include "include/CasualLeave.h"
#include <iostream>

int main() {
    CasualLeave leave(
        1,
        "2026-10-20",
        "2026-10-24",
        "Family function"
    );

    std::cout << "Leave duration: "
              << leave.calculateDays()
              << " days\n";

    return 0;
}

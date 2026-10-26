#include "include/CasualLeave.h"
#include "include/SickLeave.h"
#include "include/EarnedLeave.h"

#include <iostream>
#include <memory>
#include <vector>

int main() {
    std::vector<std::unique_ptr<Leave>> leaves;

    leaves.push_back(
        std::make_unique<CasualLeave>(
            1, "2026-11-02", "2026-11-03", "Personal work"
        )
    );

    leaves.push_back(
        std::make_unique<SickLeave>(
            2, "2026-11-04", "2026-11-05", "Medical rest"
        )
    );

    leaves.push_back(
        std::make_unique<EarnedLeave>(
            3, "2026-11-06", "2026-11-08", "Vacation"
        )
    );

    for (const auto& leave : leaves) {
        std::cout << leave->getLeaveType()
                  << ": maximum "
                  << leave->getMaximumDays()
                  << " days per request\n";
    }

    return 0;
}

#include "include/LeaveRequest.h"
#include <iostream>

int main() {
    LeaveRequest request(
        1001,
        "E101",
        "Casual Leave",
        "2026-11-02",
        "2026-11-04",
        3,
        "Personal work"
    );

    request.display();

    std::cout << "\nApproving request...\n";

    if (request.approve()) {
        std::cout << "Request approved.\n";
    }

    std::cout << "Status: " << request.getStatus() << '\n';

    std::cout << "\nTrying to approve again...\n";

    if (!request.approve()) {
        std::cout << "Duplicate approval prevented.\n";
    }

    return 0;
}

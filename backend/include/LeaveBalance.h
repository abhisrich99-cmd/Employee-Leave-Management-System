
#ifndef LEAVE_BALANCE_H
#define LEAVE_BALANCE_H

#include <string>

class LeaveBalance {
private:
    int casualLeave;
    int sickLeave;
    int earnedLeave;

public:
    LeaveBalance(int casual = 12,
                 int sick = 10,
                 int earned = 15);

    int getBalance(const std::string& leaveType) const;

    bool deductLeave(const std::string& leaveType, int days);

    void displayBalance() const;
};

#endif

//What OOP concept does this demonstrate?
//- Encapsulation: the balance variables are private, so other classes cannot change them directly.
//- Constructor: initializes the employee's leave balances.
//- Public methods: provide controlled access to check and deduct leave.
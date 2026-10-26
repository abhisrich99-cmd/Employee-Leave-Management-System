
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include "User.h"
#include "LeaveBalance.h"

class Employee : public User {
private:
    std::string email;
    std::string phone;
    std::string department;
    LeaveBalance leaveBalance;

public:
    Employee(const std::string& userId,
             const std::string& name,
             const std::string& password,
             const std::string& email,
             const std::string& phone,
             const std::string& department);

    void displayProfile() const override;

    std::string getEmail() const;
    std::string getPhone() const;
    std::string getDepartment() const;

    int getLeaveBalance(const std::string& leaveType) const;
    bool deductLeave(const std::string& leaveType, int days);
    void displayLeaveBalance() const;
};

#endif

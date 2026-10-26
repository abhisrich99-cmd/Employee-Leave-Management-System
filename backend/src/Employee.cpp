
#include "../include/Employee.h"
#include <iostream>

Employee::Employee(const std::string& userId,
                   const std::string& name,
                   const std::string& password,
                   const std::string& email,
                   const std::string& phone,
                   const std::string& department)
    : User(userId, name, password),
      email(email),
      phone(phone),
      department(department),
      leaveBalance() {
}

void Employee::displayProfile() const {
    std::cout << "User ID: " << getUserId() << '\n';
    std::cout << "Name: " << getName() << '\n';
    std::cout << "Email: " << email << '\n';
    std::cout << "Phone: " << phone << '\n';
    std::cout << "Department: " << department << '\n';
}

std::string Employee::getEmail() const {
    return email;
}

std::string Employee::getPhone() const {
    return phone;
}

std::string Employee::getDepartment() const {
    return department;
}

int Employee::getLeaveBalance(const std::string& leaveType) const {
    return leaveBalance.getBalance(leaveType);
}

bool Employee::deductLeave(const std::string& leaveType, int days) {
    return leaveBalance.deductLeave(leaveType, days);
}

void Employee::displayLeaveBalance() const {
    leaveBalance.displayBalance();
}

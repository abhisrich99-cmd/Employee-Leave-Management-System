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
      department(department) {
}

void Employee::displayProfile() const {
    std::cout << "Employee ID: " << userId << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Phone: " << phone << std::endl;
    std::cout << "Department: " << department << std::endl;
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
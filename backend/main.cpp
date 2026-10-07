#include <iostream>
#include "include/Employee.h"
#include "include/Admin.h"

int main() {

    Employee employee(
        "E101",
        "Rahul",
        "abc123",
        "rahul@gmail.com",
        "9876543210",
        "CSE"
    );

    Admin admin(
        "A001",
        "Admin User",
        "admin123"
    );

    std::cout << "----- EMPLOYEE PROFILE -----" << std::endl;
    employee.displayProfile();

    std::cout << std::endl;

    std::cout << "----- ADMIN PROFILE -----" << std::endl;
    admin.displayProfile();

    std::cout << std::endl;

    std::cout << "Employee ID: "
              << employee.getUserId() << std::endl;

    std::cout << "Employee Name: "
              << employee.getName() << std::endl;

    std::cout << "Password Correct: "
              << employee.checkPassword("abc123") << std::endl;

    return 0;
}
#include "../include/Admin.h"
#include <iostream>

Admin::Admin(const std::string& userId,
             const std::string& name,
             const std::string& password)
    : User(userId, name, password) {
}

void Admin::displayProfile() const {
    std::cout << "Admin ID: " << userId << std::endl;
    std::cout << "Name: " << name << std::endl;
}
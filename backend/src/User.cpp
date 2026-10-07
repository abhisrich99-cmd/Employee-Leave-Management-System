#include "../include/User.h"

User::User(const std::string& userId, const std::string& name, const std::string& password)
    : userId(userId), name(name), password(password) {
        //userId(userId), name(name), password(password)
        //First userId is the class variable name and the second one is the name we get
        //from the constructor.
}

std::string User::getUserId() const {
    return userId;
}

std::string User::getName() const {
    return name;
}

bool User::checkPassword(const std::string& password) const {
    return this->password == password;
}
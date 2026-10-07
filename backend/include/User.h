#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    std::string userId;
    std::string name;
    std::string password;

public:
    User(const std::string& userId, const std::string& name, const std::string& password);

    virtual void displayProfile() const = 0; //Pure virtural functions force the derived
                                            // classes to declare the function.

    std::string getUserId() const;
    std::string getName() const;

    bool checkPassword(const std::string& password) const;

    virtual ~User() = default;
};

#endif
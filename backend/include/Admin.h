#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include "User.h"

class Admin : public User {
public:
    Admin(const std::string& userId,
          const std::string& name,
          const std::string& password);

    void displayProfile() const override;
};

#endif
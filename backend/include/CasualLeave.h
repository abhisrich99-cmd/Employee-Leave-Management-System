#ifndef CASUAL_LEAVE_H
#define CASUAL_LEAVE_H

#include "Leave.h"

class CasualLeave : public Leave {
public:
    CasualLeave(int leaveId,
                const std::string& startDate,
                const std::string& endDate,
                const std::string& reason);

    std::string getLeaveType() const override;
    void display() const override;
};

#endif
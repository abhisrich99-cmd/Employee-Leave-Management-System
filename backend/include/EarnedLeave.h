
#ifndef EARNED_LEAVE_H
#define EARNED_LEAVE_H

#include "Leave.h"

class EarnedLeave : public Leave {
public:
    EarnedLeave(int leaveId,
                const std::string& startDate,
                const std::string& endDate,
                const std::string& reason);

    std::string getLeaveType() const override;
    int getMaximumDays() const override;
    void display() const override;
};

#endif

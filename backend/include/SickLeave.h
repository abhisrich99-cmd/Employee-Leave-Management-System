
#ifndef SICK_LEAVE_H
#define SICK_LEAVE_H

#include "Leave.h"

class SickLeave : public Leave {
public:
    SickLeave(int leaveId,
              const std::string& startDate,
              const std::string& endDate,
              const std::string& reason);

    std::string getLeaveType() const override;
    int getMaximumDays() const override;
    void display() const override;
};

#endif

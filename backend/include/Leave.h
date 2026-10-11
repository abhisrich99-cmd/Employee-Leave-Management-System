#ifndef LEAVE_H
#define LEAVE_H

#include <string>

class Leave {
protected:
    int leaveId;
    std::string startDate;
    std::string endDate;
    std::string reason;

public:
    Leave(int leaveId,
          const std::string& startDate,
          const std::string& endDate,
          const std::string& reason);

    virtual int calculateDays() const;
    virtual std::string getLeaveType() const = 0;
    virtual void display() const = 0;

    int getLeaveId() const;
    std::string getStartDate() const;
    std::string getEndDate() const;
    std::string getReason() const;

    virtual ~Leave() = default; //Reason for virtual:
                                //Later we may have: Leave* leave = new SickLeave(...);
                                //Here, the pointer is of type Leave*, 
                                //but the actual object is a SickLeave.
                                //A virtual destructor makes sure the correct 
                                //destruction process happens when we delete the object 
                                //through the base-class pointer.
};

#endif
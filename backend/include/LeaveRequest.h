
#ifndef LEAVE_REQUEST_H
#define LEAVE_REQUEST_H

#include <string>

class LeaveRequest {
private:
    long long requestId;
    std::string employeeId;
    std::string leaveType;
    std::string startDate;
    std::string endDate;
    int duration;
    std::string reason;
    std::string status;

public:
    LeaveRequest(long long requestId,
                 const std::string& employeeId,
                 const std::string& leaveType,
                 const std::string& startDate,
                 const std::string& endDate,
                 int duration,
                 const std::string& reason);

    long long getRequestId() const;
    std::string getEmployeeId() const;
    std::string getLeaveType() const;
    int getDuration() const;
    std::string getStatus() const;

    bool approve();
    bool reject();
    void display() const;
};

#endif

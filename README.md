# LeaveFlow – Employee Leave Management System

## 1. Project Overview

LeaveFlow is a web-based Employee Leave Management System designed to simplify and organize the employee leave application and approval process.

The application provides separate interfaces for employees and administrators. Employees can log in, view their leave balances, submit leave requests, and track their request statuses. Administrators can manage employee accounts, review leave requests, and approve or reject applications.

The project follows a layered architecture that integrates a web frontend, a Python Flask application, a C++ backend, and local file-based storage.

### Technologies Used

- **Frontend:** HTML, CSS, JavaScript
- **Application Layer:** Python Flask
- **Backend:** C++
- **Data Storage:** JSON and CSV files
- **Version Control:** Git and GitHub

---

## 2. Problem Statement

Managing employee leave manually can be time-consuming and prone to errors. Employees need a convenient way to apply for leave, monitor their available leave balance, and track application status. Administrators need an organized way to manage employee accounts and process leave requests.

LeaveFlow addresses these requirements by providing a centralized application for submitting, validating, reviewing, and tracking employee leave requests.

---

## 3. Objectives

The main objectives of the project are:

- Provide login functionality for employees and administrators.
- Implement role-based access to application features.
- Allow employees to view their leave balances.
- Enable employees to submit leave applications.
- Validate leave dates and permitted leave duration.
- Allow employees to track the status of their requests.
- Enable administrators to create employee accounts.
- Allow administrators to review, approve, and reject leave requests.
- Update leave balances when requests are approved.
- Maintain employee information, leave balances, and request records using local files.
- Demonstrate Object-Oriented Programming concepts in C++.
- Integrate the frontend, Flask application, and C++ backend into an end-to-end system.

---

## 4. Key Features

### Employee Features

- Employee login
- Employee dashboard
- View available casual, sick, and earned leave balances
- Submit leave applications with dates and reasons
- View leave request history
- Track pending, approved, and rejected requests
- View updated leave balances after approval

### Administrator Features

- Administrator login
- Create employee accounts
- View submitted leave requests
- Filter requests by status
- Approve or reject pending leave applications
- Review employee leave requests
- View a summary of leave request statistics

### System Features

- Session-based login management
- Role-based access control
- Date and leave-duration validation
- Leave-type-specific maximum duration checks
- Leave request status management
- Automatic leave-balance deduction after approval, subject to sufficient balance
- Persistent JSON and CSV storage
- Integration between Python Flask and the compiled C++ executable
- Object-Oriented Programming using abstraction, inheritance, and polymorphism

---

## 5. System Architecture

LeaveFlow follows a layered architecture in which each component has a specific responsibility.

```text
+------------------------------------------+
|              WEB FRONTEND                |
|          HTML, CSS, JavaScript           |
|                                          |
|  Login | Employee Dashboard | Admin      |
|  Leave Application | Request Status      |
+---------------------+--------------------+
                      |
                      | HTTP Requests
                      v
+------------------------------------------+
|          APPLICATION LAYER               |
|              Python Flask                |
|                                          |
|  Authentication and Sessions             |
|  Role-Based Access Control               |
|  Request Handling and Status Updates     |
|  Employee and Balance Management         |
+---------------------+--------------------+
                      |
                      | Subprocess Execution
                      v
+------------------------------------------+
|              C++ BACKEND                 |
|                                          |
|  Leave Class and Derived Leave Types     |
|  Date and Duration Calculation           |
|  Maximum Leave Duration Validation       |
|  Request Processing                      |
+---------------------+--------------------+
                      |
                      | File I/O
                      v
+------------------------------------------+
|           LOCAL DATA STORAGE             |
|                                          |
|  users.json                              |
|  leave_balances.json                     |
|  leave_requests.csv                      |
+------------------------------------------+
```

### Component Responsibilities

**Frontend:** Provides the user interface for employees and administrators.

**Flask Application:** Handles login sessions, role-based access, application routes, employee management, request processing, and leave-balance updates.

**C++ Backend:** Processes leave requests, calculates leave duration, validates dates, and checks maximum permitted duration for each leave type.

**Local Storage:** Maintains employee account information, leave balances, and leave request records in JSON and CSV files.

---

## 6. Object-Oriented Programming Concepts

The C++ backend demonstrates important Object-Oriented Programming concepts.

- **Abstraction:** The abstract `Leave` class defines common operations for different leave types.
- **Inheritance:** `CasualLeave`, `SickLeave`, and `EarnedLeave` derive from the base `Leave` class.
- **Polymorphism:** Virtual functions allow different leave types to provide their own leave-type information and maximum duration.
- **Encapsulation:** Classes organize related data and operations into manageable units.

These concepts help keep the leave-validation logic modular and easier to maintain.

---

## 7. Data Storage

The application uses local JSON and CSV files for persistent storage.

| File | Purpose |
|---|---|
| `frontend/users.json` | Stores user account information used by Flask authentication |
| `data/leave_balances.json` | Stores employee leave balances |
| `data/leave_requests.csv` | Stores leave requests, dates, duration, reasons, and statuses |

The Flask application reads and updates the relevant files to manage accounts, balances, and request statuses. The C++ backend appends valid leave requests to the CSV file with an initial `PENDING` status.

---

## 8. Leave Request Workflow

1. The employee logs in to LeaveFlow.
2. The employee selects a leave type and enters the required dates and reason.
3. Flask receives the submitted application and invokes the C++ backend.
4. The C++ backend validates the dates, calculates the duration, and checks the maximum permitted duration.
5. If validation succeeds, the request is saved with a `PENDING` status.
6. The administrator reviews the pending request.
7. The administrator approves or rejects the request.
8. For an approved request, the application checks the available leave balance and deducts the requested days when sufficient balance is available.
9. The employee can view the updated request status and leave balance.

---

## 9. Testing and Results

The main application workflows were tested to verify the interaction between the frontend, Flask application, C++ backend, and local storage.

The testing covered:

- User login and role-based access
- Leave request submission
- Leave-date and duration validation
- Administrator approval and rejection
- Employee request history
- Leave-balance updates
- Employee account creation
- Request filtering and administrative summary statistics

In the demonstrated two-day sick-leave approval, the available sick-leave balance decreased from 10 days to 8 days.

---

## 10. Project Structure

```text
Employee-Leave-Management-System/
├── backend/
│   ├── include/
│   ├── src/
│   └── leave_backend.exe
├── frontend/
│   ├── templates/
│   ├── static/
│   ├── users.json
│   └── web_app.py
├── data/
│   ├── leave_balances.json
│   └── leave_requests.csv
├── main.tex
├── LeaveFlow_Final_Report.pdf
└── README.md
```

*Note: The structure above is illustrative. Keep the directory names and files in this section consistent with the actual repository.*

---

## 11. Running the Project

### Prerequisites

- Python 3
- Flask
- A C++ compiler such as GCC or MinGW
- Git (optional, for version control)

### Setup

1. Clone or download the project repository.
2. Open the project directory in a terminal.
3. Install Flask if it is not already installed:

   ```bash
   python -m pip install flask
   ```

4. Compile the C++ backend using the source files and include paths provided in the project.
5. Ensure the compiled executable is located at the path expected by the Flask application.
6. Run the Flask application:

   ```bash
   python frontend/web_app.py
   ```

7. Open the local URL printed in the terminal in your web browser.

Use the actual compiler command, executable location, and application URL configured in your project.

---

## 12. Future Enhancements

Possible future enhancements include:

- Integration with a relational database
- Email notifications for leave status updates
- Password hashing and stronger authentication controls
- Deployment to a production web server
- Detailed audit logs and reporting
- Automated testing and improved error handling

---

## 13. Conclusion

LeaveFlow demonstrates an end-to-end Employee Leave Management System that combines a web-based interface, Python Flask application, C++ business logic, and persistent local storage.

The project provides essential employee and administrator workflows while demonstrating Object-Oriented Programming concepts and integration between multiple technologies. It serves as an academic example of how software components can work together to make leave management more organized and efficient.

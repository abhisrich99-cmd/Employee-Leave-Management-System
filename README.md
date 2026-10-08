# Employee Leave Management System

## 1. Project Overview

The Employee Leave Management System is an end-to-end application designed to simplify and automate the process of managing employee leave.

The system provides separate functionalities for employees and administrators. Employees can view their profile, check leave balances, apply for leave, track leave requests, and cancel pending requests. Administrators can manage employee records and approve or reject leave applications.

The project follows a layered architecture consisting of:

- Python Tkinter frontend
- C++ backend
- CSV-based persistent storage

The C++ backend implements the core business logic using Object-Oriented Programming concepts.

---

## 2. Problem Statement

Managing employee leave manually can be time-consuming and prone to errors.

Employees need a convenient way to apply for leave, check their leave balance, and track the status of their requests. Administrators need a centralized system to manage employee records and review, approve, or reject leave applications.

This project provides a structured solution for managing the complete leave request process from application to approval/rejection and persistent storage.

---

## 3. Objectives

The main objectives of the project are:

- Provide employee and administrator login functionality.
- Allow employees to apply for different types of leave.
- Maintain and display employee leave balances.
- Allow employees to view their leave requests and history.
- Allow employees to cancel pending requests.
- Allow administrators to approve or reject leave requests.
- Allow administrators to manage employee records.
- Validate leave requests before submission.
- Prevent overlapping or invalid leave applications.
- Store employee and leave information persistently.
- Demonstrate Object-Oriented Programming concepts using C++.
- Provide a graphical frontend using Python Tkinter.
- Integrate the frontend, backend, and storage into one end-to-end system.

---

## 4. Key Features

### Employee Features

- Employee login
- View profile
- View leave balance
- Apply for leave
- Select leave type
- Enter leave dates and reason
- View pending leave requests
- View leave history
- Cancel pending leave requests

### Administrator Features

- Administrator login
- Add employees
- Remove employees
- Search employees
- View employee information
- View pending leave requests
- Approve leave requests
- Reject leave requests
- View leave statistics

### System Features

- Leave balance management
- Date validation
- Leave overlap validation
- Duplicate request validation
- Employee validation
- Exception handling
- Persistent file storage
- End-to-end frontend and backend integration

---

## 5. System Architecture

The system follows a layered architecture.

```text
+--------------------------------------+
|          Python Tkinter GUI          |
|              FRONTEND                |
+------------------+-------------------+
                   |
                   | Integration
                   v
+--------------------------------------+
|            C++ BACKEND               |
|                                      |
|  User / Employee / Admin             |
|  Leave / Leave Types                 |
|  LeaveRequest                        |
|  LeaveManager                        |
|  EmployeeManager                     |
|  Repository<T>                       |
|  Exception Handling                  |
+------------------+-------------------+
                   |
                   | File I/O
                   v
+--------------------------------------+
|          CSV Persistent Storage       |
|                                      |
|       employees.csv                  |
|       leave_requests.csv             |
+--------------------------------------+
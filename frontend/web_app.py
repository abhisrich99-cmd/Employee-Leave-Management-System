
import csv
import json
import os
import secrets
import subprocess
from functools import wraps
from pathlib import Path

from flask import (
    Flask,
    send_from_directory,
    request,
    jsonify,
    session,
    redirect,
)

app = Flask(__name__)

# Set SECRET_KEY in your environment before running the app.
app.secret_key = "leaveflow-local-development-secret-change-later"

WEB_DIR = Path(__file__).resolve().parent / "web"
PROJECT_DIR = Path(__file__).resolve().parent.parent
BACKEND_EXE = PROJECT_DIR / "backend" / "leave_backend.exe"
CSV_FILE = PROJECT_DIR / "data" / "leave_requests.csv"
BALANCES_FILE = PROJECT_DIR / "data" / "leave_balances.json"
USERS_FILE = Path(__file__).resolve().parent / "users.json"


# ---------------- ROLE CHECKING ----------------

def require_role(required_role):
    def decorator(view_function):
        @wraps(view_function)
        def wrapper(*args, **kwargs):
            if "username" not in session:
                return redirect("/login-page")

            if session.get("role") != required_role:
                return "Access denied.", 403

            return view_function(*args, **kwargs)

        return wrapper

    return decorator


# ---------------- LOGIN ----------------

@app.route("/")
def home():
    if "username" not in session:
        return redirect("/login-page")

    if session.get("role") == "admin":
        return redirect("/admin")

    if session.get("role") == "employee":
        return redirect("/employee")

    session.clear()
    return redirect("/login-page")


@app.route("/login-page")
def login_page():
    return send_from_directory(WEB_DIR, "login.html")


@app.route("/login", methods=["POST"])
def login():
    data = request.get_json(silent=True) or {}

    username = data.get("username", "").strip()
    password = data.get("password", "")
    role = data.get("role", "")

    try:
        with open(USERS_FILE, "r", encoding="utf-8") as file:
            users = json.load(file)
    except (OSError, json.JSONDecodeError):
        app.logger.exception("Could not load users.json")
        return jsonify({"message": "Login configuration error."}), 500

    user = users.get(username)

    if (
        not isinstance(user, dict)
        or not isinstance(password, str)
        or not secrets.compare_digest(
            str(user.get("password", "")), password
        )
        or user.get("role") != role
    ):
        return jsonify({
            "message": "Invalid username, password, or role."
        }), 401

    session.clear()
    session["username"] = username
    session["role"] = role

    destination = "/admin" if role == "admin" else "/employee"

    return jsonify({"redirect": destination})


@app.route("/logout", methods=["GET", "POST"])
def logout():
    session.clear()
    return redirect("/login-page")


# ---------------- ADD EMPLOYEE (ADMIN ONLY) ----------------

@app.route("/add-employee", methods=["POST"])
@require_role("admin")
def add_employee():
    data = request.get_json(silent=True) or {}

    name = data.get("name", "").strip()
    password = data.get("password", "")

    if not name or not isinstance(password, str) or not password.strip():
        return jsonify({"message": "Name and password are required."}), 400

    if len(name) > 100 or len(password) > 128:
        return jsonify({"message": "Name or password is too long."}), 400

    try:
        with open(USERS_FILE, "r", encoding="utf-8") as file:
            users = json.load(file)

        with open(BALANCES_FILE, "r", encoding="utf-8") as file:
            balances = json.load(file)

        if not isinstance(users, dict) or not isinstance(balances, dict):
            return jsonify({"message": "Invalid user or balance data."}), 500

        # Find the next available employee ID.
        employee_numbers = [
            int(username[1:])
            for username in users
            if isinstance(username, str)
            and len(username) > 1
            and username.startswith("E")
            and username[1:].isdigit()
        ]

        next_number = max(employee_numbers, default=100) + 1
        employee_id = f"E{next_number}"

        # Avoid overwriting an existing account.
        while employee_id in users:
            next_number += 1
            employee_id = f"E{next_number}"

        # Add the account and its initial leave balances.
        users[employee_id] = {
            "name": name,
            "password": password,
            "role": "employee"
        }

        balances[employee_id] = {
            "Casual Leave": 12,
            "Sick Leave": 10,
            "Earned Leave": 15
        }

        users_temp = USERS_FILE.with_suffix(".tmp")
        balances_temp = BALANCES_FILE.with_suffix(".tmp")

        try:
            with open(users_temp, "w", encoding="utf-8") as file:
                json.dump(users, file, indent=4)
                file.write("\n")

            with open(balances_temp, "w", encoding="utf-8") as file:
                json.dump(balances, file, indent=4)
                file.write("\n")

            # Save both files after writing the temporary files.
            users_temp.replace(USERS_FILE)
            balances_temp.replace(BALANCES_FILE)

        finally:
            for temp_file in (users_temp, balances_temp):
                if temp_file.exists():
                    temp_file.unlink()

        return jsonify({
            "message": "Employee created successfully.",
            "employeeId": employee_id
        }), 201

    except (OSError, json.JSONDecodeError):
        app.logger.exception("Could not create employee")
        return jsonify({"message": "Could not save the employee."}), 500


# ---------------- EMPLOYEE AND ADMIN PAGES ----------------

@app.route("/employee")
@require_role("employee")
def employee_page():
    return send_from_directory(WEB_DIR, "employee.html")


@app.route("/admin")
@require_role("admin")
def admin_page():
    return send_from_directory(WEB_DIR, "admin.html")


# ---------------- STATIC FILES ----------------

@app.route("/<path:filename>")
def static_files(filename):
    # Do not serve private configuration or data files.
    if filename in {"users.json", "web_app.py", "app.py"}:
        return "Not found.", 404

    return send_from_directory(WEB_DIR, filename)


# ---------------- APPLY FOR LEAVE ----------------

@app.route("/apply", methods=["POST"])
@require_role("employee")
def apply_leave():
    employee_id = session["username"]
    leave_type = request.form.get("leaveType", "").strip()
    start_date = request.form.get("startDate", "").strip()
    end_date = request.form.get("endDate", "").strip()
    reason = request.form.get("reason", "").strip()

    if not all([leave_type, start_date, end_date, reason]):
        return "Please complete all fields.", 400

    if leave_type not in [
        "Casual Leave",
        "Sick Leave",
        "Earned Leave",
    ]:
        return "Invalid leave type.", 400

    if "," in reason:
        return "Please do not use commas in the reason.", 400

    if not BACKEND_EXE.exists():
        return "C++ backend executable not found.", 500

    try:
        result = subprocess.run(
            [
                str(BACKEND_EXE),
                employee_id,
                leave_type,
                start_date,
                end_date,
                reason,
            ],
            cwd=str(PROJECT_DIR),
            capture_output=True,
            text=True,
            timeout=10,
        )

        if result.returncode == 0:
            return result.stdout, 200

        return result.stdout or result.stderr or "Request failed.", 400

    except subprocess.TimeoutExpired:
        return "The backend took too long to respond.", 504
    except OSError:
        app.logger.exception("Could not run C++ backend")
        return "Could not run C++ backend.", 500


# ---------------- VIEW ALL REQUESTS (ADMIN ONLY) ----------------

@app.route("/requests", methods=["GET"])
@require_role("admin")
def get_requests():
    if not CSV_FILE.exists():
        return jsonify([])

    try:
        with open(CSV_FILE, "r", newline="", encoding="utf-8") as file:
            reader = csv.DictReader(file)
            requests_list = list(reader)

        return jsonify(requests_list)

    except (OSError, csv.Error):
        app.logger.exception("Could not read leave requests")
        return "Could not read leave requests.", 500


# ---------------- VIEW OWN REQUESTS (EMPLOYEE ONLY) ----------------

@app.route("/my-requests", methods=["GET"])
@require_role("employee")
def get_my_requests():
    if not CSV_FILE.exists():
        return jsonify([])

    try:
        with open(CSV_FILE, "r", newline="", encoding="utf-8") as file:
            reader = csv.DictReader(file)
            requests_list = [
                row for row in reader
                if row.get("EmployeeID") == session["username"]
            ]

        return jsonify(requests_list)

    except (OSError, csv.Error):
        app.logger.exception("Could not read employee requests")
        return "Could not read leave requests.", 500


# ---------------- APPROVE OR REJECT (ADMIN ONLY) ----------------


@app.route("/update-status", methods=["POST"])
@require_role("admin")
def update_status():
    request_id = request.form.get("requestId", "").strip()
    new_status = request.form.get("status", "").strip()

    if not request_id:
        return "Request ID is required.", 400

    if new_status not in ["APPROVED", "REJECTED"]:
        return "Invalid status.", 400

    if not CSV_FILE.exists():
        return "Leave requests file not found.", 404

    if not BALANCES_FILE.exists():
        return "Leave balances file not found.", 500

    temp_file = CSV_FILE.with_suffix(".tmp")
    balances_temp_file = BALANCES_FILE.with_suffix(".tmp")

    try:
        # Read leave requests
        with open(CSV_FILE, "r", newline="", encoding="utf-8") as file:
            reader = csv.DictReader(file)
            fieldnames = reader.fieldnames
            rows = list(reader)

        if (
            not fieldnames
            or "RequestID" not in fieldnames
            or "Status" not in fieldnames
            or "EmployeeID" not in fieldnames
            or "LeaveType" not in fieldnames
            or "Duration" not in fieldnames
        ):
            return "Invalid CSV format.", 500

        # Find the pending request
        target = None

        for row in rows:
            if row.get("RequestID") == request_id:
                target = row
                break

        if target is None:
            return "Leave request not found.", 404

        if target.get("Status") != "PENDING":
            return "This request has already been processed.", 400

        employee_id = target.get("EmployeeID", "")
        leave_type = target.get("LeaveType", "")

        try:
            days = int(target.get("Duration", "0"))
        except (ValueError, TypeError):
            return "Invalid leave duration.", 500

        if days <= 0:
            return "Invalid leave duration.", 400

        # Read employee balances
        with open(BALANCES_FILE, "r", encoding="utf-8") as file:
            balances = json.load(file)

        if not isinstance(balances, dict):
            return "Invalid leave balances file.", 500

        # Only approvals deduct leave
        if new_status == "APPROVED":
            employee_balances = balances.get(employee_id)

            if not isinstance(employee_balances, dict):
                return "No leave balances found for this employee.", 400

            if leave_type not in employee_balances:
                return "Invalid leave type in request.", 400

            current_balance = employee_balances[leave_type]

            if (
                not isinstance(current_balance, int)
                or isinstance(current_balance, bool)
                or current_balance < 0
            ):
                return "Invalid employee leave balance.", 500

            if current_balance < days:
                return (
                    f"Insufficient {leave_type} balance. "
                    f"Available: {current_balance} day(s); "
                    f"requested: {days} day(s)."
                ), 400

            # Deduct from the employee's balance
            employee_balances[leave_type] = current_balance - days

        # Update request status in memory
        target["Status"] = new_status

        # Write both temporary files before replacing either original
        with open(balances_temp_file, "w", encoding="utf-8") as file:
            json.dump(balances, file, indent=4)
            file.write("\n")

        with open(temp_file, "w", newline="", encoding="utf-8") as file:
            writer = csv.DictWriter(file, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(rows)

        # Save balances first, then update the request status
        if new_status == "APPROVED":
            balances_temp_file.replace(BALANCES_FILE)

        temp_file.replace(CSV_FILE)

        return f"Leave request {new_status.lower()} successfully.", 200

    except (OSError, csv.Error, json.JSONDecodeError):
        app.logger.exception("Could not update leave request or balances")
        return "Could not update leave request or balances.", 500

    finally:
        for path in (temp_file, balances_temp_file):
            if path.exists():
                try:
                    path.unlink()
                except OSError:
                    pass


@app.route("/my-balances", methods=["GET"])
@require_role("employee")
def get_my_balances():
    employee_id = session["username"]

    if not BALANCES_FILE.exists():
        return jsonify({
            "message": "Leave balances not found."
        }), 404

    try:
        with open(BALANCES_FILE, "r", encoding="utf-8") as file:
            balances = json.load(file)

        employee_balances = balances.get(employee_id)

        if employee_balances is None:
            return jsonify({
                "message": "No balances found for this employee."
            }), 404

        return jsonify(employee_balances)

    except (OSError, json.JSONDecodeError):
        app.logger.exception("Could not read leave balances")
        return jsonify({
            "message": "Could not read leave balances."
        }), 500


if __name__ == "__main__":
    app.run(debug=True)

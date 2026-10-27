import csv
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
FRONTEND_DIR = ROOT / "frontend"
DATA_DIR = ROOT / "data"

USERS_FILE = FRONTEND_DIR / "users.json"
BALANCES_FILE = DATA_DIR / "leave_balances.json"
REQUESTS_FILE = DATA_DIR / "leave_requests.csv"


def write_json_if_missing(path, data):
    if path.exists():
        print(f"Keeping existing file: {path.relative_to(ROOT)}")
        return

    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8") as file:
        json.dump(data, file, indent=4)
        file.write("\n")
    print(f"Created: {path.relative_to(ROOT)}")


def main():
    FRONTEND_DIR.mkdir(parents=True, exist_ok=True)
    DATA_DIR.mkdir(parents=True, exist_ok=True)

    # Demo credentials for local coursework only.
    users = {
        f"E{i}": {
            "name": f"Employee {i}",
            "password": f"Emp@{i}",
            "role": "employee"
        }
        for i in range(101, 111)
    }
    users["admin"] = {
        "name": "Administrator",
        "password": "admin@123",
        "role": "admin"
    }

    balances = {
        f"E{i}": {
            "Casual Leave": 12,
            "Sick Leave": 10,
            "Earned Leave": 15
        }
        for i in range(101, 111)
    }

    write_json_if_missing(USERS_FILE, users)
    write_json_if_missing(BALANCES_FILE, balances)

    if REQUESTS_FILE.exists():
        print(f"Keeping existing file: {REQUESTS_FILE.relative_to(ROOT)}")
    else:
        with REQUESTS_FILE.open("w", newline="", encoding="utf-8") as file:
            writer = csv.writer(file)
            writer.writerow([
                "RequestID", "EmployeeID", "LeaveType", "StartDate",
                "EndDate", "Duration", "Reason", "Status"
            ])
        print(f"Created: {REQUESTS_FILE.relative_to(ROOT)}")

    print("\nSetup complete.")
    print("Demo admin login: admin / admin@123")
    print("Demo employee logins: E101-E110; passwords are Emp@101-Emp@110")
    print("These are demo credentials. Change them before any public deployment.")


if __name__ == "__main__":
    main()

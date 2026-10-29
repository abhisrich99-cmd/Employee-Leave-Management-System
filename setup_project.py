
import subprocess
import csv
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
FRONTEND_DIR = ROOT / "frontend"
DATA_DIR = ROOT / "data"
BACKEND_DIR = ROOT / "backend"

USERS_FILE = FRONTEND_DIR / "users.json"
BALANCES_FILE = DATA_DIR / "leave_balances.json"
REQUESTS_FILE = DATA_DIR / "leave_requests.csv"
BACKEND_EXE = BACKEND_DIR / "leave_backend.exe"


def write_json_if_missing(path, data):
    if path.exists():
        print(f"Keeping existing file: {path.relative_to(ROOT)}")
        return

    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8") as file:
        json.dump(data, file, indent=4)
        file.write("\n")

    print(f"Created: {path.relative_to(ROOT)}")


def setup_data():
    FRONTEND_DIR.mkdir(parents=True, exist_ok=True)
    DATA_DIR.mkdir(parents=True, exist_ok=True)

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
        with REQUESTS_FILE.open(
            "w", newline="", encoding="utf-8"
        ) as file:
            writer = csv.writer(file)
            writer.writerow([
                "RequestID", "EmployeeID", "LeaveType",
                "StartDate", "EndDate", "Duration",
                "Reason", "Status"
            ])

        print(f"Created: {REQUESTS_FILE.relative_to(ROOT)}")


def setup_backend():
    if BACKEND_EXE.exists():
        print("C++ backend executable already exists.")
        return True

    print("C++ backend executable not found. Compiling...")

    source_files = [
        BACKEND_DIR / "integration_demo.cpp",
        BACKEND_DIR / "src" / "Leave.cpp",
        BACKEND_DIR / "src" / "CasualLeave.cpp",
        BACKEND_DIR / "src" / "SickLeave.cpp",
        BACKEND_DIR / "src" / "EarnedLeave.cpp",
    ]

    missing_files = [str(path) for path in source_files if not path.exists()]

    if missing_files:
        print("Cannot compile: required source files are missing:")
        for path in missing_files:
            print(f"  {path}")
        return False

    try:
        subprocess.run(
            [
                "g++",
                "-std=c++17",
                *[str(path) for path in source_files],
                "-o",
                str(BACKEND_EXE),
            ],
            cwd=ROOT,
            check=True,
        )
        print("C++ backend compiled successfully.")
        return True

    except FileNotFoundError:
        print("g++ was not found. Install MinGW-w64 and add g++ to PATH.")
    except subprocess.CalledProcessError:
        print("C++ compilation failed. Check the compiler errors above.")

    return False


def main():
    setup_data()
    backend_ready = setup_backend()

    print("\nSetup complete.")
    print("Demo admin login: admin / admin@123")
    print("Demo employee logins: E101-E110")
    print("Employee passwords: Emp@101 through Emp@110")

    if not backend_ready:
        print("WARNING: The C++ backend is not ready.")
        print("Fix the compilation issue before running the application.")

    print("Demo credentials only; change them before public deployment.")


if __name__ == "__main__":
    main()

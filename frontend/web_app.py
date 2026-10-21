import csv
from flask import Flask, send_from_directory,request, jsonify #request reads the data submitted by the website.
import subprocess #Allows python to run your existing C++ executable.
from pathlib import Path

app = Flask(__name__)

WEB_DIR = Path(__file__).resolve().parent / "web"

PROJECT_DIR = Path(__file__).resolve().parent.parent
BACKEND_EXE = PROJECT_DIR / "backend" / "leave_backend.exe"
CSV_FILE = PROJECT_DIR / "data" / "leave_requests.csv"

@app.route("/")
def home():
    return send_from_directory(WEB_DIR, "index.html")


@app.route("/<path:filename>")
def static_files(filename):
    return send_from_directory(WEB_DIR, filename)

@app.route("/apply", methods=["POST"])
def apply_leave():
    employee_id = request.form.get("employeeId", "").strip()
    leave_type = request.form.get("leaveType", "").strip()
    start_date = request.form.get("startDate", "").strip()
    end_date = request.form.get("endDate", "").strip()
    reason = request.form.get("reason", "").strip()

    if not all([employee_id, leave_type, start_date, end_date, reason]):
        return "Please complete all fields.", 400

    if not BACKEND_EXE.exists():
        return "C++ backend executable not found.", 500

    if "," in reason:
        return "Please do not use commas in the reason.", 400

    try:
        result = subprocess.run(
            [
                str(BACKEND_EXE),
                employee_id,
                start_date,
                end_date,
                reason
            ],
            cwd=str(PROJECT_DIR),
            capture_output=True,
            text=True,
            timeout=10
        )

        if result.returncode == 0:
            return result.stdout, 200

        return result.stdout or result.stderr or "Request failed.", 400

    except subprocess.TimeoutExpired:
        return "The backend took too long to respond.", 504

@app.route("/requests", methods=["GET"]) #This helps in reading the previous leave requests
def get_requests():                      #which were saved in the csv file and returns them
                                         #in JSON format so that JavaScript can understand it.
    if not CSV_FILE.exists():
        return jsonify([])

    with open(CSV_FILE, "r", newline="", encoding="utf-8") as file:
        reader = csv.DictReader(file)
        requests_list = list(reader)

    return jsonify(requests_list)

if __name__ == "__main__":
    app.run(debug=True)

#What does this code do?
#- Flask creates the Python web application.
#- Path helps Python locate your project files.
#- WEB_DIR points to your frontend/web folder.
#- @app.route("/") defines the website's home URL.
#- home() sends your index.html to the browser.
#- static_files() serves your CSS and JavaScript files.
#- app.run(debug=True) starts the local development server.

import tkinter as tk
from tkinter import messagebox
import subprocess
from pathlib import Path

PROJECT_DIR = Path(__file__).resolve().parent.parent
BACKEND_EXE = PROJECT_DIR / "backend" / "leave_backend.exe"


def apply_leave():
    employee_id = employee_entry.get().strip()
    start_date = start_entry.get().strip()
    end_date = end_entry.get().strip()
    reason = reason_entry.get().strip()

    if not all([employee_id, start_date, end_date, reason]):
        messagebox.showerror("Error", "Please fill in every field.")
        return

    if not BACKEND_EXE.exists():
        messagebox.showerror(
            "Error",
            "C++ backend not found. Compile it first."
        )
        return

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
            messagebox.showinfo("Backend Result", result.stdout)
        else:
            messagebox.showerror(
                "Request Failed",
                result.stdout or result.stderr or "Unknown error"
            )

    except Exception as error:
        messagebox.showerror("Error", str(error))


root = tk.Tk()
root.title("Employee Leave Management System")
root.geometry("430x390")

tk.Label(
    root,
    text="Employee Leave Management System",
    font=("Arial", 15, "bold")
).pack(pady=15)

tk.Label(root, text="Employee ID").pack()
employee_entry = tk.Entry(root, width=35)
employee_entry.pack(pady=4)

tk.Label(root, text="Start Date (YYYY-MM-DD)").pack()
start_entry = tk.Entry(root, width=35)
start_entry.pack(pady=4)

tk.Label(root, text="End Date (YYYY-MM-DD)").pack()
end_entry = tk.Entry(root, width=35)
end_entry.pack(pady=4)

tk.Label(root, text="Reason (no commas)").pack()
reason_entry = tk.Entry(root, width=35)
reason_entry.pack(pady=4)

tk.Button(
    root,
    text="Apply for Casual Leave",
    command=apply_leave,
    width=25
).pack(pady=20)

root.mainloop()

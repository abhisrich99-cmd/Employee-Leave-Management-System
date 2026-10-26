
const form = document.getElementById("leaveForm");
const startInput = document.getElementById("startDate");
const endInput = document.getElementById("endDate");
const durationPreview = document.getElementById("durationPreview");
const requestList = document.getElementById("requestList");
const adminRequestList = document.getElementById("adminRequestList");


// ---------------- LOGOUT ----------------

const logoutButton = document.getElementById("logoutButton");

if (logoutButton) {
    logoutButton.addEventListener("click", async () => {
        try {
            const response = await fetch("/logout", {
                method: "POST"
            });

            if (response.ok) {
                window.location.href = "/login-page";
            } else {
                alert("Logout failed. Please try again.");
            }
        } catch (error) {
            console.error("Logout error:", error);
            alert("Could not connect to the server.");
        }
    });
}


// ---------------- LOAD EMPLOYEE REQUESTS ----------------

async function loadRequests() {
    if (!requestList) return;

    try {
        const response = await fetch("/my-requests");

        if (response.status === 401 || response.status === 403) {
            window.location.href = "/login-page";
            return;
        }

        if (!response.ok) {
            throw new Error("Could not load your leave requests.");
        }

        const requests = await response.json();

        const pendingCount = document.getElementById("pendingCount");

        if (pendingCount) {
            pendingCount.textContent = requests.filter(
                request => request.Status === "PENDING"
            ).length;
        }

        requestList.innerHTML = "";

        if (requests.length === 0) {
            requestList.innerHTML = `
                <div class="empty-state">
                    <div class="empty-icon">▤</div>
                    <h3>No requests yet</h3>
                    <p>Your submitted leave requests will appear here.</p>
                </div>`;
            return;
        }

        requests.forEach(request => {
            const item = document.createElement("div");
            item.className = "request-item";

            const title = document.createElement("h3");
            title.textContent = request.LeaveType;

            const dates = document.createElement("p");
            dates.textContent =
                `Dates: ${request.StartDate} to ${request.EndDate}`;

            const duration = document.createElement("p");
            duration.textContent =
                `Duration: ${request.Duration} day(s)`;

            const reason = document.createElement("p");
            reason.textContent = `Reason: ${request.Reason}`;

            const status = document.createElement("span");
            status.className = "status";
            status.textContent = request.Status;
            status.classList.add(request.Status.toLowerCase());

            item.append(title, dates, duration, reason, status);
            requestList.appendChild(item);
        });

    } catch (error) {
        console.error("Error loading employee requests:", error);
    }
}


// ---------------- LOAD ADMIN REQUESTS ----------------

async function loadAdminRequests() {
    if (!adminRequestList) return;

    try {
        const response = await fetch("/requests");

        if (response.status === 401 || response.status === 403) {
            window.location.href = "/login-page";
            return;
        }

        if (!response.ok) {
            throw new Error("Could not load admin requests.");
        }

        const requests = await response.json();

        const pendingRequests = requests.filter(
            request => request.Status === "PENDING"
        );

        const adminPendingCount =
            document.getElementById("adminPendingCount");

        if (adminPendingCount) {
            adminPendingCount.textContent = pendingRequests.length;
        }

        adminRequestList.innerHTML = "";

        if (pendingRequests.length === 0) {
            adminRequestList.innerHTML =
                '<p class="empty-state">No pending requests.</p>';
            return;
        }

        pendingRequests.forEach(request => {
            const item = document.createElement("div");
            item.className = "request-item";

            const title = document.createElement("h3");
            title.textContent = request.LeaveType;

            const employee = document.createElement("p");
            employee.textContent =
                `Employee ID: ${request.EmployeeID}`;

            const dates = document.createElement("p");
            dates.textContent =
                `Dates: ${request.StartDate} to ${request.EndDate}`;

            const duration = document.createElement("p");
            duration.textContent =
                `Duration: ${request.Duration} day(s)`;

            const reason = document.createElement("p");
            reason.textContent = `Reason: ${request.Reason}`;

            const status = document.createElement("span");
            status.className = "status pending";
            status.textContent = request.Status;

            const approveButton = document.createElement("button");
            approveButton.textContent = "Approve";
            approveButton.className = "primary-button";

            const rejectButton = document.createElement("button");
            rejectButton.textContent = "Reject";
            rejectButton.className = "reject-button";

            approveButton.addEventListener("click", () => {
                updateRequestStatus(request.RequestID, "APPROVED");
            });

            rejectButton.addEventListener("click", () => {
                updateRequestStatus(request.RequestID, "REJECTED");
            });

            const actions = document.createElement("div");
            actions.className = "admin-actions";
            actions.append(approveButton, rejectButton);

            item.append(
                title,
                employee,
                dates,
                duration,
                reason,
                status,
                actions
            );

            adminRequestList.appendChild(item);
        });

    } catch (error) {
        console.error("Error loading admin requests:", error);
    }
}


// ---------------- APPROVE OR REJECT ----------------

async function updateRequestStatus(requestId, status) {
    try {
        const response = await fetch("/update-status", {
            method: "POST",
            headers: {
                "Content-Type": "application/x-www-form-urlencoded"
            },
            body: new URLSearchParams({
                requestId,
                status
            })
        });

        const message = await response.text();

        if (response.status === 401 || response.status === 403) {
            alert("You are not authorized to perform this action.");
            window.location.href = "/login-page";
            return;
        }

        if (!response.ok) {
            alert(message);
            return;
        }

        alert(message);

        await loadAdminRequests();

    } catch (error) {
        console.error("Error updating request:", error);
        alert("Could not connect to the Flask server.");
    }
}


// ---------------- LEAVE DURATION PREVIEW ----------------

function updateDuration() {
    if (!startInput || !endInput || !durationPreview) return;

    const start = startInput.value;
    const end = endInput.value;

    if (!start || !end) {
        durationPreview.textContent =
            "Select your dates to calculate duration.";
        return;
    }

    const [sy, sm, sd] = start.split("-").map(Number);
    const [ey, em, ed] = end.split("-").map(Number);

    const startDay = Date.UTC(sy, sm - 1, sd);
    const endDay = Date.UTC(ey, em - 1, ed);

    if (endDay < startDay) {
        durationPreview.textContent =
            "End date must not be before start date.";
        return;
    }

    const days = Math.round(
        (endDay - startDay) / 86400000
    ) + 1;

    durationPreview.textContent =
        `Your requested leave: ${days} day${days === 1 ? "" : "s"}.`;
}

if (startInput && endInput && durationPreview) {
    startInput.addEventListener("change", updateDuration);
    endInput.addEventListener("change", updateDuration);
}


// ---------------- SUBMIT LEAVE REQUEST ----------------

if (form) {
    form.addEventListener("submit", async event => {
        event.preventDefault();

        const leaveType =
            document.getElementById("leaveType").value;
        const startDate = startInput.value;
        const endDate = endInput.value;
        const reason =
            document.getElementById("reason").value.trim();

        if (!leaveType || !startDate || !endDate || !reason) {
            alert("Please complete all fields.");
            return;
        }

        if (endDate < startDate) {
            alert("End date cannot be before start date.");
            return;
        }

        if (reason.includes(",")) {
            alert("Please do not use commas in the reason.");
            return;
        }

        try {
            const response = await fetch("/apply", {
                method: "POST",
                headers: {
                    "Content-Type": "application/x-www-form-urlencoded"
                },
                body: new URLSearchParams({
                    leaveType,
                    startDate,
                    endDate,
                    reason
                })
            });

            const message = await response.text();

            if (response.status === 401 || response.status === 403) {
                alert("Please log in as an employee to apply for leave.");
                window.location.href = "/login-page";
                return;
            }

            if (!response.ok) {
                alert(message);
                return;
            }

            alert("Leave request submitted successfully!\n\n" + message);

            form.reset();

            if (durationPreview) {
                durationPreview.textContent =
                    "Select your dates to calculate duration.";
            }

            await loadRequests();

        } catch (error) {
            console.error("Submission error:", error);
            alert("Could not connect to the Flask server.");
        }
    });
}


// ---------------- NAVIGATION ----------------

document.querySelectorAll(".nav-link").forEach(link => {
    link.addEventListener("click", () => {
        document.querySelectorAll(".nav-link").forEach(item => {
            item.classList.remove("active");
        });

        link.classList.add("active");
    });
});


 // ---------------- LOAD EMPLOYEE LEAVE BALANCES ----------------

async function loadBalances() {
    const casualBalance = document.getElementById("casualBalance");
    const sickBalance = document.getElementById("sickBalance");
    const earnedBalance = document.getElementById("earnedBalance");

    // This function runs only on the employee page.
    if (!casualBalance || !sickBalance || !earnedBalance) return;

    try {
        const response = await fetch("/my-balances");

        if (response.status === 401 || response.status === 403) {
            window.location.href = "/login-page";
            return;
        }

        if (!response.ok) {
            throw new Error("Could not load leave balances.");
        }

        const balances = await response.json();

        casualBalance.innerHTML =
            `${balances["Casual Leave"]} <small>days</small>`;

        sickBalance.innerHTML =
            `${balances["Sick Leave"]} <small>days</small>`;

        earnedBalance.innerHTML =
            `${balances["Earned Leave"]} <small>days</small>`;

    } catch (error) {
        console.error("Error loading leave balances:", error);
    }
}

// ---------------- INITIALIZE ----------------

loadRequests();
loadAdminRequests();
loadBalances();
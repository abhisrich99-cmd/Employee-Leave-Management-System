const form = document.getElementById("leaveForm");
const startInput = document.getElementById("startDate");
const endInput = document.getElementById("endDate");
const durationPreview = document.getElementById("durationPreview");
const requestList = document.getElementById("requestList");

//The meaning of the upcoming function is below the function.
async function loadRequests() {
    try {
        const response = await fetch("/requests");

        if (!response.ok) {
            throw new Error("Could not load leave requests.");
        }

        const requests = await response.json();

        requestList.innerHTML = "";

        if (requests.length === 0) {
            requestList.innerHTML =
                '<p class="empty-state">No leave requests yet.</p>';
            return;
        }

        requests.forEach((request) => {
            const item = document.createElement("div");
            item.className = "request-item";

            const title = document.createElement("h3");
            title.textContent = request.LeaveType;

            const dates = document.createElement("p");
            dates.textContent =
                `${request.StartDate} to ${request.EndDate}`;

            const reason = document.createElement("p");
            reason.textContent = `Reason: ${request.Reason}`;

            const status = document.createElement("span");
            status.className = "status";
            status.textContent = request.Status;

            item.append(title, dates, reason, status);
            requestList.appendChild(item);
        });
    } catch (error) {
        console.error("Error loading requests:", error);
    }
}

loadRequests();

//fetch("/requests") asks Flask for the saved requests.
//await response.json() waits for the response and converts it into JavaScript data.
//requests.forEach(...) processes each saved request.
//The code creates a card showing its leave type, dates, reason, and status.
//loadRequests() at the end runs the function when the page loads.

function updateDuration() {
    const start = startInput.value;
    const end = endInput.value;

    if (!start || !end) {
        durationPreview.textContent =
            "Select your dates to calculate duration.";
        return;
    }

    // Split date components instead of relying on UTC parsing.
    const [sy, sm, sd] = start.split("-").map(Number); //map conerts the string yr,m,d to num
    const [ey, em, ed] = end.split("-").map(Number);

    const startDay = Date.UTC(sy, sm - 1, sd); //Date.UTC() converts the date components 
                                               //into a numerical timestamp.
//We want to subtract two dates to find the number of days between them for better user
//experience.
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

startInput.addEventListener("change", updateDuration);
endInput.addEventListener("change", updateDuration);

form.addEventListener("submit", async(event) => {
    event.preventDefault();

    const employeeId = document.getElementById("employeeId").value.trim();
    const leaveType = document.getElementById("leaveType").value;
    const startDate = startInput.value;
    const endDate = endInput.value;
    const reason = document.getElementById("reason").value.trim();

    if (!employeeId || !startDate || !endDate || !reason) {
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

    const item = document.createElement("div");
    item.className = "request-item";

       try {
        const response = await fetch("/apply", {
            method: "POST",
            headers: {
                "Content-Type": "application/x-www-form-urlencoded"
            },
            body: new URLSearchParams({
                employeeId: employeeId,
                leaveType: leaveType,
                startDate: startDate,
                endDate: endDate,
                reason: reason
            })
        });

        const message = await response.text();

        if (!response.ok) {
            alert(message);
            return;
        }

        alert("Leave request submitted successfully!\n\n" + message);

        form.reset();
        durationPreview.textContent =
            "Select your dates to calculate duration.";

        await loadRequests();

    } catch (error) {
        alert("Could not connect to the Flask server. Make sure it is running.");
        console.error(error);
    }
});

document.querySelectorAll(".nav-link").forEach((link) => {
    link.addEventListener("click", () => {
        document.querySelectorAll(".nav-link").forEach((item) => {
            item.classList.remove("active");
        });

        link.classList.add("active");
    });
});

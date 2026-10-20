const form = document.getElementById("leaveForm");
const startInput = document.getElementById("startDate");
const endInput = document.getElementById("endDate");
const durationPreview = document.getElementById("durationPreview");
const requestList = document.getElementById("requestList");

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

from flask import Flask, send_from_directory
from pathlib import Path

app = Flask(__name__)

WEB_DIR = Path(__file__).resolve().parent / "web"


@app.route("/")
def home():
    return send_from_directory(WEB_DIR, "index.html")


@app.route("/<path:filename>")
def static_files(filename):
    return send_from_directory(WEB_DIR, filename)


if __name__ == "__main__":
    app.run(debug=True)
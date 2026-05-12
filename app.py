"""
app.py  —  Queue Simulation Flask Server
=========================================
Routes
------
GET  /          → serves index.html (the UI)
POST /run       → accepts JSON params, writes settings.txt,
                  runs ./simulation, reads results.csv, returns JSON
GET  /health    → quick liveness check
"""

import os
import subprocess
import csv
import json
from flask import Flask, request, jsonify, send_from_directory

# ── Paths ──────────────────────────────────────────────────────────────────
BASE_DIR     = os.path.dirname(os.path.abspath(__file__))
import platform
SIM_EXE = os.path.join(BASE_DIR, "simulation.exe" if platform.system() == "Windows" else "simulation")
SETTINGS_TXT = os.path.join(BASE_DIR, "settings.txt")
RESULTS_CSV  = os.path.join(BASE_DIR, "results.csv")

app = Flask(__name__, static_folder=BASE_DIR)

# ── Helpers ────────────────────────────────────────────────────────────────

def write_settings(num_servers: int, arrival_probability: int,
                   max_service_time: int, max_simulation_time: int) -> None:
    """Write parameters to settings.txt so the C++ exe can read them."""
    with open(SETTINGS_TXT, "w") as f:
        f.write(f"{num_servers} {arrival_probability} {max_service_time} {max_simulation_time}\n")


def run_simulation(timeout: int = 30) -> tuple[bool, str]:
    """
    Execute the compiled C++ simulation.
    Returns (success: bool, message: str).
    """
    if not os.path.isfile(SIM_EXE):
        return False, f"Simulation binary not found at {SIM_EXE}. Did you compile with: g++ -o simulation main.cpp -std=c++17?"

    try:
        result = subprocess.run(
            [SIM_EXE],
            cwd=BASE_DIR,
            capture_output=True,
            text=True,
            timeout=timeout,
        )
        if result.returncode != 0:
            return False, f"Simulation exited with code {result.returncode}.\nstderr: {result.stderr}"
        return True, result.stdout
    except subprocess.TimeoutExpired:
        return False, f"Simulation timed out after {timeout} seconds."
    except Exception as e:
        return False, str(e)


def read_results() -> list[dict]:
    """Parse results.csv into a list of row dicts."""
    if not os.path.isfile(RESULTS_CSV):
        raise FileNotFoundError("results.csv was not produced by the simulation.")

    rows = []
    with open(RESULTS_CSV, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            rows.append({
                "minute":            int(row["minute"]),
                "queue_length":      int(row["queue_length"]),
                "customers_served":  int(row["customers_served"]),
                "avg_wait_time":     round(float(row["avg_wait_time"]), 2),
            })
    return rows


# ── Routes ─────────────────────────────────────────────────────────────────

@app.route("/")
def index():
    """Serve the frontend HTML."""
    return send_from_directory(BASE_DIR, "index.html")


@app.route("/health")
def health():
    return jsonify({"status": "ok", "sim_binary_exists": os.path.isfile(SIM_EXE)})


@app.route("/run", methods=["POST"])
def run():
    """
    Expected JSON body:
    {
        "servers":          int,   // number of servers         (1–20)
        "arrival_rate":     int,   // arrival probability 1-100%
        "service_time":     int,   // max service time in minutes
        "simulation_time":  int    // optional, default 60
    }

    Returns JSON:
    {
        "rows":   [ { minute, queue_length, customers_served, avg_wait_time }, … ],
        "summary": { avg_wait_time, max_queue_length, total_served },
        "params":  { servers, arrival_rate, service_time, simulation_time },
        "log":     "…stdout from the exe…"
    }
    """
    body = request.get_json(force=True, silent=True) or {}

    # ── Validate & extract params ──────────────────────────────────────────
    errors = []

    def get_int(key, default, lo, hi):
        val = body.get(key, default)
        try:
            val = int(val)
        except (TypeError, ValueError):
            errors.append(f"'{key}' must be an integer.")
            return default
        if not (lo <= val <= hi):
            errors.append(f"'{key}' must be between {lo} and {hi} (got {val}).")
            return default
        return val

    num_servers       = get_int("servers",         3,  1,  20)
    arrival_prob      = get_int("arrival_rate",    30,  1, 100)
    max_service_time  = get_int("service_time",     8,  1, 120)
    sim_time          = get_int("simulation_time", 60,  5, 500)

    if errors:
        return jsonify({"error": " | ".join(errors)}), 400

    # ── Run pipeline ──────────────────────────────────────────────────────
    write_settings(num_servers, arrival_prob, max_service_time, sim_time)

    ok, message = run_simulation()
    if not ok:
        return jsonify({"error": message}), 500

    try:
        rows = read_results()
    except FileNotFoundError as e:
        return jsonify({"error": str(e)}), 500

    if not rows:
        return jsonify({"error": "results.csv is empty."}), 500

    # ── Build summary ──────────────────────────────────────────────────────
    wait_times   = [r["avg_wait_time"]  for r in rows]
    queue_lens   = [r["queue_length"]   for r in rows]
    total_served = rows[-1]["customers_served"]

    summary = {
        "avg_wait_time":   round(sum(wait_times) / len(wait_times), 2),
        "max_queue_length": max(queue_lens),
        "total_served":    total_served,
    }

    return jsonify({
        "rows":    rows,
        "summary": summary,
        "params":  {
            "servers":         num_servers,
            "arrival_rate":    arrival_prob,
            "service_time":    max_service_time,
            "simulation_time": sim_time,
        },
        "log": message,
    })


# ── Entry point ────────────────────────────────────────────────────────────
if __name__ == "__main__":
    print("Queue Simulation Server")
    print(f"  Working dir : {BASE_DIR}")
    print(f"  Sim binary  : {SIM_EXE}")
    print(f"  Running on  : http://localhost:5000")
    app.run(debug=True, port=5000)

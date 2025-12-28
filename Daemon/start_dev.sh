#!/usr/bin/env bash
set -euo pipefail

# Start a socat virtual serial pair and then the Python daemon.
# Usage: ./start.sh

BASE=$(mktemp -u /tmp/axiom-tty.XXXXXX)
PORT_DAEMON="${BASE}-A"
PORT_TEST="${BASE}-B"
SOCAT_LOG="/tmp/socat-$$.log"

cleanup() {
    echo "\nStopping..."
    if [[ -n "${PY_PID-}" ]]; then kill "${PY_PID}" 2>/dev/null || true; fi
    if [[ -n "${SOCAT_PID-}" ]]; then kill "${SOCAT_PID}" 2>/dev/null || true; fi
    wait "${PY_PID-}" 2>/dev/null || true
    wait "${SOCAT_PID-}" 2>/dev/null || true
    rm -f "$PORT_DAEMON" "$PORT_TEST" "$SOCAT_LOG" || true
}
trap cleanup EXIT

# Start socat to create two linked PTYs
socat -d -d pty,raw,echo=0,link="$PORT_DAEMON" pty,raw,echo=0,link="$PORT_TEST" 2> "$SOCAT_LOG" &
SOCAT_PID=$!

# Wait for the PTY device files to appear
for i in {1..50}; do
    if [[ -e "$PORT_DAEMON" && -e "$PORT_TEST" ]]; then
        break
    fi
    sleep 0.1
done

if [[ ! -e "$PORT_DAEMON" || ! -e "$PORT_TEST" ]]; then
    echo "Failed to create PTYs. See $SOCAT_LOG" >&2
    cat "$SOCAT_LOG" >&2 || true
    exit 1
fi

echo "Created virtual serial pair:" 
echo "  daemon port (used by python): $PORT_DAEMON" 
echo "  test port (connect for commands): $PORT_TEST" 

echo "Example: connect to the test port with 'socat - $PORT_TEST,raw,echo=0' or 'screen $PORT_TEST 115200'"

# Select Python executable (use venv if available)
PYTHON=python3
if [[ -x .venv/bin/python ]]; then
    PYTHON=.venv/bin/python
fi

# Start the camera daemon and point it at the daemon port
$PYTHON main.py --port "$PORT_DAEMON" &
PY_PID=$!

# Wait for python to exit (cleanup will stop socat)
wait "$PY_PID"

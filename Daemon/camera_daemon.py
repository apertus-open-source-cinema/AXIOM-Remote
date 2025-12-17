# camera_daemon.py
from dataclasses import dataclass
from typing import Any, List, Optional

MAX_LINE = 180  # max allowed line length

# -----------------------------
# PARAMETER MODEL
# -----------------------------
@dataclass
class Param:
    name: str
    type: str  # "int", "float", "enum"
    value: Any
    rw: bool = True
    min: Optional[float] = None
    max: Optional[float] = None
    step: Optional[float] = None
    unit: Optional[str] = None
    values: Optional[List[str]] = None

# -----------------------------
# PARAMETERS
# -----------------------------
params = {
    "exposure_us": Param(name="exposure_us", type="int", value=12000, min=1, max=100000, step=1, unit="us"),
    "wb_mode": Param(name="wb_mode", type="enum", value="auto", values=["auto", "daylight", "tungsten"]),
    "focus_mode": Param(name="focus_mode", type="enum", value="auto", values=["auto", "manual"]),
    "focus_pos": Param(name="focus_pos", type="int", value=0, min=0, max=255),
    "temp_soc": Param(name="temp_soc", type="float", value=52.3, rw=False, unit="C"),
}

# -----------------------------
# PROTOCOL ERRORS
# -----------------------------
class ProtoError(Exception):
    def __init__(self, code: str, detail: Optional[str] = None):
        self.code = code
        self.detail = detail

    def response(self):
        if self.detail:
            return f"ERR {self.code} {self.detail}"
        return f"ERR {self.code}"

# -----------------------------
# VALUE PARSING
# -----------------------------
def parse_value(param: Param, raw: str):
    if param.type == "int":
        try:
            v = int(raw)
        except ValueError:
            raise ProtoError("invalid_value")
    elif param.type == "float":
        try:
            v = float(raw)
        except ValueError:
            raise ProtoError("invalid_value")
    elif param.type == "enum":
        if raw not in param.values:
            raise ProtoError("invalid_enum")
        return raw
    else:
        raise ProtoError("internal", "unknown_type")

    if param.min is not None and v < param.min:
        raise ProtoError("out_of_range")
    if param.max is not None and v > param.max:
        raise ProtoError("out_of_range")

    return v

# -----------------------------
# COMMAND HANDLERS
# -----------------------------
def cmd_get(name: str):
    if name not in params:
        raise ProtoError("unknown_param")
    p = params[name]
    return f"OK {name} {p.value}"

def cmd_set(name: str, raw_value: str):
    if name not in params:
        raise ProtoError("unknown_param")
    p = params[name]
    if not p.rw:
        raise ProtoError("read_only")

    value = parse_value(p, raw_value)

    # Focus position only allowed in manual mode
    if name == "focus_pos" and params["focus_mode"].value != "manual":
        raise ProtoError("invalid_state")

    # Apply value (replace with actual camera code later)
    p.value = value
    return "OK"

def cmd_info(name: str):
    if name not in params:
        raise ProtoError("unknown_param")
    p = params[name]
    fields = [f"type={p.type}"]
    if p.min is not None:
        fields.append(f"min={p.min}")
    if p.max is not None:
        fields.append(f"max={p.max}")
    if p.step is not None:
        fields.append(f"step={p.step}")
    if p.unit is not None:
        fields.append(f"unit={p.unit}")
    if p.values is not None:
        fields.append(f"values={','.join(p.values)}")
    return f"OK {name} " + " ".join(fields)

# -----------------------------
# COMMAND DISPATCHER
# -----------------------------
def handle_command(line: str) -> str:
    raw_line = line.rstrip("\r\n")
    if len(raw_line) > MAX_LINE:
        return "ERR too_long"

    tokens = raw_line.strip().split()
    if not tokens:
        return "ERR empty"

    cmd = tokens[0]
    try:
        if cmd == "HELLO":
            return "OK proto=1.0 device=camera-daemon"
        elif cmd == "LIST":
            return "OK " + " ".join(params.keys())
        elif cmd == "GET" and len(tokens) == 2:
            return cmd_get(tokens[1])
        elif cmd == "SET" and len(tokens) == 3:
            return cmd_set(tokens[1], tokens[2])
        elif cmd == "INFO" and len(tokens) == 2:
            return cmd_info(tokens[1])
        else:
            raise ProtoError("syntax")
    except ProtoError as e:
        return e.response()
    except Exception:
        return "ERR internal"

# -----------------------------
# SERIAL LOOP
# -----------------------------
def run(port="/dev/ttyACM0", baud=115200):
    import serial
    ser = serial.Serial(port, baud, timeout=1)

    print("Camera daemon started")
    while True:
        try:
            line = ser.readline().decode(errors="ignore")
            if not line:
                continue
            response = handle_command(line)
            ser.write((response + "\n").encode())
        except Exception:
            # never die
            pass

# -----------------------------
# OPTIONAL: TEST HARNESS
# -----------------------------
if __name__ == "__main__":
    while True:
        line = input("> ")
        print(handle_command(line))

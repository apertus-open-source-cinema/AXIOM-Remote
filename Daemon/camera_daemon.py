import logging
import serial
from utils import ProtocolError

log = logging.getLogger(__name__)

MAX_LINE = 180


class CameraDaemon:
    def __init__(self, manager):
        self.manager = manager

    def cmd_get(self, name: str):
        if name not in self.manager.params:
            raise ProtocolError("unknown_param")
        # self.manager.get(name) returns value
        val = self.manager.get(name)
        return f"OK {name} {val}"

    def cmd_set(self, name: str, raw_value: str):
        if name not in self.manager.params:
            raise ProtocolError("unknown_param")
        
        p = self.manager.get_param(name)
        if not p.rw:
            raise ProtocolError("read_only")

        self.manager.set(name, raw_value)
        return "OK"

    def cmd_info(self, name: str):
        if name not in self.manager.params:
            raise ProtocolError("unknown_param")

        p = self.manager.get_param(name)
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

    def handle_command(self, line: str) -> str:
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
                return "OK " + " ".join(self.manager.params.keys())
            elif cmd == "GET" and len(tokens) == 2:
                return self.cmd_get(tokens[1])
            elif cmd == "SET" and len(tokens) == 3:
                return self.cmd_set(tokens[1], tokens[2])
            elif cmd == "INFO" and len(tokens) == 2:
                return self.cmd_info(tokens[1])
            else:
                raise ProtocolError("syntax")
        except ProtocolError as e:
            return e.response()
        except Exception as e:
            log.error(f"Internal error: {e}")
            return "ERR internal"

    def run(self, port="/dev/ttyACM0", baud=115200):
        ser = serial.Serial(port, baud, timeout=1)

        print("Camera daemon started")
        try:
            while True:
                try:
                    line = ser.readline().decode(errors="ignore")
                    if not line:
                        continue
                    log.info(f"RX: {line.strip()}")
                    response = self.handle_command(line)
                    ser.write((response + "\n").encode())
                except Exception as e:
                    log.error(f"Error: {e}")
                    # never die
                    pass
        except KeyboardInterrupt:
            print("\nCamera daemon stopped.")
        finally:
            ser.close()

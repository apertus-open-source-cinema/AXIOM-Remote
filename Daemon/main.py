import logging
import pprint
import os
from pathlib import Path
from camera_daemon import CameraDaemon
from fs_backend import FSBackend, CameraBackend
from param_manager import ParamManager

log = logging.getLogger(__name__)

supported_parameters = ["pga_gain", "dig_gain", "analog_gain"]

def main():
    logging.basicConfig(level=logging.INFO)
    import argparse
    parser = argparse.ArgumentParser(description="Camera daemon")
    parser.add_argument("--port", default="/dev/ttyACM0", help="Serial port")
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate")
    args = parser.parse_args()

    fs_backend = FSBackend()
    # Scan all parameters
    parameter_db, errors = fs_backend.scan_parameters(supported_parameters, "/tmp/nctrl-api/")
    if errors:
        log.error(errors)
        # Proceed even with errors if partial params found?
        if not parameter_db:
             exit(1)
        
    print("Found parameters:\n")
    pprint.pp(parameter_db)

    backend = CameraBackend()
    manager = ParamManager.from_parameter_db(parameter_db, backend)

    daemon = CameraDaemon(manager)
    daemon.run(port=args.port, baud=args.baud)

if __name__ == "__main__":
    main()
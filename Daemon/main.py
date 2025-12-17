from Daemon.camera_daemon import run


def main():
    import argparse
    parser = argparse.ArgumentParser(description="Camera daemon")
    parser.add_argument("--port", default="/dev/ttyACM0", help="Serial port")
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate")
    args = parser.parse_args()

    run(port=args.port, baud=args.baud)

if __name__ == "__main__":
    main()

# REMOTE daemon

REMOTE daemon is a Linux daemon that communicates with a remote control, shares camera parameters, and applies new settings sent from the remote side.


## TODOs

# systemd setup

**Path:** /etc/systemd/system/axiom-remote-daemon.service

```bash
[Unit]
Description=Camera Daemon
After=network.target

[Service]
Type=simple
ExecStart=/usr/bin/python3 /usr/local/bin/camera_daemon.py --port /dev/ttyACM0
Restart=always
User=pi
Group=pi
WorkingDirectory=/usr/local/bin
StandardOutput=syslog
StandardError=syslog
SyslogIdentifier=camera-daemon

[Install]
WantedBy=multi-user.target
```
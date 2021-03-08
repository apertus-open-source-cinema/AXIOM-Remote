# AXIOM Remote Firmware Visualizer

## About

The AXIOM Remote Visualizer is a tool to emulate the actual code running on the PIC32 and pixels displayed on the 320x240 LCD on a PC.

## Acquiring Git module

The **imgui** library is embedded as git module, so you need to execute:

    git submodule update --init --recursive 

to initialize it.

## Building

* Install required packages: **cmake, libsdl2-dev, libsdl2-image-dev**
* Open terminal, usual shortcut under Linux is **Ctrl+Alt+T**
* **cd** into **/AXIOM_Remote_Firmware_Visualizer**
* **mkdir build**
* **cd build**
* **cmake ..**
* **make -j4**
* **./AXIOM_Remote_Firmware_Visualizer**


These steps above can be done in one go with the **build.sh** script

## Serial communication

* Create virtual serial ports by executing:

  ```socat pty,raw,echo=0,link=/tmp/axiom_remote_port pty,raw,echo=0,link=/tmp/axiom_host_port```

* Start visualiser with command line argument ```-p```, use first port from previous step:

  ```./AXIOM_Remote_Firmware_Visualiser -p /tmp/axiom_remote_port```

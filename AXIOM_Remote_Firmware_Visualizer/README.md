<h1 align = "center"> AXIOM Remote Firmware Visualizer </h1>

## About

The AXIOM Remote Visualizer is a tool to emulate the actual code running on the PIC32 and pixels displayed on the 320x240 LCD on a PC.

## Acquiring Git module

The **imgui** library is embedded as git module, so you need to execute:

    git submodule update --init --recursive 

to initialize it.

## Installing Required Packages for Build

### For Linux Build

* Install required packages: **cmake, libsdl2-dev, libsdl2-image-dev**
* Open terminal, usual shortcut under Linux is **Ctrl+Alt+T**

### For Windows Build

* Install cmake on windows <br>
  <a href = "https://cmake.org/download/"> Download Link of Cmake for windows</a>
* Install a C++ compiler. <br>
  <a href = "https://nchc.dl.sourceforge.net/project/mingw-w64/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe"> Download Link For Minimalist GNU for Windows  </a>
* Download SDL2 and SDL2_image for your compiler.  <br>
  <a href = "https://www.libsdl.org/release/SDL2-devel-2.0.14-mingw.tar.gz"> Download Link of SDL2 for Minimalist GNU for Windows  </a> <br>
  <a href = "https://www.libsdl.org/release/SDL2-devel-2.0.14-mingw.tar.gz"> Download Link of SDL2_image for Minimalist GNU for Windows  </a>
* Copy the content (include,bin and lib folder) of SDL2 and SDL2_image, and paste it into your compiler installed Directory<br>

## Running the Visualizer

### For Linux Build

* **cd** into **/AXIOM_Remote_Firmware_Visualizer**
* **mkdir build**
* **cd build**
* **cmake ..**
* **make -j4**
* **./AXIOM_Remote_Firmware_Visualizer**

### For Windows Build

* **cd** into **/AXIOM_Remote_Firmware_Visualizer**
* **mkdir build**
* **cd build**
* **cmake  "MinGW Makefiles" ..**
* **mingw32-make -j4**
* **./AXIOM_Remote_Firmware_Visualizer.exe**

These steps above can be done in one go with the **build.sh** script

## Serial communication

* Create virtual serial ports by executing:

  ```socat pty,raw,echo=0,link=/tmp/axiom_remote_port pty,raw,echo=0,link=/tmp/axiom_host_port```

* Start visualiser with command line argument ```-p```, use first port from previous step:

  ```./AXIOM_Remote_Firmware_Visualiser -p /tmp/axiom_remote_port```

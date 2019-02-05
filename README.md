AXIOM Remote
========
**Build status**

| master |
|:------:|
|[![CircleCI](https://circleci.com/gh/apertus-open-source-cinema/AXIOM-Remote/tree/master.svg?style=svg)](https://circleci.com/gh/apertus-open-source-cinema/AXIOM-Remote/tree/master)|



**Description**

A universal remote control with rocker switches, buttons, dials and an LCD for menu/settings (no live video) for AXIOM devices and potentially many other things as well.

**Build visualizer**
- Required packages: **CMake**, **SDL2**
- Open terminal
- **cd** into **/visualizer**
- **mkdir build**
- **cd build**
- **cmake ..**
- **make -j4**
- **./axiom_remote_visualizer**


**Adjustments**
- Renamed folder "AXIOM Remote Prototype V01" to "axiom_remote_firmware"
- Added globals.h and globals.c which will receive parts from main.h to loosen up the dependencies
- Moved all #include <xc.h> to definitions.h
- Renamed uitility.c to utility.h
- Added media.h in /media, which is only file to include .c files for now
- Changed framebuffer width and height to constants through #define
- Removed underscores in variables and type names, which are reserved for system purposes (see C guidelines (search for underscores): https://cseweb.ucsd.edu/~ricko/CSE30/indhill-cstyle.html)
- Changed enum values to upper-case

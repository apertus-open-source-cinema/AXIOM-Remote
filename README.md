# AXIOM Remote

## Build status

| dev |
|:------:|
|[![CircleCI](https://circleci.com/gh/apertus-open-source-cinema/AXIOM-Remote/tree/dev.svg?style=svg)](https://circleci.com/gh/apertus-open-source-cinema/AXIOM-Remote/tree/dev)|
|[![codecov](https://codecov.io/gh/apertus-open-source-cinema/AXIOM-Remote/branch/dev/graph/badge.svg)](https://codecov.io/gh/apertus-open-source-cinema/AXIOM-Remote)

## Description

A universal remote control with buttons, dials and an LCD for menu/settings (no live video) for AXIOM devices and potentially many other things as well.

## License

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License 2 as published by the Free Software Foundation.
for details see: LICENSE.txt

## Folder Structure

```/.vscode/``` contains configurations for VS code IDE.

```/Archive/``` contains old outdated projects related to the AXIOM Remote.

```/AXIOM_Remote_Firmware_Visualizer/``` contains the AXIOM Remote Visualizer - a tool to emulate the actual code running on the PIC32 and pixels displayed on the 320x240 LCD on a PC.

```/Bootloader/``` The Bootloader will be used for handling of periphery (LCD, USB-UART, I2C) and updating the firmware of the main PIC32 and also the east and west PIC16s (key managers) without a dedicated programming hardware.

```/Common/``` contains general code and definition that are used in several projects in this repository.

```/datasheets/```  contains datasheets related to the used hardware (TFT and TFT controller, etc.).

```/Docs/``` contains illustrations and drawings

```/Firmware/``` contains the actual Firmware of the AXIOM Remote - this is the main folder of this repository. See the [README.md](/Firmware/README.md) inside the subfolder for more documentation about the GUI structure of the AXIOM Remote.

```/FirmwareTest/``` contains unit tests of the Firmware.

## Build instructions

### Fetch the Microchip XC32/XC32++ compiler for your OS

[https://www.microchip.com/mplab/compilers](https://www.microchip.com/mplab/compilers). We have tested the code on v2.4.

To be able to build, the variable **XC32_PATH** in the Makefile of firmware and bootloader has to be adjusted to the actual install path of XC32.

### Bootloader

- Open **Bootloader** folder in terminal
- Execute **make** or when rebuilding **make clean && make**
- **HEX** and **ELF** files would placed in the **build** folder
- After flashing connect with **minicom** and reset the board, after a short moment the information about west key manager will be shown

### Firmware

- Open **Firmware** folder in terminal
- Execute **make** or when rebuilding **make clean && make**
- **HEX** and **ELF** files would placed in the **build** folder

## Flash instructions

Note that with the current hardware the PICKit2 needs to stay connected for normal operation even when flashing has been completed as the PICkit2 pulls down the RESET signal.

### IPE

- Open IPE and connect to your PICKit3 or later, drag and drop the HEX file into the IPE window
- Click **Program**, after a moment it should be finished and main menu with 6 buttons shown

### pic32prog

- Acquire binary or build from source from: <https://github.com/sergev/pic32prog> to flash with PICKit2
- run **pic32prog yourfile.hex** and you should see a progress bar of the flashing/verification process

If ```pic32prog``` only works with ```sudo``` add your user to the plugdev group and add the following udev rule to /etc/udev/rules.d/26-microchip.rules (create file if it does not exist) and reboot:

```
ATTR{idVendor}=="04d8", MODE="664", GROUP="plugdev"
```

The idVendor should be looked up with ```lsusb```, in case of the PICkit2 it looks like this:

```
Bus 003 Device 002: ID 04d8:0033 Microchip Technology, Inc. PICkit2
```

## Development Environment

We use Visual Studio Code (<https://code.visualstudio.com/>) as IDE and supply some configurations for it in /.vscode/ .
This means its important to open the root folder of this repo in VS code.

We recommend installing the VsCode Action Buttons extension: <https://marketplace.visualstudio.com/items?itemName=seunlanlege.action-buttons> and configurations (.vscode/settings.json) to add buttons to compile and flash the AXIOM Remote.

## Coding Guidelines ##

Please refer to the [CodingGuidelines.md](CodingGuidelines.md)

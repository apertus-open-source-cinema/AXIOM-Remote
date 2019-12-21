# AXIOM Remote

## Description**

A universal remote control with buttons, dials and an LCD for menu/settings (no live video) for AXIOM devices and potentially many other things as well.

## Build status

| master | dev |
|:------:|:------:|
|[![CircleCI](https://circleci.com/gh/apertus-open-source-cinema/AXIOM-Remote/tree/master.svg?style=svg)](https://circleci.com/gh/apertus-open-source-cinema/AXIOM-Remote/tree/master)|[![CircleCI](https://circleci.com/gh/apertus-open-source-cinema/AXIOM-Remote/tree/dev.svg?style=svg)](https://circleci.com/gh/apertus-open-source-cinema/AXIOM-Remote/tree/dev)|

## Build instructions

### Bootloader

- Open **Bootloader** folder in terminal
- Execute **make** or when rebuilding **make clean && make**
- **HEX** and **ELF** files would placed in the **build** folder
- Open IPE and connect to your PICKit, drag and drop the HEX file into the IPE window
- Click **Program**, after a moment it should be finished 
- Connect with **minicom** and reset the board, after a short moment the information about west key manager will be shown

### Firmware

- Open **Firmware** folder in terminal
- Execute **make** or when rebuilding **make clean && make**
- **HEX** and **ELF** files would placed in the **build** folder
- Open IPE and connect to your PICKit, drag and drop the HEX file into the IPE window
- Click **Program**, after a moment it should be finished and main menu with 6 buttons shown

## TODO

- Add instructions for **pic32prog**

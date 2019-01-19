# AXIOM Remote
This software runs bare metal on the PIC32MZ. 

Further documentation/hardware schematics please see:
https://wiki.apertus.org/index.php/AXIOM_Remote
 
## License
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License 2 as published 
by the Free Software Foundation.
for details see: LICENSE.txt

## Folder Structure
```/nbproject/``` contains a project that can be opened in MPLABX (note that the XC32 compiler is a separate download).

```/datasheets/``` contains contains datasheets related to the used hardware (TFT and TFT controller, etc.).

```/reference/``` contains demo code that was developed to test/showcase several aspects of the hardware

## Terms used in the GUI
* *menu* - refers to a screen with a header (showing breadcrumbs) and 6 menu_items displayed at the same time on the TFT (scrollbars are automatically shown if more than 6 menu_items are present. A menu is typically navigated with the rotary/push knob.  (type: ```menu_t```)
* *menu_item* - refers to one option/line in the menu, can be hidden or disabled and can show readonly information, lead to another submenu or contain a numeric or dropdown list like selection (type: ```menu_item_t```)
* *parameter_menu* - this menu pops up when a menu_item containing a numeric or dropdown list selection parameter is activated.
* *page* -  a page refers to the display type where the 12 buttons around the TFT are utilized for navigation/operation. Each of the 6 page_items on screen is associated with one of the three buttons above or below the TFT. Pages could be seen like "desktops" on a PC with icons on them to click. (type: ```page_t```)
* *page_item* - each item on a page can lead to another page or menu when clicked (type: ```page_item_t```)

## Interfacing with the East/West PIC16
Two additional smaller PIC16 are used for handling push button, rotary encoder and LED IO.

They are connected to the PIC32MZ via i2c:
### PIC16F1718 West
    Index   Bits    Function
    0x00    [7:0]   Port A Change
    0x01    [7:0]   Port B Change
    0x02    [7:0]   Port C Change
    0x03            unused
    0x04    [7:0]   Port A Status
    0x05    [7:0]   Port B Status
    0x06    [7:0]   Port C Status
    0x07            unused
    .
    .
    .
    0x10    [7:0]   Quadrature Encoder 1
    0x11    [7:0]   Quadrature Encoder 2
    0x12            unused
    .
    .
    .
    0x20    [7:4]   Red Pattern
            [3:0]   Red PWM
    0x21    [7:4]   Green Pattern
            [3:0]   Green PWM
    0x22    [7:4]   Blue Pattern
            [3:0]   Blue PWM
    0x23    [7:0]   Pattern/PWM Load (auto clear)
    0x24            unused
    .
    .
    .
    0x30    [7:0]   Pattern Red [7:0]
    0x31    [7:0]   Pattern Red [15:8]
    0x32    [7:0]   Pattern Green [7:0]
    0x33    [7:0]   Pattern Green [15:8]
    0x34    [7:0]   Pattern Blue [7:0]
    0x35    [7:0]   Pattern Blue [15:8]
    0x36    [7:0]   Pattern Load

### PIC16F1718 East
    Index   Bits    Function
    0x00    [7:0]   Port A Change
    0x01    [7:0]   Port B Change
    0x02    [7:0]   Port C Change
    0x03            unused
    0x04    [7:0]   Port A Status
    0x05    [7:0]   Port B Status
    0x06    [7:0]   Port C Status
    0x07            unused
    .
    .
    .

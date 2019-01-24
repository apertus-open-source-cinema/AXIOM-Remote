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

## Key functions

### void displayFramebuffer()
in main.c (https://github.com/apertus-open-source-cinema/AXIOM-Remote/blob/3fb507dd943ca3533536b8fae8b2233deeffb364/AXIOM%20Remote%20Prototype%20V01/main.c#L658)

Transfers the content of the framebuffer (*_framebuffer[x][y]*) to the LCD - meaning a new picture is drawn/displayed.
This function also deals with any transition animation effects (*_transition_active* & *_transition_animation_type*).

### void draw_pixel(int16_t x, int16_t y, uint16_t color)
in draw.c (https://github.com/apertus-open-source-cinema/AXIOM-Remote/blob/2ffbb82b26a225873fe662a5b175444faa97bacb/AXIOM%20Remote%20Prototype%20V01/draw.c#L34)

This is the most basic drawing function that puts one color pixel into the framebuffer and is used in pretty much any other drawing function for rendering text, image or geometric shapes. This function deals with rotating the framebuffer (the LCD is meant to be used in portrait mode but we use it in landscape mode so that the origin 0,0 is in the lower left corner).

### void draw_menu()
in menu.c (https://github.com/apertus-open-source-cinema/AXIOM-Remote/blob/d8e71aba6b4d622804759e4bd52208523be7ca0f/AXIOM%20Remote%20Prototype%20V01/menu.c#L529)

This function draws the entire menu into the framebuffer. It renders the menu header (with breadcrumbs), side icons, menu items and scrollbar.

### void draw_page()
in page.c (https://github.com/apertus-open-source-cinema/AXIOM-Remote/blob/3906fd5cb76b287aae41864517cab496447eb3ce/AXIOM%20Remote%20Prototype%20V01/page.c#L160)

Draws one page with up to 12 page_items (buttons).

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

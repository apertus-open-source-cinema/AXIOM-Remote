# AXIOM Remote Firmware
This software runs bare metal on the PIC32MZ. 

Further documentation/hardware schematics please see:
https://wiki.apertus.org/index.php/AXIOM_Remote

## General UI Guidelines

Good User Interface (UI) / User Experience (UX) principles:

* any button/knob/dial (anything the user can interact with) should clearly show what will happen when it is pressed/turned/operated before doing so. Button labels should use actions like “Set” or “Start” instead of “OK” or “Yes”, etc. If turning the knob has an effect it should be clearly shown what will happen with icons/graphics/etc. No button should be called “User 1” or “A”, “B”, etc.
* The different screens should have a clear hierarchy or layout structure (like a big map) so the user always knows “where am I currently”. This shall be achieved with breadcrumbs, scrollbars, navigation bars, clear headers, shifting animations when switching screens, etc.
* Creating operating experience by using clear patterns for button placements. Eg. The CANCEL button is always at the bottom left, the primary action button always at the bottom right. The HOME and BACK buttons are always in the same place in the left sidebar. This makes learning the UI easy and fast, avoids confusion and creates trust.
* Big camera manufacturer Sony is known for their "infamous" UI designs choices with their cameras. A main problem they have is that setting some options have wide reaching implications that are not made obvious in the UI. For example some codecs require more processing power than others resulting in higher battery drain so Sony dims the LCD brightness to reduce battery drain and processor overheating with that particular codec. The user is not supplied with any such information so starts to wonder why the LCD is suddenly darker after choosing another codec. -> Any implication an action has should always be communicated in the UI before executing such action - if there is no space/way to do it in the menu/page directly this can be done for example in a “confirm action screen” that pops up when the user chooses to execute a change. Also options that are disabled because of another setting should never just disappear, rather they should be read-only and show an explanation why this option is disabled, example: “100 fps - disabled because of codec choice ABC..”.

## Terms used in GUI

<p align="center">
  <img src="https://lh6.googleusercontent.com/LAwx0tUKyB3_XFQIg46tdciVZxFjK_TwD84dJe8mCU8BmWBYeuxiuF2F-evQn0UwuPy8X4w085mN9Ti--YnOGHRjQyRjU95HS59ZCIPC">
</p>

Following terms can help understand the GUI better:

- **Screen**: It refers to the entire content of the LCD visible at one time. Currently, there are two types of screens:
	- **Page**: A page refers to the display type where the 12 buttons around the TFT are utilized for navigation/operation. Each of the 6 page_items on screen is associated with one of the three buttons above or below the TFT. Pages could be seen like "desktops" on a PC with icons on them to click. (type: page_t)
	- **Menu**: It refers to a screen with a header (showing breadcrumbs) and 6 menu_items displayed at the same time on the LCD (scrollbars are automatically shown if more than 6 menu items are present. A menu is typically navigated with the rotary/push knob.

- **PageItem**: Each item on a page acts like a button and can execute an action or can lead to another page or menu when clicked

- **MenuItem**: It refers to one option/line in the menu, can be hidden or disabled and can show readonly information, lead to another submenu or contain a boolean, numeric or dropdown list like selection.

- **ParameterMenu**: This menu pops up when a menu item containing a numeric or dropdown list selection parameter is clicked.

## Usage instructions

- USB communication to/from the AXIOM Remote is done via a USB communications device class (CDC)
- **sudo minicom -D /dev/ttyACM0** (baud rate is not required)

## LCD
The drawing origin (X,Y = 0,0) is located in the top left corner. The LCD is used in landscape (widescreen) mode.

## Communication Protocol
A simple ASCII based line prototcol is currently envisioned (not implemented yet):

Format:
```
Xyyyzz FIELDS...
```

where `X` indicates the request type, currently
- `G` for get
- `S` for set
- `R` for return value / reply

Get / set requests are initiated by the remote and answered asynchronously by the beta with a `R`

`yyy` is a alphanumeric id, for example a counter formatted in hex counting up
`zz` is the CRC8 (polynomial `0x7`, initial value `0x0`) in hex of everything coming after that, including the RS and the EOT (end of transmission - ASCII code 0x04)) character indicating the end of the message.
and `FIELDS` is a RS (Record Seperator - ASCII code 0x1E) seperated list of fields

for replies the id matches the id of the (get or set) request

example:
```
[remote to beta] G123459 analog_gain
[remote to beta] S1235FB analog_gain 5
[beta to remote] R12352F ERR set 4
[beta to remote] R12344B OK 1
```

The remote should use a timeout of `1s` for long running requests.

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


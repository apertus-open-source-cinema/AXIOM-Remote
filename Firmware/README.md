# AXIOM Remote Firmware
This software runs bare metal on the PIC32MZ. 

Further documentation/hardware schematics please see:
https://wiki.apertus.org/index.php/AXIOM_Remote

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


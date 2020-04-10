# LCD
The drawing origin (X,Y = 0,0) is located in the top left corner. The LCD is used in landscape (widescreen) mode.

# Communication Protocol
A simple ASCII based line prototcol is currently envisioned:

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


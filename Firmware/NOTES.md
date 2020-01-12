# Communication Protocol
A simple ASCII based line prototcol is currently envisioned:

G = GET
R = return value
S = SET

Example:
[Remote to BETA] G analog_gain
[BETA to Remote] R 1
[Remote to BETA] S analog_gain 2
[BETA to Remote] R success


Timeouts shall be used for waiting for replies.

First attempt will be made without any buffering, but if it shows to be a problem the requests and replies could be made async,
for example using a request id:
[Remote to BETA] G 1234 analog_gain
[Remote to BETA] S 1235 analog_gain 5
[BETA to Remote] R 1235 ERR set 4
[BETA to Remote] R 1234 OK 4

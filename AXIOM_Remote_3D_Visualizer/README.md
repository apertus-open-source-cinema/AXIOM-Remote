# AXIOM Remote 3D Visualizer

## Description

A 3D mockup of the real device with fully functional menu running directly in your browser. This should help with development and testing as well as allowing easy access to testing/playing around with the AXIOM Remote device before acquiring actual hardware.

## License

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License 2 as published by the Free Software Foundation.
for details see: LICENSE.txt

## Build instructions
```
npm install
npx emsdk-checkout
npx emsdk install latest
npx emsdk activate latest
npm run transpile_firmware
npm run dev
```
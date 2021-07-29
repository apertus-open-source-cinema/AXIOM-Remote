# AXIOM Remote 3D Visualizer

## Description

A 3D mockup of the real device with fully functional menu running directly in your browser. This should help with development and testing as well as allowing easy access to testing/playing around with the AXIOM Remote device before acquiring actual hardware.

## License

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License 2 as published by the Free Software Foundation.
for details see: LICENSE.txt

## Transpile Firmware - Requirements
Tested with node.js version 14 and up.

```
npx emsdk-checkout
npx emsdk install latest
npx emsdk activate latest
```

## Transpile Firmware

A binary version of the firmware is already contained in the repository in ```/src/js/FW/``` but if you want transpile a new version:

```
npm run transpile_firmware
```

## 3D Viewer - Requirements

```
npm install
```

## 3D Viewer - Requirements

```
npm run dev
```

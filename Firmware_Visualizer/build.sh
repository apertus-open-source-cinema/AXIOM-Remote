#!/bin/bash

#cmake -Bbuild -GNinja -DCMAKE_BUILD_TYPE=Debug . &&
cmake --build build &&

pushd build &&
./AXIOM_Remote_Firmware_Visualizer &&
popd

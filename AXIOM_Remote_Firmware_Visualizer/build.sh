rm -r build/
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j4
./AXIOM_Remote_Firmware_Visualizer

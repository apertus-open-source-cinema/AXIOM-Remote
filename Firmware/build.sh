cmake -B build -G Ninja . #-DCMAKE_TOOLCHAIN_FILE=./arm-toolchain.cmake .
cmake --build build

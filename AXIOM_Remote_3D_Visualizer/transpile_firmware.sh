#!/bin/bash

# Exit on error, undefined variables, and pipe failures
set -euo pipefail

# Check if running from correct directory
if [ ! -f "FirmwareBinder/FirmwareBinder.cpp" ]; then
    echo >&2 "Error: This script must be run from AXIOM_Remote_3D_Visualizer directory"
    exit 1
fi

# --- Configuration & Path Resolution ---
# We resolve absolute paths here to prevent breakage when we 'cd' later
FIRMWARE_ROOT=$(realpath "../Firmware")
LVGL_SRC=$(realpath "$FIRMWARE_ROOT/3rdParty/lvgl-src")
LVGL_BUILD=$(realpath "$FIRMWARE_ROOT/3rdParty/lvgl-build")
LV_CONF="$FIRMWARE_ROOT/lv_conf.h"
FIRMWARE_SRC="$FIRMWARE_ROOT/src/UI"
BUILD_DIR="build"
OUTPUT_DIR="src/js/FW"

# Validate source paths exist
if [ ! -d "$LVGL_SRC/src" ]; then
    echo >&2 "Error: LVGL source directory not found at: $LVGL_SRC"
    echo >&2 "Hint: Run 'git submodule update --init --recursive' in the project root."
    exit 1
fi

# Force clean build if LV_COLOR_DEPTH changed or if requested via --clean
if [[ "${1:-}" == "--clean" ]]; then
    echo "Cleaning build directories..."
    rm -rf build "$LVGL_BUILD/build_obj"
fi

COMMON_FLAGS="-O0 -DLV_CONF_INCLUDE_SIMPLE -DLV_LVGL_H_INCLUDE_SIMPLE -flto -gsource-map -DLV_COLOR_DEPTH=32" # -fexceptions
# production
# COMMON_FLAGS="-O3 -flto --closure 1 -DLV_CONF_INCLUDE_SIMPLE -DLV_LVGL_H_INCLUDE_SIMPLE"

# Include Root for relative paths, SRC for flat UI includes, and Binder for local headers
BINDER_DIR=$(realpath "FirmwareBinder")
INCLUDES="-I. -I$BINDER_DIR -I$FIRMWARE_ROOT -I$FIRMWARE_SRC -I$LVGL_SRC"

echo "--- Building LVGL static library ---"
mkdir -p "$LVGL_BUILD/build_obj"

# We use a subshell for building LVGL to keep our directory context clean
(
    cd "$LVGL_BUILD"
    # Find all C files and compile them
    echo "  Compiling LVGL source files (parallel)..."
    find "$LVGL_SRC/src" -name "*.c" | xargs -P "$(nproc)" -I {} bash -c '
        cfile="{}"
        obj_name=$(echo "$cfile" | md5sum | cut -d" " -f1).o
        if [ "$cfile" -nt "build_obj/$obj_name" ] || [ "'"$LV_CONF"'" -nt "build_obj/$obj_name" ]; then
            emcc '"$COMMON_FLAGS"' -c -I"'"$LVGL_SRC"'" -I"'"$FIRMWARE_ROOT"'" "$cfile" -o "build_obj/$obj_name"
        fi
    '

    echo "  Creating static library..."
    emar rcs liblvgl.a build_obj/*.o 2>/dev/null
)

echo "--- Starting GUI firmware compilation ---"
mkdir -p "$BUILD_DIR"

# Allow globs to expand to nothing if no files match
shopt -s nullglob

# Compile C++ files (Firmware + Binder)
CPP_FILES=(
    "$BINDER_DIR/FirmwareBinder.cpp"
    "$FIRMWARE_SRC/LvglUI.cpp"
    "$FIRMWARE_SRC/Theme.cpp"
    "$FIRMWARE_SRC/AppContext.cpp"
    $FIRMWARE_SRC/Screens/*.cpp
)

if [ ! -d "$FIRMWARE_SRC" ]; then
    echo >&2 "Error: Firmware UI source directory not found at $FIRMWARE_SRC"
    exit 1
fi

for cppfile in "${CPP_FILES[@]}"; do
    obj_file="$BUILD_DIR/$(basename "$cppfile" .cpp).o"
    if [ "$cppfile" -nt "$obj_file" ] || [ "$LV_CONF" -nt "$obj_file" ]; then
        echo "  Compiling $cppfile..."
        emcc $COMMON_FLAGS -std=c++20 -c $INCLUDES "$cppfile" -o "$obj_file"
    fi
done

# Compile C files (Fonts and Media Assets)
echo "  Compiling C assets..."
for cfile in "$FIRMWARE_SRC/Fonts"/*.c "$FIRMWARE_SRC/Media"/*.c; do
    [ -e "$cfile" ] || continue
    obj_file="$BUILD_DIR/$(basename "$cfile" .c).o"
    if [ "$cfile" -nt "$obj_file" ]; then
        echo "  Compiling asset: $cfile"
        emcc $COMMON_FLAGS -std=c11 -c $INCLUDES "$cfile" -o "$obj_file"
    fi
done

echo "--- Linking WebAssembly Module ---"

EMCC_LINK_FLAGS=(
    "-s EXPORT_ES6=1"
    "-s EXPORT_NAME=axiomRemoteFirmware"
    "-s EXIT_RUNTIME=0"
    "-s INITIAL_MEMORY=64MB"
    "-s ALLOW_MEMORY_GROWTH=1"
    "-s USE_WEBGL2=1"
    "-s FULL_ES3=1"
    "-s ASSERTIONS=1"
    "-s EXPORTED_FUNCTIONS=['_main']"
    "-s STRICT=1"
    "--bind"
)

emcc $COMMON_FLAGS -std=c++20 \
    $INCLUDES \
    "${EMCC_LINK_FLAGS[@]}" \
    "$BUILD_DIR"/*.o \
    "$LVGL_BUILD/liblvgl.a" \
    -o "$OUTPUT_DIR/axiom_remote_firmware.mjs"

echo "Done!"
ls -lh "$OUTPUT_DIR/axiom_remote_firmware.js" "$OUTPUT_DIR/axiom_remote_firmware.wasm"

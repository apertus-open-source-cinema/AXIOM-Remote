# Change to the path of XC32 installation, default is /opt/microchip/xc32/<version>/
XC32_PATH=/opt/microchip/xc32/v3.00
CC=${XC32_PATH}/bin/xc32-g++
LD=${XC32_PATH}/bin/xc32-ld

NAME=AXIOM_Remote_Firmware

DEVICE=32MZ2048ECG100

LINKER_SCRIPT=procdefs_app.ld

BUILD_DIR=build
INC_DIR=-I${XC32_PATH}/pic32mx/include -I../Common -I../Bootloader

# Bootloader modules are added temporarily, till UA can be flashed through BL
SOURCE_FILES=$(wildcard *.cpp) $(wildcard **/*.cpp) $(wildcard UI/**/*.cpp) $(wildcard ../Bootloader/Periphery/**/*.cpp)
OBJ_FILES=${SOURCE_FILES:%.cpp=${BUILD_DIR}/%.o}

# Compiler options for XC32 3.00
OPTS=-c -O0 -fdiagnostics-color -fstack-usage -Wstack-usage=96 -fdump-tree-optimized -fno-rtti -mprocessor=${DEVICE} -mconfig-data-dir=${XC32_PATH}/pic32mx/lib/proc/${DEVICE} -iprefix ${XC32_PATH}/lib/gcc/pic32mx/8.3.1/ -Wall -MMD -MF ${BUILD_DIR}/.depend
LD_OPTS=-mprocessor=${DEVICE} -Wl,--defsym=_min_heap_size=128,--trace,--script="${LINKER_SCRIPT}",--cref,-Map="${BUILD_DIR}/${NAME}.map",--report-mem,--warn-section-align,--memorysummary=${BUILD_DIR}/memoryfile.xml

all: ${BUILD_DIR}/${NAME}.elf

${BUILD_DIR}/${NAME}.elf: ${OBJ_FILES}
	${CC} ${LD_OPTS} $^ -o $@
	${XC32_PATH}/bin/xc32-bin2hex $@ 

${BUILD_DIR}/%.o: %.cpp
	@ echo ${SOURCE_FILES}
	@ mkdir -p $(dir $@)
	${CC} ${OPTS} $< -o $@ ${INC_DIR}

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}

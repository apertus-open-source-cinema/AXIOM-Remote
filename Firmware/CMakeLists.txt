cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(PICO_PLATFORM rp2040)
set(PICO_BOARD pico)

# Pico SDK
include(${CMAKE_CURRENT_SOURCE_DIR}/pico_sdk_import.cmake)

project(AXIOM_Remote_Firmware C CXX ASM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

pico_sdk_init()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include(CMakeLists.deps.txt)

add_subdirectory(src/UI)

add_executable(${PROJECT_NAME} src/Main.cpp src/drivers/rp2040/RP2040HALDriver.cpp src/drivers/ili9341/ILI9341Display.cpp) #src/drivers/ili9341/ILI9341DisplaySPI.cpp
target_link_libraries(${PROJECT_NAME} PUBLIC pico_stdlib hardware_clocks hardware_pio hardware_dma hardware_spi lvgl FirmwareUI) #$<TARGET_OBJECTS:FirmwareUI>

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/ili9341/pargen.pio)

target_link_options(${PROJECT_NAME} PRIVATE -Wl,--print-memory-usage)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})

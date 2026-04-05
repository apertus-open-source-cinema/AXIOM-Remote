cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

set(PICO_PLATFORM rp2040)
set(PICO_BOARD pico)

# Pico SDK
if(NOT EXISTS PICO_SDK_PATH)
    set(PICO_SDK_FETCH_FROM_GIT TRUE)
    set(PICO_SDK_FETCH_FROM_GIT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty")
endif()
include(${CMAKE_CURRENT_SOURCE_DIR}/pico_sdk_import.cmake)

project(AXIOM_Remote_Firmware C CXX ASM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

pico_sdk_init()

include(CMakeLists.deps.txt)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src/UI)

add_executable(${PROJECT_NAME}
    src/Main.cpp
    src/Application.h
    src/Application.cpp
    src/ErrorHandler.h
    src/ErrorHandler.cpp
    src/ConfigurationManager.h
    src/ConfigurationManager.cpp
    src/EventSystem.h
    src/EventSystem.cpp
    src/Logger.h
    src/Logger.cpp
    src/drivers/rp2040/RP2040HALDriver.cpp
    src/drivers/ili9341/ILI9341Display.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC pico_stdlib hardware_clocks hardware_pio hardware_dma hardware_spi lvgl FirmwareUI) #$<TARGET_OBJECTS:FirmwareUI>

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/ili9341/pargen.pio)

target_link_options(${PROJECT_NAME} PRIVATE -Wl,--print-memory-usage,--gc-sections)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})

CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(AXIOM_Remote_Firmware_Visualizer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

find_package(SDL2 REQUIRED)
# Fix for ArchLinux, see https://bbs.archlinux.org/viewtopic.php?id=235931
if("${SDL2_LIBRARIES}" STREQUAL "")
    SET(SDL2_LIBRARIES SDL2::SDL2)
ELSE()
    string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
ENDIF()
include_directories(${SDL2_INCLUDE_DIRS})

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/..")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/../Firmware/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/../Bootloader/")

MESSAGE(STATUS "PATH_SRC: ${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/3rdParty/imgui/")
FILE(GLOB IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/*.cpp")
list(REMOVE_ITEM IMGUI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/imgui_demo.cpp)

include_directories("${CMAKE_SOURCE_DIR}/3rdParty/gl3w/")
FILE(GLOB GL3W_SOURCES "${CMAKE_SOURCE_DIR}/3rdParty/gl3w/GL/gl3w.c")

FILE(GLOB FIRMWARE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/UI/*.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/UI/Painter/*.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/UI/Screens/*.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/UI/Widgets/*.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/CentralDB/Attribute.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/CentralDB/CentralDB.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/CentralDB/CentralDBObserver.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/GlobalSettings.cpp")
MESSAGE(STATUS "Firmware files: ${FIRMWARE_SOURCES}")

#FILE(GLOB BOOTLOADER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../Bootloader/Periphery/USB/*.cpp")
#MESSAGE(STATUS "Bootloader USB files: ${BOOTLOADER_SOURCES}")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../Bootloader/Periphery/USB/")

SET(VISUALIZER_SOURCES Main.cpp VirtualUI.cpp imgui_modules/imgui_impl_sdl.cpp imgui_modules/imgui_impl_opengl3.cpp)

# Copy image folder to build folder
file(COPY images DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

ADD_EXECUTABLE(${PROJECT_NAME} ${FIRMWARE_SOURCES} ${IMGUI_SOURCES} ${GL3W_SOURCES} ${VISUALIZER_SOURCES}) # ${BOOTLOADER_SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image GL dl)

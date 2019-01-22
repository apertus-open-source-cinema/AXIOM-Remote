CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(axiom_remote_visualizer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

FILE(GLOB AXIOM_REMOTE_SOURCES "${CMAKE_SOURCE_DIR}/../axiom_remote_firmware/draw.c"
                               "${CMAKE_SOURCE_DIR}/../axiom_remote_firmware/page.c"
                               "${CMAKE_SOURCE_DIR}/../axiom_remote_firmware/menu.c"
                               "${CMAKE_SOURCE_DIR}/../axiom_remote_firmware/globals.c")

#"${CMAKE_SOURCE_DIR}/axiom_remote/draw.c"
#                               "${CMAKE_SOURCE_DIR}/axiom_remote/page.c"
#                               "${CMAKE_SOURCE_DIR}/axiom_remote/menu.c"
#                               "${CMAKE_SOURCE_DIR}/axiom_remote/page_wb.c")
#                           "${CMAKE_SOURCE_DIR}/../axiom_remote_firmware/*.c"
#                           "${CMAKE_SOURCE_DIR}/../axiom_remote_firmware/*.h")

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/..")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/../axiom_remote/")

#add_subdirectory("3rdParty/imgui")
#include_directories("3rdParty/imgui/")
#FILE(GLOB IMGUI_SOURCES "3rdParty/imgui/*.h" "3rdParty/imgui/*.cpp")
#list(REMOVE_ITEM IMGUI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/imgui_demo.cpp)

#include_directories("3rdParty/imgui_sdl/")
#FILE(GLOB IMGUI_SDL_SOURCES "3rdParty/imgui_sdl/imgui_sdl.h" "3rdParty/imgui_sdl/imgui_sdl.cpp")

SET(VISUALIZER_SOURCES ./source/Main.cpp)

ADD_EXECUTABLE(${PROJECT_NAME} ${VISUALIZER_SOURCES} ${AXIOM_REMOTE_SOURCES})# ${IMGUI_SOURCES} ${IMGUI_SDL_SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image)

CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
PROJECT(AXIOM_Remote_Firmware_Visualizer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

find_package(SDL2 REQUIRED)
#string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
include_directories(${SDL2_INCLUDE_DIRS})

#FILE(GLOB AXIOM_REMOTE_SOURCES "${CMAKE_SOURCE_DIR}/../AXIOM_Remote_Prototype_V01.X/draw.c"
#                               "${CMAKE_SOURCE_DIR}/../AXIOM_Remote_Prototype_V01.X/page.c"
#                               "${CMAKE_SOURCE_DIR}/../AXIOM_Remote_Prototype_V01.X/page_wb.c"
#                               "${CMAKE_SOURCE_DIR}/../AXIOM_Remote_Prototype_V01.X/page_wb_help.c"
#                               "${CMAKE_SOURCE_DIR}/../AXIOM_Remote_Prototype_V01.X/menu.c"
#                               "${CMAKE_SOURCE_DIR}/../AXIOM_Remote_Prototype_V01.X/globals.c")

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/..")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/../Firmware/")

MESSAGE(STATUS "PATH_SRC: ${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/3rdParty/imgui/")
FILE(GLOB IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/*.cpp")
list(REMOVE_ITEM IMGUI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/imgui_demo.cpp)

include_directories("${CMAKE_SOURCE_DIR}/3rdParty/gl3w/")
FILE(GLOB GL3W_SOURCES "${CMAKE_SOURCE_DIR}/3rdParty/gl3w/GL/gl3w.c")

FILE(GLOB FIRMWARE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/UI/*.cpp")
MESSAGE(STATUS "Firmware files: ${FIRMWARE_SOURCES}")

SET(VISUALIZER_SOURCES Main.cpp VirtualUI.cpp imgui_modules/imgui_impl_sdl.cpp imgui_modules/imgui_impl_opengl3.cpp )

# Copy image folder to build folder
file(COPY images DESTINATION ${CMAKE_BINARY_DIR})

ADD_EXECUTABLE(${PROJECT_NAME} ${VISUALIZER_SOURCES} ${AXIOM_REMOTE_SOURCES} ${IMGUI_SOURCES} ${GL3W_SOURCES} ${FIRMWARE_SOURCES}) #${IMGUI_SDL_SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} SDL2::SDL2 SDL2_image GL)

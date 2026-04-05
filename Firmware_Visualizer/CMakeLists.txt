cmake_minimum_required(VERSION 3.20)
project(AXIOM_Remote_Firmware_Visualizer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)

# Include directories
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        # "${CMAKE_SOURCE_DIR}/../Firmware"
        # "${CMAKE_SOURCE_DIR}/../Bootloader"
        #    "${CMAKE_SOURCE_DIR}/3rdParty/glad_v2/include"
        # "${imgui_SOURCE_DIR}"
        # "${imgui_SOURCE_DIR}/backends"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../Firmware) # For lv_conf.h include in LVGL
include(CMakeLists.deps.txt)

set(VISUALIZER_SOURCES
        Main.cpp
        VirtualUI.cpp
        HelpersGL.cpp
        DesktopGLDriver.cpp
        UIConstants.h
)

# ImGui sources
file(GLOB IMGUI_SOURCES
        "${imgui_SOURCE_DIR}/*.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

add_executable(${PROJECT_NAME}
        ${VISUALIZER_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/gl.c
        ${IMGUI_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
        "${imgui_SOURCE_DIR}"
        "${imgui_SOURCE_DIR}/backends"
)

add_subdirectory("../Firmware/src/UI" "${CMAKE_BINARY_DIR}/FirmwareUI_build")

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        SDL3
        SDL3_image
        GL
        dl
        lvgl
        # $<TARGET_OBJECTS:FirmwareUI>
        FirmwareUI
)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include"
        ${UI_SOURCE_DIR}
)

target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -pedantic
)

# if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#     target_compile_options(${PROJECT_NAME} PRIVATE "-fsanitize=address")
#     target_link_options(${PROJECT_NAME} PRIVATE "-fsanitize=address")
# endif()

# Post-build: Copy images and shaders
file(COPY "${CMAKE_SOURCE_DIR}/images" DESTINATION "${CMAKE_BINARY_DIR}/")
file(COPY "${CMAKE_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_BINARY_DIR}/")

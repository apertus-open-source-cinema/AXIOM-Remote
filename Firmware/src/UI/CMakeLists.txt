add_library(FirmwareUI STATIC
    LvglUI.cpp
    LvglCore.cpp
    Theme.cpp
    Media/ApertusLogo_full.c
    Screens/MainScreen.cpp
    Screens/SettingsMenuScreen.cpp
    ScreenManager.cpp
    AppContext.cpp
    Fonts/titillium_12.c
    Fonts/titillium_18.c
    Fonts/titillium_24.c)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # For lv_conf.h include in LVGL
# include(CMakeLists.deps.txt)

target_link_libraries(FirmwareUI
    PRIVATE
    lvgl
)

target_include_directories(FirmwareUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Media)

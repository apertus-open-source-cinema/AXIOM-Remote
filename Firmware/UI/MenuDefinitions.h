#ifndef MENUDEFINITIONS_H
#define MENUDEFINITIONS_H

#include <stdint.h>

// First entry has value of 0, second is 1 etc.
enum class AvailableScreens : uint8_t
{
    MainPage,
    MainMenu,
    SettingsSubMenu1,
    WhiteBalance
};

#endif // MENUDEFINITIONS_H
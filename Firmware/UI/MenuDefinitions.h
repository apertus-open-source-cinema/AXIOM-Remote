#ifndef MENUDEFINITIONS_H
#define MENUDEFINITIONS_H

#include <stdint.h>

// First entry has value of 0, second is 1 etc.
enum class AvailableScreens : uint8_t
{
    MainPage,
    SettingsMenu,
    WhiteBalance
};

#endif // MENUDEFINITIONS_H
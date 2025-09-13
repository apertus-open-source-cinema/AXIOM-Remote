#pragma once

#include <stdint.h>

// Pins
enum class ILI9341Pins : uint8_t
{
    Reset = 0,
    TE = 1,

    D0 = 2, // D7 = 9

    RDX = 10,
    WRX = 11,

    DCX = 12,
    CS = 13
};

// Specs
enum class ILI9341Specs
{
    Width = 320,
    Height = 240
};
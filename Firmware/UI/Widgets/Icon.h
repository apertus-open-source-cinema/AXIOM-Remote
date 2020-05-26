#ifndef ICON_H
#define ICON_H

#include <inttypes.h>

#define STATIC_SECTION __attribute__((space(prog), section(".kseg0_static_mem")))

struct Icon
{
    uint16_t Width;
    uint16_t Height;
    const uint8_t* Data;

    // Icon(uint16_t width, uint16_t height, const uint8_t* data) : Width(width), Height(height), Data(data)
    // {
    // }
};

#endif // ICON_H

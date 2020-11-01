#ifndef ICON_H
#define ICON_H

#include <inttypes.h>

#define STATIC_SECTION __attribute__((section(".kseg0_static_data")))

struct Icon
{
    const uint16_t Width;
    const uint16_t Height;
    const uint8_t* Data;
};

#endif // ICON_H

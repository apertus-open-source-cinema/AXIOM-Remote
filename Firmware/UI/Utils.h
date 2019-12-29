#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

static inline uint16_t RGB565(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}

#endif //UTILS_H
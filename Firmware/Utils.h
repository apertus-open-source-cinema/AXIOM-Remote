#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define UNUSED(x) (void)(x)

static inline uint16_t RGB565(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}

static inline int8_t LimitRange(int8_t in, int8_t min, int8_t max)
{
    if (in >= max)
    {
        return max;
    }
    if (in <= min)
    {
        return min;
    }
    return in;
}

#endif // UTILS_H
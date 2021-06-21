#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <cstdint>
#include <cstring>

#define UNUSED(x) (void)(x)

namespace utils
{

inline uint16_t RGB565(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}

inline int8_t LimitRange(int8_t in, int8_t min, int8_t max)
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

// TODO: Check against methods shown at https://randomascii.wordpress.com/2013/04/03/stop-using-strncpy-already/
inline void CopyString(char* destination, const char* source, const size_t max_size)
{
    memset(destination, 0, max_size);
    size_t len = strlen(source);
    if (len > max_size)
    {
        len = max_size;
    }
    strncpy(destination, source, len);
}
} // namespace utils

#endif // UTILS_H

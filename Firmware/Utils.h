#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "./UI/FontDefinitions.h"

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

namespace utils
{
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
static inline uint8_t GetTextWidth(const char * text, Font font)
{   
    GFXfont currentFont = fontList[(uint8_t)font].fontGFX;
    uint16_t width = 0;
    uint8_t xo1;
    bool first_letter = true;

    int length = strlen((char*)text);
    uint8_t first = currentFont.first;
    uint8_t last = currentFont.last;

    for (int i = 0; i < length; i++) {
        // Char not present in this font?
        if ((text[i] < first) || (text[i] > last)) {
            continue;
        }

        GFXglyph *glyph = &currentFont.glyph[text[i] - first];
        uint8_t xa = glyph -> xAdvance;
        int8_t xo = glyph -> xOffset;
        width += xa;

        if (first_letter) {
            xo1 = xo;
            first_letter = false;
        }
    }

    if (width > 0) {
        width -= xo1;
    }
    return width;
}
} // namespace utils

#endif // UTILS_H

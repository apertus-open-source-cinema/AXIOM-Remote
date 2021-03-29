#include "Utils.h"
#include "UI/FontDefinitions.h"

namespace utils {

inline uint8_t GetTextWidth(const char * text, Font font)
{   
    GFXfont currentFont = fontList[(uint8_t)font].fontGFX;
    uint16_t width = 0;
    uint8_t gap;
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
        uint8_t gw = glyph -> width;
        uint8_t xa = glyph -> xAdvance;
        int8_t xo = glyph -> xOffset;
        gap = xa - gw - xo;
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
}


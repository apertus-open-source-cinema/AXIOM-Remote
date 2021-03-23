#include "Utils.h"
#include "UI/GFXFont.h"
#include "UI/FontDefinitions.h"

#include "./Media/Fonts/FreeSans9pt7b.h"
#include "./Media/Fonts/FreeSans12pt7b.h"
#include "./Media/Fonts/FreeSans18pt7b.h"
#include "./Media/Fonts/FreeSans24pt7b.h"


namespace utils {

inline GFXfont currentFont;
inline uint8_t currentFontHeight;
inline Font curFont;
inline GFXfont fontList[4] = {
    FreeSans9pt7b,
    FreeSans12pt7b,
    FreeSans18pt7b,
    FreeSans24pt7b
};

inline void SetFont(Font font) {
    curFont = font;
    switch (font) {
    case Font::FreeSans9pt7b:
        currentFont = fontList[0];
        currentFontHeight = 13;
        break;
    case Font::FreeSans12pt7b:
        currentFont = fontList[1];
        currentFontHeight = 17;
        break;
    case Font::FreeSans18pt7b:
        currentFont = fontList[2];
        currentFontHeight = 25;
        break;
    case Font::FreeSans24pt7b:
        currentFont = fontList[3];
        currentFontHeight = 33;
        break;
    }
}

inline uint8_t GetTextWidth(const char * text, Font font = curFont)
{
    SetFont(font);
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


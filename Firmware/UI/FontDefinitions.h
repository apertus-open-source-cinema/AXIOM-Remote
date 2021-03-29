#ifndef FONTDEFINITIONS_H
#define FONTDEFINITIONS_H

#include "UI/GFXFont.h"

#include "./Media/Fonts/FreeSans9pt7b.h"
#include "./Media/Fonts/FreeSans12pt7b.h"
#include "./Media/Fonts/FreeSans18pt7b.h"
#include "./Media/Fonts/FreeSans24pt7b.h"

enum class Font
{
    FreeSans9pt7b = 0,
    FreeSans12pt7b,
    FreeSans18pt7b,
    FreeSans24pt7b
};

enum class TextAlign
{
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
};

struct FontInfo{
    GFXfont fontGFX;
    uint8_t fontHeight;
};

inline FontInfo fontList[4] = {
    { FreeSans9pt7b, 13 },
    { FreeSans12pt7b, 17 },
    { FreeSans18pt7b, 25 },
    { FreeSans24pt7b, 33 }
};

#endif //FONTDEFINITIONS_H
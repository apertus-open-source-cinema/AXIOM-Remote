#include "Painter.h"

#include <cstring>
#include <cstdlib>

#include "../../Utils.h"

#include "../../GlobalSettings.h"

#include "../../Media/Fonts/FreeSans9pt7b.h"
#include "../../Media/Fonts/FreeSans12pt7b.h"
#include "../../Media/Fonts/FreeSans18pt7b.h"
#include "../../Media/Fonts/FreeSans24pt7b.h"

#define _swap_int16_t(a, b)                                                                                            \
    {                                                                                                                  \
        int16_t t = a;                                                                                                 \
        a = b;                                                                                                         \
        b = t;                                                                                                         \
    }

Painter::Painter(volatile uint16_t* framebuffer, uint16_t framebufferWidth, uint8_t framebufferHeight) :
    _framebuffer(framebuffer), _framebufferWidth(framebufferWidth),
    _framebufferHeight(framebufferHeight), _fontList{FreeSans9pt7b, FreeSans12pt7b, FreeSans18pt7b, FreeSans24pt7b},
    _cursorX(0), _cursorY(0), _debugPainter(nullptr)
{
    // Default font
    SetFont(Font::FreeSans9pt7b);
}

void Painter::SetDebugOverlay(IDebugPainter* debugPainter)
{
    _debugPainter = debugPainter;
}

void Painter::SetFont(Font font)
{
    switch (font)
    {
    case Font::FreeSans9pt7b:
        _currentFont = _fontList[0];
        _currentFontHeight = 13;
        break;
    case Font::FreeSans12pt7b:
        _currentFont = _fontList[1];
        _currentFontHeight = 17;
        break;
    case Font::FreeSans18pt7b:
        _currentFont = _fontList[2];
        _currentFontHeight = 25;
        break;
    case Font::FreeSans24pt7b:
        _currentFont = _fontList[3];
        _currentFontHeight = 33;
        break;
    }
}

void Painter::DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    // int16_t steep = (y1 - y0) > (x1 - x0);
    if (steep)
    {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1)
    {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);
    // dy = (y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1)
    {
        ystep = 1;
    } else
    {
        ystep = -1;
    }

    for (; x0 <= x1; x0++)
    {
        if (steep)
        {
            DrawPixel(y0, x0, color);
        } else
        {
            DrawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

void Painter::DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    DrawFillRectangle(x, y, 1, h, color);
}

void Painter::DrawFastHLine(int16_t x, int16_t y, int16_t l, uint16_t color)
{
    DrawFillRectangle(x, y, l, 1, color);
}

void Painter::DrawFillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    for (uint16_t yIndex = y; yIndex < y + height; yIndex++)
    {
        for (uint16_t xIndex = x; xIndex < x + width; xIndex++)
        {
            DrawPixel(xIndex, yIndex, color);
        }
    }
}

void Painter::DrawFillRoundRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t radius,
                                     uint16_t color)
{

    DrawFillRectangle(x + radius, y, width - 2 * radius, height, color);

    // draw four corners
    DrawFillCircleQuarter(x + width - radius - 1, y + radius, radius, 1, height - 2 * radius - 1, color);
    DrawFillCircleQuarter(x + radius, y + radius, radius, 2, height - 2 * radius - 1, color);
}

void Painter::DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    // Draw top and bottom line
    DrawFastHLine(x, y, width, color);
    DrawFastHLine(x, y + height - 1, width, color);

    // Draw left and right line
    DrawFastVLine(x, y, height, color);
    DrawFastVLine(x + width - 1, y, height, color);
}

void Painter::DrawStripedRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t firstColor,
                                   uint16_t secondColor, uint8_t tilt, uint8_t offset)
{
    int16_t stripeIndex = 0;
    uint16_t drawColor = firstColor;

    for (uint16_t yIndex = y; yIndex < y + height; yIndex++)
    {
        stripeIndex -= tilt;

        for (uint16_t xIndex = x; xIndex < x + width; xIndex++)
        {
            stripeIndex++;
            if (stripeIndex > offset)
            {
                drawColor = (drawColor == firstColor) ? secondColor : firstColor;

                stripeIndex = 0;
            }

            DrawPixel(xIndex, yIndex, drawColor);
        }
    }
}

void Painter::DrawCircle(int16_t x, int16_t y, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x0 = 0;
    int16_t y0 = r;

    DrawPixel(x, y + r, color);
    DrawPixel(x, y - r, color);
    DrawPixel(x + r, y, color);
    DrawPixel(x - r, y, color);

    while (x0 < y0)
    {
        if (f >= 0)
        {
            y0--;
            ddF_y += 2;
            f += ddF_y;
        }
        x0++;
        ddF_x += 2;
        f += ddF_x;

        DrawPixel(x + x0, y + y0, color);
        DrawPixel(x - x0, y + y0, color);
        DrawPixel(x + x0, y - y0, color);
        DrawPixel(x - x0, y - y0, color);
        DrawPixel(x + y0, y + x0, color);
        DrawPixel(x - y0, y + x0, color);
        DrawPixel(x + y0, y - x0, color);
        DrawPixel(x - y0, y - x0, color);
    }
}

void Painter::DrawFillCircle(int16_t x, int16_t y, int16_t r, uint16_t color)
{
    DrawFastVLine(x, y - r, 2 * r + 1, color);
    DrawFillCircleQuarter(x, y, r, 3, 0, color);
}

void Painter::DrawCircleQuarter(int16_t x, int16_t y, int16_t r, uint8_t cornername, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x0 = 0;
    int16_t y0 = r;

    while (x0 < y0)
    {
        if (f >= 0)
        {
            y0--;
            ddF_y += 2;
            f += ddF_y;
        }
        x0++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4)
        {
            DrawPixel(x + x0, y + y0, color);
            DrawPixel(x + y0, y + x0, color);
        }
        if (cornername & 0x2)
        {
            DrawPixel(x + x0, y - y0, color);
            DrawPixel(x + y0, y - x0, color);
        }
        if (cornername & 0x8)
        {
            DrawPixel(x - y0, y + x0, color);
            DrawPixel(x - x0, y + y0, color);
        }
        if (cornername & 0x1)
        {
            DrawPixel(x - y0, y - x0, color);
            DrawPixel(x - x0, y - y0, color);
        }
    }
}

void Painter::DrawFillCircleQuarter(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta,
                                    uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1)
        {
            DrawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            DrawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x2)
        {
            DrawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            DrawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }
}

void Painter::DrawImage(const uint8_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    for (uint16_t yIndex = 0; yIndex < height; yIndex++)
    {
        for (uint16_t xIndex = 0; xIndex < width; xIndex++)
        {
            uint16_t index = ((width * yIndex) + xIndex) * 2;

            uint8_t value1 = data[index];
            uint8_t value2 = data[index + 1];

            uint16_t color = ((uint16_t)value1 << 8) | value2;
            DrawPixel(x + xIndex, y + yIndex, color);
        }
    }
}

/*************** Draw Icon ***************/

void Painter::DrawIcon(const Icon* image, uint16_t x, uint16_t y, uint16_t color)
{
    int b = 0;

    for (uint16_t yIndex = 0; yIndex < image->Height; yIndex++)
    {
        uint16_t yPos = y + yIndex;

        for (uint16_t xIndex = 0; xIndex < image->Width;)
        {
            xIndex = ProcessByte(image->Data[b], x, xIndex, yPos, image->Height, color);
            b++;
        }
    }

    if (_debugPainter != nullptr)
    {
        _debugPainter->DrawIcon(this, image, x, y, color);
    }
}

/*****************************************/

// uint8_t count = 0;

void Painter::DrawText(uint16_t x, uint16_t y, const char* text, uint16_t color, TextAlign align,
                       uint16_t textblockwidth)
{
    uint8_t first = _currentFont.first;
    uint8_t last = _currentFont.last;
    uint8_t length = strlen(text);
    bool newline = false;

    /*char debug[32];
    sprintf(debug, "length = %d", length);
    debug_uart(text);
    debug_uart(debug);*/

    _cursorX = x;
    _cursorY = y;

    // if (count < 3)
    //{

    //}

    uint16_t textPixelWidth = GetStringFramebufferWidth(text);

    // DrawFillRectangle(x, 0, textPixelWidth, 10, RGB565(255, 255, 0));
    // count++;

    GFXglyph* glyph = nullptr;

    for (uint8_t i = 0; i < length; i++)
    {
        char c = text[i];

        if (c == 32)
        { // space " " character
            glyph = &_currentFont.glyph[32 - first];
            _cursorX += glyph->xAdvance; //(uint8_t) pgm_read_byte(&glyph->xAdvance);
            newline = false;

            // wrap text into new line: - check at every space character if next word will
            // still fit into textblockwidth if not advance to next line
            if ((align == TextAlign::TEXT_ALIGN_LEFT) && (textblockwidth > 0))
            {
                uint16_t next_space = 0;
                uint16_t j;

                // find out where the next space or end of string is
                for (j = 1; j < (length - i + 1); j++)
                {

                    /*char debug5[32];
                    sprintf(debug5, "text[%u + %u] = %c (%d)", i, j, text[i + j], text[i + j]);
                    debug_uart(debug5);*/

                    if ((text[i + j] == 32) || (text[i + j] == 0))
                    {
                        next_space = j;

                        /*char debug2[32];
                        sprintf(debug2, "next_space = %u", next_space);
                        debug_uart(debug2);*/

                        break;
                    }
                }

                // measure the width of the next word in pixels
                uint16_t next_wordframebuffer_width = 0;
                for (j = 1; j <= next_space - 1; j++)
                {
                    glyph = &_currentFont.glyph[text[i + j] - first];
                    next_wordframebuffer_width += glyph->xAdvance;
                }

                // does the width of the next word already go outside the textblockwidth ?
                //                if ((_cursorX + next_wordframebuffer_width) > (x + textblockwidth))
                //                {
                //
                //                    //debug
                //                    draw_line(_cursorX, _cursorY - 3, _cursorX + next_wordframebuffer_width, _cursorY
                //                    - 3, color);
                //
                //                    /*char debug3[32];
                //                    sprintf(debug3, "next_wordframebuffer_width = %u", next_wordframebuffer_width);
                //                    debug_uart(debug3);*/
                //
                //
                //                    _cursorX = x;
                //                    _cursorY -= _currentFont.yAdvance;
                //                    newline = true;
                //
                //                }
            }
        } else

            if (c == 10)
        { // "\n" (LF) line feed - new line character
            _cursorX = x;
            _cursorY += _currentFont.yAdvance;
            newline = true;
        } else if (c == 13)
        { // "\r" (CR) carriage return character
            _cursorX = x;
            _cursorY += _currentFont.yAdvance;
            newline = true;
        } else if ((c >= first) && (c <= last))
        {
            glyph = &_currentFont.glyph[c - first];
            uint8_t w = glyph->width, h = glyph->height;
            if ((w > 0) && (h > 0))
            { // Is there an associated bitmap?
                int16_t xo = glyph->xOffset;

                if ((i == 0) || newline)
                {
                    // first character doesn't need x offset
                    _cursorX -= xo;
                    newline = false;
                }

                if (align == TextAlign::TEXT_ALIGN_LEFT)
                {
                    DrawCharacter(c, _cursorX + xo, _cursorY, color);
                }
                if ((align == TextAlign::TEXT_ALIGN_CENTER) && (textblockwidth > 0))
                {
                    DrawCharacter(c, _cursorX + xo - textPixelWidth / 2 + textblockwidth / 2, _cursorY, color);
                }
                if ((align == TextAlign::TEXT_ALIGN_RIGHT) && (textblockwidth > 0))
                {
                    DrawCharacter(c, _cursorX + xo + textblockwidth - textPixelWidth, _cursorY, color);
                }

                _cursorX += glyph->xAdvance;
            }
        }

        //        if (c == '\n')
        //        { // Newline?
        //            _cursorX = 0; // Reset x to zero,
        //            _cursorY += size * 8; // advance y one line
        //        }
        //        else if (c != '\r')
        //        { // Ignore carriage returns
        //            if (wrap && ((_cursorX + size * 6) > _framebufferWidth))
        //            { // Off right?
        //                _cursorX = 0; // Reset x to zero,
        //                _cursorY += size * 8; // advance y one line
        //            }
        //            DrawCharacter(c, _cursorX, _cursorY, color);
        //            _cursorX += size * 6; // Advance x one char
        //        }

        if (_debugPainter != nullptr)
        {
            _debugPainter->DrawText(this, x, y, text, color, align, textblockwidth);
        }
    }
}

void Painter::DrawCharacter(unsigned char character, int16_t x, int16_t y, uint16_t color)
{
    character -= _currentFont.first;
    GFXglyph* glyph = &_currentFont.glyph[character];
    uint8_t* bitmap = _currentFont.bitmap;

    uint16_t bo = glyph->bitmapOffset;
    uint8_t glyphWidth = glyph->width;
    uint8_t glyphHeight = glyph->height;
    int8_t xo = glyph->xOffset;
    int8_t yo = glyph->yOffset;
    uint8_t bits = 0, bit = 0;
    //    int16_t xo16 = 0, yo16 = 0;

    // Todo: Add character clipping here

    for (uint8_t glyphY = 0; glyphY < glyphHeight; glyphY++)
    {
        for (uint8_t glyphX = 0; glyphX < glyphWidth; glyphX++)
        {
            if (!(bit++ & 7))
            {
                bits = bitmap[bo++];
            }
            if (bits & 0x80)
            {
                DrawPixel(x + xo + glyphX, y + yo + glyphY, color);
            }
            bits <<= 1;
        }
    }
}

uint8_t Painter::GetCurrentFontHeight()
{
    return _currentFontHeight;
}

uint16_t Painter::GetStringFramebufferWidth(const char* str)
{
    uint16_t width = 0;
    uint8_t gap;
    uint8_t xo1;
    bool first_letter = true;

    int length = strlen(str);
    // std::cout << "Text length: " << str << " " << length << std::endl;
    uint8_t first = _currentFont.first;
    uint8_t last = _currentFont.last;

    for (int i = 0; i < length; i++)
    {
        // Char not present in this font?
        if ((str[i] < first) && (str[0] > last))
        {
            continue;
        }

        GFXglyph* glyph = &_currentFont.glyph[str[i] - first];
        uint8_t gw = glyph->width;
        uint8_t xa = glyph->xAdvance;
        int8_t xo = glyph->xOffset;
        gap = xa - gw - xo;
        width += xa;
        // std::cout << "Glyph width: " << str << (uint16_t)xa << std::endl;

        if (first_letter)
        {
            xo1 = xo;
            first_letter = false;
        }

        //(*str)++;
    }

    // remove the gap after the last letter
    /*if (width > 0)
    {
        width -= gap;
    }*/

    // remove the xoffset before the first letter
    if (width > 0)
    {
        width -= xo1;
    }
    // std::cout << "Glyph width: " << str << (uint16_t)width << std::endl;
    return width;
}

void Painter::DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    // Prevent drawing outside of bounds
    if (x < _framebufferWidth && y < _framebufferHeight)
    {
        _framebuffer[y * _framebufferWidth + x] = color;
    }
}

void Painter::Fill(uint16_t fillColor)
{
    DrawFillRectangle(0, 0, _framebufferWidth, _framebufferHeight, fillColor);
}

void Painter::Dim()
{
    for (int i = 0; i < _framebufferWidth * _framebufferHeight; i++)
    {
        // half the brightness: set LSB of each color schannel 0 and shiftright once
        _framebuffer[i] = (_framebuffer[i] & 0xF7DE) >> 1;
    }
}

uint16_t Painter::ProcessByte(uint8_t data, uint16_t x, uint16_t xIndex, uint16_t yPos, uint16_t height, uint16_t color)
{
    for (int i = 0; i < 8; i++)
    {
        uint8_t pixel = (data >> i) & 0x01;

        if (pixel)
        {
            DrawPixel(x + xIndex, yPos, color);
        }

        xIndex++;
    }

    return xIndex;
}

#ifndef PAINTER_H
#define PAINTER_H

#include <stdint.h>

#include "GFXFont.h"

#include "Color565.h"

enum class Font
{
    FreeSans9pt7b = 1,
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

class Painter
{
    uint16_t _framebufferWidth;
    uint16_t _framebufferHeight;

    volatile uint16_t* _framebuffer;

    GFXfont _fontList[4];
    GFXfont _currentFont;

    uint16_t _cursorX;
    uint16_t _cursorY;

  public:
    Painter(volatile uint16_t* framebuffer, uint16_t framebufferWidth, uint8_t framebufferHeight);

    // Primitives
    void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void DrawFastHLine(int16_t x, int16_t y, int16_t l, uint16_t color);
    void DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
    void DrawFillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
    void DrawFillRoundRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t radius,
                                uint16_t color);
    void DrawCirlce(int16_t x, int16_t y, int16_t r, uint16_t color);
    void DrawFillCirlce(int16_t x, int16_t y, int16_t r, uint16_t color);
    void DrawCircleQuarter(int16_t x, int16_t y, int16_t r, uint8_t cornername, uint16_t color);
    void DrawFillCircleQuarter(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

    // Misc
    void DrawImage(const uint8_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    void DrawPixel(uint16_t x, uint16_t y, uint16_t color);

    void Fill(uint16_t fillColor);

    // Text Related
    // void SetFont(Font font);
    void DrawCharacter(unsigned char c, int16_t x, int16_t y, uint16_t color);
    void DrawText(uint16_t x, uint16_t y, const char* text, uint16_t color, Font font, TextAlign align,
                  uint16_t textblockwidth);
    uint16_t GetStringFramebufferWidth(const char* str);
};

#endif /* PAINTER_H */

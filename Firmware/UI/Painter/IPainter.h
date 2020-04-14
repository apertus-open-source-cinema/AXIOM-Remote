#ifndef IPAINTER_H
#define IPAINTER_H

#include <stdint.h>

#include "../FontDefinitions.h"

class IPainter
{
  public:
    // IPainter(volatile uint16_t* framebuffer, uint16_t framebufferWidth, uint8_t framebufferHeight) :
    //     _framebuffer(framebuffer), _framebufferWidth(framebufferWidth), _framebufferHeight(framebufferHeight)
    // {
    // }

    // Primitives
    virtual void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {}
    virtual void DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {}
    virtual void DrawFastHLine(int16_t x, int16_t y, int16_t l, uint16_t color) {}

    // Rectangle
    virtual void DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {}
    virtual void DrawFillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {}
    virtual void DrawFillRoundRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t radius,
                                        uint16_t color) {}
    virtual void DrawStripedRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t firstColor,
                                      uint16_t secondColor, uint8_t tilt, uint8_t offset) {}

    // Circle
    virtual void DrawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {}
    virtual void DrawFillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {}
    virtual void DrawCircleQuarter(int16_t x, int16_t y, int16_t r, uint8_t cornername, uint16_t color) {}
    virtual void DrawFillCircleQuarter(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta,
                                       uint16_t color) {}

    // Misc
    virtual void DrawImage(const uint8_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {}
    virtual void DrawPixel(uint16_t x, uint16_t y, uint16_t color) {}
    virtual void DrawIcon(const uint8_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                          uint16_t color) {}
    virtual void Fill(uint16_t fillColor) {}

    // Text Related
    virtual void SetFont(Font font) {}
    virtual void DrawCharacter(unsigned char c, int16_t x, int16_t y, uint16_t color) {}
    virtual void DrawText(uint16_t x, uint16_t y, const char* text, uint16_t color, TextAlign align,
                          uint16_t textblockwidth) {}
    virtual uint16_t GetStringFramebufferWidth(const char* str) {}
    virtual uint8_t GetCurrentFontHeight() {}
};

#endif /* PAINTER_H */

#ifndef IPAINTER_H
#define IPAINTER_H

#include <stdint.h>

#include "../FontDefinitions.h"

#include "../Widgets/Icon.h"

class IDebugPainter;

class IPainter
{
  public:
    // IPainter(volatile uint16_t* framebuffer, uint16_t framebufferWidth, uint8_t framebufferHeight) :
    //     _framebuffer(framebuffer), _framebufferWidth(framebufferWidth), _framebufferHeight(framebufferHeight)
    // {
    // }

    void SetDebugOverlay(IDebugPainter* debugPainter);

    // Primitives
    virtual void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) = 0;
    virtual void DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) = 0;
    virtual void DrawFastHLine(int16_t x, int16_t y, int16_t l, uint16_t color) = 0;

    // Rectangle
    virtual void DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) = 0;
    virtual void DrawFillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) = 0;
    virtual void DrawFillRoundRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t radius,
                                        uint16_t color) = 0;
    virtual void DrawStripedRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t firstColor,
                                      uint16_t secondColor, uint8_t tilt, uint8_t offset) = 0;

    // Circle
    virtual void DrawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) = 0;
    virtual void DrawFillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) = 0;
    virtual void DrawCircleQuarter(int16_t x, int16_t y, int16_t r, uint8_t cornername, uint16_t color) = 0;
    virtual void DrawFillCircleQuarter(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta,
                                       uint16_t color) = 0;

    // Misc
    virtual void DrawImage(const uint8_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height) = 0;
    virtual void DrawPixel(uint16_t x, uint16_t y, uint16_t color) = 0;
    virtual void DrawIcon(const Icon* image, uint16_t x, uint16_t y, uint16_t color) = 0;
    virtual void Fill(uint16_t fillColor) = 0;
    virtual void Dim() = 0;

    // Text Related
    virtual void SetFont(Font font) = 0;
    virtual void DrawCharacter(unsigned char c, int16_t x, int16_t y, uint16_t color) = 0;
    virtual void DrawText(uint16_t x, uint16_t y, const char* text, uint16_t color, TextAlign align,
                          uint16_t textblockwidth) = 0;
    virtual uint16_t GetStringFramebufferWidth(const char* str) = 0;
    virtual uint8_t GetCurrentFontHeight() = 0;
};

#endif /* PAINTER_H */

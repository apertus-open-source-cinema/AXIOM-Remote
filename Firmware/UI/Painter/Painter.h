#ifndef PAINTER_H
#define PAINTER_H

#include <stdint.h>

#include "IPainter.h"
#include "IDebugPainter.h"

#include "../GFXFont.h"
#include "../FontDefinitions.h"

#include "../Color565.h"

#include "../Widgets/Icon.h"

class Painter : public IPainter
{
    GFXfont _fontList[4];
    GFXfont _currentFont;
    uint8_t _currentFontHeight;

    uint16_t _cursorX;
    uint16_t _cursorY;

    IDebugPainter* _debugPainter;

  protected:
    volatile uint16_t* _framebuffer;
    uint16_t _framebufferWidth;
    uint16_t _framebufferHeight;

  public:
    Painter(volatile uint16_t* framebuffer, uint16_t framebufferWidth, uint8_t framebufferHeight);

    // Debugging
    void SetDebugOverlay(IDebugPainter* debugPainter);

    // Primitives
    void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) override;
    void DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
    void DrawFastHLine(int16_t x, int16_t y, int16_t l, uint16_t color) override;

    // Rectangle
    void DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) override;
    void DrawFillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) override;
    void DrawFillRoundRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t radius,
                                uint16_t color) override;
    void DrawStripedRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t firstColor,
                              uint16_t secondColor, uint8_t tilt, uint8_t offset) override;

    // Circle
    void DrawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) override;
    void DrawFillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) override;
    void DrawCircleQuarter(int16_t x, int16_t y, int16_t r, uint8_t cornername, uint16_t color) override;
    void DrawFillCircleQuarter(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta,
                               uint16_t color) override;

    // Misc
    void DrawImage(const uint8_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height) override;
    void DrawPixel(uint16_t x, uint16_t y, uint16_t color) override;
    virtual void DrawIcon(const Icon* image, uint16_t x, uint16_t y, uint16_t color) override;
    uint16_t ProcessByte(uint8_t data, uint16_t x, uint16_t xIndex, uint16_t yPos, uint16_t height, uint16_t color);
    void Fill(uint16_t fillColor) override;
    void Dim() override;

    // Text Related
    void SetFont(Font font) override;
    void DrawCharacter(unsigned char c, int16_t x, int16_t y, uint16_t color) override;
    virtual void DrawText(uint16_t x, uint16_t y, const char* text, uint16_t color, TextAlign align,
                          uint16_t textblockwidth) override;
    uint16_t GetStringFramebufferWidth(const char* str) override;
    uint8_t GetCurrentFontHeight() override;
};

#endif /* PAINTER_H */

#ifndef DEBUGPAINTER_H
#define DEBUGPAINTER_H

#include <stdint.h>

#include "PainterDecorator.h"

// template <class T>
class DebugPainter : public Painter
{
    Painter* _painter;

  public:
    DebugPainter(Painter* painter) : Painter(*painter), _painter(painter)
    {
    }

    void DrawIcon(const uint8_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) override
    {
        _painter->DrawIcon(data, x, y, width, height, color);
        _painter->DrawRectangle(x, y, width, height, (uint16_t)Color565::Red);
    }

    void DrawText(uint16_t x, uint16_t y, const char* text, uint16_t color, TextAlign align,
                  uint16_t textBlockWidth) override
    {
        _painter->DrawText(x, y, text, color, align, textBlockWidth);

        uint16_t textWidth = _painter->GetStringFramebufferWidth(text);
        uint16_t textHeight = GetCurrentFontHeight();

        int16_t xOffset = 0;

        if ((align == TextAlign::TEXT_ALIGN_CENTER) && (textBlockWidth > 0))
        {
            xOffset = -textWidth / 2 + textBlockWidth / 2;
        }

        uint8_t boundingBoxYOffset = 0;
        if (y > textHeight)
        {
            boundingBoxYOffset = y - textHeight;
        }

        // boundingBoxYOffset += 2;
        //        x -= 1;
        //      textWidth += 2;
        // xOffset += 3;

        // draw bounding box
        _painter->DrawRectangle(x + xOffset, boundingBoxYOffset, textWidth, textHeight, (uint16_t)Color565::Red);

        // draw text baseline
        _painter->DrawLine(x + xOffset, y, x + xOffset + textWidth, y, (uint16_t)Color565::Green);

        // std::cout << "Text: " <<  text << " | x: " << x << " y: " << y << " width: " << textWidth << " height: " <<
        // textHeight << std::endl;
    }
};

#endif // DEBUGPAINTER_H

#ifndef DEBUGPAINTER_H
#define DEBUGPAINTER_H

#include <stdint.h>

#include "IDebugPainter.h"

class DebugPainter : public IDebugPainter
{
    bool _enabled = false;

  public:
    DebugPainter()
    {
    }

    void SetEnable(bool value)
    {
        _enabled = value;
    }

    void DrawIcon(IPainter* painter, const Icon* image, uint16_t x, uint16_t y, uint16_t color) override
    {
        if (!_enabled)
        {
            return;
        }

        painter->DrawRectangle(x, y, image->Width, image->Height, (uint16_t)Color565::Red);
    }

    void DrawText(IPainter* painter, uint16_t x, uint16_t y, const char* text, uint16_t color, TextAlign align,
                  uint16_t textBlockWidth) override
    {
        if (!_enabled)
        {
            return;
        }

        uint16_t textWidth = painter->GetStringFramebufferWidth(text);
        uint16_t textHeight = painter->GetCurrentFontHeight();

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
        painter->DrawRectangle(x + xOffset, boundingBoxYOffset, textWidth, textHeight, (uint16_t)Color565::Red);

        // draw text baseline
        painter->DrawLine(x + xOffset, y, x + xOffset + textWidth, y, (uint16_t)Color565::Green);

        // std::cout << "Text: " <<  text << " | x: " << x << " y: " << y << " width: " << textWidth << " height: " <<
        // textHeight << std::endl;
    }
};

#endif // DEBUGPAINTER_H

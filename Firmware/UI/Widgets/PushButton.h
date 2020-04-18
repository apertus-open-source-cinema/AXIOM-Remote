#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <cstring>

#include "IButton.h"
#include "../Painter/Painter.h"

class PushButton : public IButton
{
    uint8_t _cornerRadius;
    const char* _caption;

  public:
    PushButton(const char* caption, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IButton(x, y, width, height), _caption(caption), _cornerRadius(5)
    {
    }

    void SetCornerRadius(uint8_t cornerRadius)
    {
        _cornerRadius = cornerRadius;
    }

    virtual void Draw(IPainter* painter)
    {
        painter->DrawFillRoundRectangle(_x, _y, _width, _height, _cornerRadius, (uint16_t)Color565::White);

        uint8_t textPosY = _height / 2 + painter->GetCurrentFontHeight() / 2;
        painter->DrawText(_x + _width / 2, _y + textPosY, _caption, (uint16_t)Color565::Black, TextAlign::TEXT_ALIGN_CENTER,
                          strlen(_caption));
    }
};

#endif // PUSHBUTTON_H
#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <cstring>

#include "IButton.h"
#include "../Painter/Painter.h"

class PushButton : public IButton
{
    uint8_t _cornerRadius;
    const char* _label;
    bool _hightlighted;

    // Color Defintions
    uint16_t _TextColor;
    uint16_t _BackgroundColor;

    uint16_t _textHighlightColor;
    uint16_t _backgroundHighlightColor;

    uint16_t _currentTextColor;
    uint16_t _currentBackgroundColor;

  public:
    PushButton(const char* caption, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IButton(x, y, width, height), _label(caption), _cornerRadius(3), _hightlighted(false)
    {
        _currentTextColor = _TextColor = (uint16_t)Color565::Black;
        _currentBackgroundColor = _BackgroundColor = RGB565(220, 220, 220);

        _backgroundHighlightColor = (uint16_t)Color565::AXIOM_Blue;
        _textHighlightColor = (uint16_t)Color565::Black;
    }

    void SetCornerRadius(uint8_t cornerRadius)
    {
        _cornerRadius = cornerRadius;
    }

    virtual void Draw(IPainter* painter)
    {
        painter->DrawFillRoundRectangle(_x, _y, _width, _height, _cornerRadius, _currentBackgroundColor);

        painter->SetFont(Font::FreeSans12pt7b);
        uint8_t textPosY = _height / 2 + painter->GetCurrentFontHeight() / 2;
        painter->DrawText(_x + _width / 2, _y + textPosY, _label, _currentTextColor, TextAlign::TEXT_ALIGN_CENTER,
                          strlen(_label));
    }

    void SetBackgroundColor(uint16_t color)
    {
        _BackgroundColor = color;
        SetHighlighted(_hightlighted);
    }

    void SetTextColor(uint16_t color)
    {
        _TextColor = color;
        SetHighlighted(_hightlighted);
    }

    void SetHighlightBackgroundColor(uint16_t color)
    {
        _backgroundHighlightColor = color;
        SetHighlighted(_hightlighted);
    }

    void SetHighlightTextColor(uint16_t color)
    {
        _textHighlightColor = color;
        SetHighlighted(_hightlighted);
    }

    void SetHighlighted(bool hightlighted)
    {
        _hightlighted = hightlighted;
        if (hightlighted)
        {
            _currentTextColor = _textHighlightColor;
            _currentBackgroundColor = _backgroundHighlightColor;

        } else
        {
            _currentTextColor = _TextColor;
            _currentBackgroundColor = _BackgroundColor;
        }
    };
};

#endif // PUSHBUTTON_H
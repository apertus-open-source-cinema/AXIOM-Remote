#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <cstring>

#include "IButton.h"
#include "../Painter/Painter.h"

class PushButton : public IButton
{
    const char* _label;
    uint8_t _cornerRadius;
    bool _highlighted;

    // Color Defintions
    uint16_t _TextColor;
    uint16_t _BackgroundColor;

    uint16_t _textHighlightColor;
    uint16_t _backgroundHighlightColor;

    uint16_t _currentTextColor;
    uint16_t _currentBackgroundColor;

  public:
    explicit PushButton(const char* label, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IButton(x, y, width, height), _label(label), _cornerRadius(3), _highlighted(false)
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

    void SetLabel(const char* label)
    {
        _label = label;
    }

    virtual void Draw(IPainter* painter) override
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
        SetHighlighted(_highlighted);
    }

    void SetTextColor(uint16_t color)
    {
        _TextColor = color;
        SetHighlighted(_highlighted);
    }

    void SetHighlightBackgroundColor(uint16_t color)
    {
        _backgroundHighlightColor = color;
        SetHighlighted(_highlighted);
    }

    void SetHighlightTextColor(uint16_t color)
    {
        _textHighlightColor = color;
        SetHighlighted(_highlighted);
    }

    void SetHighlighted(bool highlighted)
    {
        _highlighted = highlighted;
        if (highlighted)
        {
            _currentTextColor = _textHighlightColor;
            _currentBackgroundColor = _backgroundHighlightColor;

        } else
        {
            _currentTextColor = _TextColor;
            _currentBackgroundColor = _BackgroundColor;
        }
    }
};

#endif // PUSHBUTTON_H

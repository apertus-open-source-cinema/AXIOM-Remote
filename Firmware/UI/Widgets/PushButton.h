#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <cstring>

#include "IButton.h"
#include "ButtonState.h"
#include "../Painter/Painter.h"

class PushButton : public IButton
{
    const char* _label;
    uint8_t _cornerRadius;
    ButtonState _state;

    // Color Defintions
    uint16_t _textColor;
    uint16_t _backgroundColor;

    uint16_t _textHighlightColor;
    uint16_t _backgroundHighlightColor;

    uint16_t _currentTextColor;
    uint16_t _currentBackgroundColor;

    void UpdateCurrentColors()
    {
        if (_state == ButtonState::Highlighted)
        {
            _currentBackgroundColor = _backgroundHighlightColor;
            _currentTextColor = _textHighlightColor;

        } else
        {
            _currentBackgroundColor = _backgroundColor;
            _currentTextColor = _textColor;
        }
    }

  public:
    explicit PushButton(const char* label, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IButton(x, y, width, height), _label(label), _cornerRadius(3), _state(ButtonState::Default)
    {
        _currentTextColor = _textColor = (uint16_t)Color565::Black;
        _currentBackgroundColor = _backgroundColor = utils::RGB565(220, 220, 220);

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
        _backgroundColor = color;
        UpdateCurrentColors();
    }

    void SetTextColor(uint16_t color)
    {
        _textColor = color;
        UpdateCurrentColors();
    }

    void SetHighlightBackgroundColor(uint16_t color)
    {
        _backgroundHighlightColor = color;
        UpdateCurrentColors();
    }

    void SetHighlightTextColor(uint16_t color)
    {
        _textHighlightColor = color;
        UpdateCurrentColors();
    }

    void SetHighlighted(bool highlighted)
    {
        _state = highlighted ? ButtonState::Highlighted : ButtonState::Default;
        UpdateCurrentColors();
    }
};

#endif // PUSHBUTTON_H

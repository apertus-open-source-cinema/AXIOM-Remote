#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "IButton.h"
#include "../Painter/Painter.h"
#include "Icon.h"
//#include "../../Media/Icons/home_icon.h"

class ToggleButton : public IButton
{
    uint8_t _cornerRadius;
    bool _highlighted;

    // Color Defintions
    uint16_t _TextColor;
    uint16_t _BackgroundColor;

    uint16_t _textHighlightColor;
    uint16_t _textDisabledColor;
    uint16_t _backgroundHighlightColor;

    uint16_t _currentTextColor;
    uint16_t _currentBackgroundColor;

    bool _checked;
    const char* _label;
    Icon* _checkboxIcon;

  public:
    explicit ToggleButton(const char* label, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IButton(x, y, width, height), _label(label), _cornerRadius(3), _highlighted(false), _checked(false)
    {
        _currentTextColor = _TextColor = (uint16_t)Color565::Black;
        _currentBackgroundColor = _BackgroundColor = RGB565(220, 220, 220);
        _textDisabledColor = RGB565(180, 180, 180);

        _backgroundHighlightColor = (uint16_t)Color565::AXIOM_Blue;
        _textHighlightColor = (uint16_t)Color565::Black;

        _checkboxIcon = &home_icon;
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
        uint8_t gap = 8;
        uint8_t totaltextwidth = painter->GetStringFramebufferWidth(_label) + gap + _checkboxIcon->Width;

        painter->DrawText(_x + _width / 2 - totaltextwidth / 2, _y + textPosY, _label, _currentTextColor,
                          TextAlign::TEXT_ALIGN_LEFT, 0);

        painter->DrawIcon(_checkboxIcon->Data,
                          _x + _width / 2 - totaltextwidth / 2 + painter->GetStringFramebufferWidth(_label) + gap,
                          _y + _height / 2 - _checkboxIcon->Height / 2, _checkboxIcon->Width, _checkboxIcon->Height,
                          _currentTextColor);
    }

    void SetChecked(bool checked)
    {
        _checked = checked;

        if (_checked)
        {
            _currentTextColor = _TextColor;
        } else
        {
            _currentTextColor = _textDisabledColor;
        }
    }

    bool GetChecked()
    {
        return _checked;
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
    };
};

#endif // TOGGLEBUTTON_H

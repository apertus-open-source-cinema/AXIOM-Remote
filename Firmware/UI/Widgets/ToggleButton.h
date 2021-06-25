#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "IButton.h"
#include "../Painter/Painter.h"
#include "../../../Firmware/Media/Icons/checkboxfalse_icon.h"
#include "../../../Firmware/Media/Icons/checkboxtrue_icon.h"

class ToggleButton : public IButton
{
    uint8_t _cornerRadius;
    bool _highlighted;

    // Color Defintions
    uint16_t _textColor;
    uint16_t _backgroundColor;

    uint16_t _textHighlightColor;
    uint16_t _textDisabledColor;
    uint16_t _backgroundHighlightColor;

    uint16_t _currentTextColor;
    uint16_t _currentBackgroundColor;

    bool _checked;
    const char* _label;
    const Icon* _checkboxIcon;

  public:
    explicit ToggleButton(const char* label, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IButton(x, y, width, height), _cornerRadius(3), _highlighted(false), _textColor(_currentTextColor),
        _backgroundColor(_currentBackgroundColor), _textHighlightColor((uint16_t)Color565::Black),
        _textDisabledColor(utils::RGB565(180, 180, 180)), _backgroundHighlightColor((uint16_t)Color565::AXIOM_Blue),
        _currentTextColor((uint16_t)Color565::Black), _currentBackgroundColor(utils::RGB565(220, 220, 220)),
        _checked(true), _label(label), _checkboxIcon(&checkboxfalse_icon)
    {
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
        uint8_t gap = 8; // gap (in pixels) between checkbox icon and text

        _checkboxIcon = (_checked) ? &checkboxtrue_icon : &checkboxfalse_icon;
        uint8_t totalcontentwidth = _checkboxIcon->Width + gap + painter->GetStringFramebufferWidth(_label);

        painter->DrawIcon(_checkboxIcon, _x + _width / 2 - totalcontentwidth / 2,
                          _y + _height / 2 - _checkboxIcon->Height / 2, _currentTextColor);

        painter->DrawText(_x + _width / 2 - totalcontentwidth / 2 + _checkboxIcon->Width + gap, _y + textPosY, _label,
                          _currentTextColor, TextAlign::TEXT_ALIGN_LEFT, 0);
    }

    void SetChecked(bool checked)
    {
        _checked = checked;

        if (_checked)
        {
            _currentTextColor = _textColor;
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
        _backgroundColor = color;
        SetHighlighted(_highlighted);
    }

    void SetTextColor(uint16_t color)
    {
        _textColor = color;
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
            _currentTextColor = _textColor;
            _currentBackgroundColor = _backgroundColor;
        }
    };
};

#endif // TOGGLEBUTTON_H

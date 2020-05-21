#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "IButton.h"
#include "../Painter/Painter.h"

//#include <cstring>

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

    bool _toggle;
    const char* _label1;
    const char* _label2;

  public:
    explicit ToggleButton(const char* label1, const char* label2, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0,
                          uint16_t height = 0) :
        IButton(x, y, width, height),
        _label1(label1), _label2(label2), _cornerRadius(3), _highlighted(false), _toggle(false)
    {
        _currentTextColor = _TextColor = (uint16_t)Color565::Black;
        _currentBackgroundColor = _BackgroundColor = RGB565(220, 220, 220);
        _textDisabledColor = RGB565(180, 180, 180);

        _backgroundHighlightColor = (uint16_t)Color565::AXIOM_Blue;
        _textHighlightColor = (uint16_t)Color565::Black;
    }

    void SetCornerRadius(uint8_t cornerRadius)
    {
        _cornerRadius = cornerRadius;
    }

    void SetLabels(const char* label1, const char* label2)
    {
        _label1 = label1;
        _label2 = label2;
    }

    virtual void Draw(IPainter* painter) override
    {
        painter->DrawFillRoundRectangle(_x, _y, _width, _height, _cornerRadius, _currentBackgroundColor);

        painter->SetFont(Font::FreeSans9pt7b);
        uint8_t textPosY = _height / 2 + painter->GetCurrentFontHeight() / 2;
        uint8_t gap = 8;
        uint8_t totaltextwidth =
            painter->GetStringFramebufferWidth(_label1) + gap + painter->GetStringFramebufferWidth(_label2);
        uint8_t rect_margin = 8;
        if (_toggle)
        {
            painter->DrawFillRoundRectangle(_x + _width / 2 - totaltextwidth / 2 - rect_margin / 2,
                                            _y + textPosY - painter->GetCurrentFontHeight() - rect_margin / 2 + 1,
                                            painter->GetStringFramebufferWidth(_label1) + rect_margin,
                                            painter->GetCurrentFontHeight() + rect_margin, 2, _TextColor);

            painter->DrawText(_x + _width / 2 - totaltextwidth / 2, _y + textPosY, _label1,
                              (uint16_t)Color565::AXIOM_Orange, TextAlign::TEXT_ALIGN_LEFT, 0);

            painter->DrawText(_x + _width / 2 - totaltextwidth / 2 + painter->GetStringFramebufferWidth(_label1) + gap,
                              _y + textPosY, _label2, _textDisabledColor, TextAlign::TEXT_ALIGN_LEFT, 0);
        } else
        {
            painter->DrawFillRoundRectangle(_x + _width / 2 - totaltextwidth / 2 +
                                                painter->GetStringFramebufferWidth(_label1) + gap - rect_margin / 2,
                                            _y + textPosY - painter->GetCurrentFontHeight() - rect_margin / 2 + 1,
                                            painter->GetStringFramebufferWidth(_label1) + rect_margin,
                                            painter->GetCurrentFontHeight() + rect_margin, 2, _TextColor);

            painter->DrawText(_x + _width / 2 - totaltextwidth / 2, _y + textPosY, _label1, _textDisabledColor,
                              TextAlign::TEXT_ALIGN_LEFT, 0);

            painter->DrawText(_x + _width / 2 - totaltextwidth / 2 + painter->GetStringFramebufferWidth(_label1) + gap,
                              _y + textPosY, _label2, (uint16_t)Color565::AXIOM_Orange, TextAlign::TEXT_ALIGN_LEFT, 0);
        }
    }

    void SetToggle(bool toggle)
    {
        _toggle = toggle;
    }

    bool GetToggle()
    {
        return _toggle;
    }

    const char* GetOption()
    {
        if (_toggle)
        {
            return _label1;
        } else
        {
            return _label2;
        }
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

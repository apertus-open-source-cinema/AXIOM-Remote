#ifndef PARAMETERMENUITEM_H
#define PARAMETERMENUITEM_H

#include "IWidget.h"

#include "../Color565.h"

class IPainter;

class ParameterMenuItem : public IWidget
{
    bool _disabled;
    bool _hidden;
    bool _pressed;
    bool _highlighted;
    bool _previousChoice;
    char const* _label;
    char const* _value;

    uint16_t _backgroundColor;
    uint16_t _backgroundHighlightColor;
    uint16_t _backgroundPressedColor;
    uint16_t _backgroundDisabledColor;

    uint16_t _textColor;
    uint16_t _textHighlightColor;
    uint16_t _textPressedColor;
    uint16_t _textDisabledColor;

    uint16_t _currentBackgroundColor;
    uint16_t _currentTextColor;

    uint8_t _verticalLabelOffset;
    uint8_t _horizontalTextMargin;

  public:
    ParameterMenuItem(const char* label = "...", bool disabled = false, const char* value = nullptr,
                      bool hidden = false, bool pressed = false, bool highlighted = false) :
        _disabled(disabled),
        _hidden(hidden), _pressed(pressed), _highlighted(highlighted), _previousChoice(false), _label(label),
        _value(value), _backgroundColor((uint16_t)Color565::White),
        _backgroundHighlightColor(utils::RGB565(255, 128, 0)), _backgroundPressedColor(utils::RGB565(0, 128, 255)),
        _backgroundDisabledColor(utils::RGB565(180, 180, 180)), _textColor((uint16_t)Color565::Black),
        _textHighlightColor((uint16_t)Color565::White), _textPressedColor((uint16_t)Color565::White),
        _textDisabledColor(utils::RGB565(180, 180, 180)), _currentBackgroundColor(_backgroundColor),
        _currentTextColor(_textColor), _verticalLabelOffset(20)
    {
        _x = 0;
        _y = 0;
        _width = 50;
        _height = 20;
    }

    void SetDisabled(bool disabled)
    {
        _disabled = disabled;

        if (disabled)
        {
            _currentBackgroundColor = _backgroundDisabledColor;
            _currentTextColor = _textDisabledColor;
        } else
        {
            // TODO: add more case handling here (what if highlighted, pressed, etc.)

            _currentBackgroundColor = _backgroundColor;
            _currentTextColor = _textColor;
        }
    }

    bool IsDisabled()
    {
        return _disabled;
    }

    void SetHidden(bool hide)
    {
        _hidden = hide;
    }

    bool IsHidden()
    {
        return _hidden;
    }

    void SetPressed(bool pressed)
    {
        if (_disabled)
        {
            return;
        }

        _pressed = pressed;

        if (pressed)
        {
            _currentBackgroundColor = _backgroundPressedColor;
            _currentTextColor = _textPressedColor;
        } else if (_highlighted)
        {
            _currentBackgroundColor = _backgroundHighlightColor;
            _currentTextColor = _textHighlightColor;
        } else
        {
            _currentBackgroundColor = _backgroundColor;
            _currentTextColor = _textColor;
        }
    }

    bool IsPressed()
    {
        return _pressed;
    }

    void SetHighlighted(bool highlighted)
    {
        _highlighted = highlighted;

        if (highlighted)
        {
            _currentBackgroundColor = _backgroundHighlightColor;
            _currentTextColor = _textHighlightColor;
        } else if (_disabled)
        {
            _currentBackgroundColor = _backgroundDisabledColor;
            _currentTextColor = _textDisabledColor;
        } else
        {
            _currentBackgroundColor = _backgroundColor;
            _currentTextColor = _textColor;
        }
    }

    bool IsHighlighted()
    {
        return _highlighted;
    }

    void SetLabel(const char* value)
    {
        _label = value;
    }

    char const* GetLabel()
    {
        return _label;
    }

    void SetValue(char const* value)
    {
        _value = value;
    }

    char const* GetValue()
    {
        return _value;
    }

    void SetPreviousChoice(bool previousChoice)
    {
        _previousChoice = previousChoice;
    }

    void SetDimensions(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        _x = x;
        _y = y;

        _width = width;
        _height = height;
    }

    void SetHorizontalTextMargin(uint8_t horizontalTextMargin)
    {
        _horizontalTextMargin = horizontalTextMargin;
    }

    void SetY(uint16_t y)
    {
        _y = y;
    }

    void Draw(IPainter* painter) override
    {
        // Draw background
        if (_disabled)
        {
            painter->DrawStripedRectangle(_x, _y, _width, _height, 0x8A01, 0x5120, 5, 12);
        } else
        {
            painter->DrawFillRectangle(_x, _y, _width, _height, _currentBackgroundColor);
        }

        // draw label
        painter->DrawText(_x + _horizontalTextMargin, _y + _verticalLabelOffset, _label, _currentTextColor,
                          TextAlign::TEXT_ALIGN_LEFT, 0);

        // draw current (old) value indicator circle
        if (_previousChoice)
        {
            painter->DrawFillCircle(_x + 4, _y + 15, 3, _currentTextColor);
        }
    }

    void ExecuteAction(IMenuSystem* menuSystem)
    {
    }
};

#endif /* PARAMETERMENUITEM_H */

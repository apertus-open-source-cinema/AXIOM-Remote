#ifndef MAINPAGEBUTTON_H
#define MAINPAGEBUTTON_H

#include "IButton.h"
#include "../Painter.h"

#include "../Color565.h"

enum class ButtonType
{
    VALUE_AND_LABEL,
    BUTTON
};

class MainPageButton : public IButton
{
    uint16_t _x;
    uint16_t _y;

    uint16_t _width;

    uint16_t _labelHeight;
    uint16_t _valueHeight;

    char* _label;
    char* _value;

    bool _invertOrder;

    Font _labelFont;
    Font _valueFont;

    ButtonType _type;

    // Color Defintions
    uint16_t labelTextColor;
    uint16_t labelBackgroundColor;
    uint16_t valueTextColor;
    uint16_t valueBackgroundColor;

    // bool _hideValue;

  public:
    MainPageButton() : _x(0), _y(0), _width(0)
    {
    }

    // TODO: Check if customizable height is required for this button, if yes, add it later
    MainPageButton(uint16_t x, uint16_t y, uint16_t width, const char* label = "...", bool invertOrder = false,
                   ButtonType type = ButtonType::VALUE_AND_LABEL) :
        _x(x),
        _y(y), _width(width), _labelHeight(20), _valueHeight(40), _label((char*)label), _value((char*)"..."),
        _invertOrder(invertOrder), _labelFont(Font::FreeSans9pt7b), _valueFont(Font::FreeSans12pt7b), _type(type)
    {
        labelTextColor = (uint16_t)Color565::White;
        labelBackgroundColor = (uint16_t)Color565::Black;
        valueTextColor = (uint16_t)Color565::Black;
        valueBackgroundColor = (uint16_t)Color565::White;
    }

    void Draw(Painter* painter) override
    {
        // 3 Buttons at the top (with invertedOrder == true) of the screen have the label at the top and the value below
        if (_invertOrder)
        {
            if (_type == ButtonType::VALUE_AND_LABEL)
            {
                // draw label
                painter->DrawFillRoundRectangle(_x, _y + _valueHeight, _width, _labelHeight, 3, labelBackgroundColor);
                painter->DrawFillRectangle(_x, _y + _valueHeight - 3, _width, 6, labelBackgroundColor);
                // painter->DrawFillRectangle(_x, _y + _valueHeight, _width, _labelHeight, (uint16_t)Color565::Black);
                painter->DrawText(_x, _y + _valueHeight + 5, _label, labelTextColor, Font::FreeSans9pt7b,
                                  TextAlign::TEXT_ALIGN_CENTER, _width);

                // draw value
                painter->DrawFillRoundRectangle(_x, _y, _width, _valueHeight, 3, valueBackgroundColor);
                painter->DrawText(_x, _y + 10, _value, valueTextColor, Font::FreeSans9pt7b,
                                  TextAlign::TEXT_ALIGN_CENTER, _width);
            }

            if (_type == ButtonType::BUTTON)
            {

                uint8_t fontoffset = GetFontOffset();

                // draw label
                painter->DrawFillRoundRectangle(_x, _y, _width, _labelHeight, 3, labelBackgroundColor);
                painter->DrawText(_x, _y + fontoffset, _label, labelTextColor, _labelFont, TextAlign::TEXT_ALIGN_CENTER,
                                  _width);
            }
        } else
        // Buttons at the bottom of the screen have the label at the bottom and the value above it -> _invertOrder ==
        // false
        {
            if (_type == ButtonType::BUTTON)
            {
                uint8_t fontoffset = GetFontOffset();

                // draw label
                painter->DrawFillRoundRectangle(_x, _y, _width, _labelHeight, 3, labelBackgroundColor);
                painter->DrawText(_x, _y + fontoffset, _label, labelTextColor, _labelFont, TextAlign::TEXT_ALIGN_CENTER,
                                  _width);
            } else if (_type == ButtonType::VALUE_AND_LABEL)
            {
                // draw label
                painter->DrawFillRoundRectangle(_x, _y + 1, _width, _labelHeight, 3, labelBackgroundColor);
                painter->DrawFillRectangle(_x, _y + _labelHeight - 3, _width, 6, labelBackgroundColor);
                painter->DrawText(_x, _y + 5, _label, labelTextColor, Font::FreeSans9pt7b, TextAlign::TEXT_ALIGN_CENTER,
                                  _width);

                // draw value
                painter->DrawFillRoundRectangle(_x, _y + _labelHeight, _width, _valueHeight, 3, valueBackgroundColor);
                painter->DrawText(_x, _y + _labelHeight + 10, _value, valueTextColor, Font::FreeSans9pt7b,
                                  TextAlign::TEXT_ALIGN_CENTER, _width);
            }
        }
    }

    uint8_t GetFontOffset()
    {
        uint8_t fontOffset = 0;

        switch (_labelFont)
        {
        case Font::FreeSans9pt7b:
            fontOffset = _labelHeight / 2 - 16 / 2;
            break;
        case Font::FreeSans12pt7b:
            fontOffset = _labelHeight / 2 - 24 / 2;
            break;
        case Font::FreeSans18pt7b:
            fontOffset = _labelHeight / 2 - 34 / 2;
            break;
        case Font::FreeSans24pt7b:
            fontOffset = _labelHeight / 2 - 44 / 2;
            break;
        }

        return fontOffset;
    }

    void SetLabelFont(Font font)
    {
        _labelFont = font;
    }

    void SetType(ButtonType type)
    {
        _type = type;
    }

    void SetValue(char* value)
    {
        _value = value;
    }

    void SetLabelHeight(uint16_t height)
    {
        _labelHeight = height;
    };
};

#endif /* MAINPAGEBUTTON_H */

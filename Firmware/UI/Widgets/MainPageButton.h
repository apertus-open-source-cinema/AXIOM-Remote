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
    bool _hightlighted;

    Font _labelFont;
    Font _valueFont;

    ButtonType _type;

    // Color Defintions
    uint16_t labelTextColor;
    uint16_t labelBackgroundColor;
    uint16_t valueTextColor;
    uint16_t valueBackgroundColor;
    uint16_t backgroundHighlightColor;
    uint16_t textHighlightColor;

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
        backgroundHighlightColor = (uint16_t)Color565::AXIOMOrange;
        textHighlightColor = (uint16_t)Color565::White;
        _hightlighted = false;
    }

    void Draw(Painter* painter) override
    {
        DrawLabel(painter);
        DrawValue(painter);
    }

    void DrawLabel(Painter* painter)
    {
        /* the buttons at the top have label at the top and value fields below
         *  for the buttons at the bottom of the screen the order is inverted (_invertOrder == true)
         *  meaning the value field is at the top and the label at the bottom. The drawing routine needs to
         *  account for that.
         */

        if (_type == ButtonType::VALUE_AND_LABEL)
        {
            if (_invertOrder)
            {
                // bottom buttons
                if (_hightlighted)
                {
                    painter->DrawFillRoundRectangle(_x, _y + _valueHeight - 2, _width, _labelHeight + 3, 3,
                                                    backgroundHighlightColor);
                } else
                {
                    painter->DrawFillRoundRectangle(_x, _y + _valueHeight - 2, _width, _labelHeight + 3, 3,
                                                    labelBackgroundColor);
                }
                painter->SetFont(_labelFont);

                uint8_t fontOffset = GetFontYOffset(_labelHeight);
                // std::cout << "Font offset: " << (int)fontOffset << std::endl;

                if (_hightlighted)
                {
                    painter->DrawText(_x, _y + _valueHeight + 16, _label, labelBackgroundColor,
                                      TextAlign::TEXT_ALIGN_CENTER, _width);
                } else
                {
                    painter->DrawText(_x, _y + _valueHeight + 16, _label, labelTextColor, TextAlign::TEXT_ALIGN_CENTER,
                                      _width);
                }
            } else
            {
                // top buttons
                if (_hightlighted)
                {
                    painter->DrawFillRoundRectangle(_x, _y, _width, _labelHeight + 3, 3, backgroundHighlightColor);
                } else
                {
                    painter->DrawFillRoundRectangle(_x, _y, _width, _labelHeight + 3, 3, labelBackgroundColor);
                }

                painter->SetFont(_labelFont);

                uint8_t fontOffset = GetFontYOffset(_labelHeight);
                // std::cout << "Font offset: " << (int)fontOffset << std::endl;

                if (_hightlighted)
                {
                    painter->DrawText(_x, _y + _labelHeight - fontOffset, _label, labelBackgroundColor,
                                      TextAlign::TEXT_ALIGN_CENTER, _width);
                } else
                {
                    painter->DrawText(_x, _y + _labelHeight - fontOffset, _label, labelTextColor,
                                      TextAlign::TEXT_ALIGN_CENTER, _width);
                }
            }
        }

        if (_type == ButtonType::BUTTON)
        {
            if (_hightlighted)
            {
                painter->DrawFillRoundRectangle(_x, _y, _width, _labelHeight, 3, backgroundHighlightColor);
                painter->SetFont(_labelFont);

                uint8_t fontOffset = GetFontYOffset(_labelHeight);
                // std::cout << "Font offset: " << (int)fontOffset << std::endl;
                painter->DrawText(_x, _y + 20, _label, labelBackgroundColor, TextAlign::TEXT_ALIGN_CENTER, _width);
            } else
            {
                painter->DrawFillRoundRectangle(_x, _y, _width, _labelHeight, 3, labelBackgroundColor);
                painter->SetFont(_labelFont);

                uint8_t fontOffset = GetFontYOffset(_labelHeight);
                // std::cout << "Font offset: " << (int)fontOffset << std::endl;
                painter->DrawText(_x, _y + 20, _label, labelTextColor, TextAlign::TEXT_ALIGN_CENTER, _width);
            }
        }
    }

    void DrawValue(Painter* painter)
    {
        if (_type == ButtonType::VALUE_AND_LABEL)
        {
            if (_invertOrder)
            {
                // bottom buttons

                if (_hightlighted)
                {
                    painter->DrawFillRoundRectangle(_x, _y, _width, _valueHeight, 3, valueBackgroundColor);
                    painter->SetFont(_valueFont);

                    uint8_t fontOffset = GetFontYOffset(_valueHeight);
                    painter->DrawText(_x, _y + fontOffset + 16, _value, valueTextColor, TextAlign::TEXT_ALIGN_CENTER,
                                      _width);
                } else
                {
                    painter->DrawFillRoundRectangle(_x, _y, _width, _valueHeight, 3, valueBackgroundColor);
                    painter->SetFont(_valueFont);

                    uint8_t fontOffset = GetFontYOffset(_valueHeight);
                    painter->DrawText(_x, _y + fontOffset + 16, _value, valueTextColor, TextAlign::TEXT_ALIGN_CENTER,
                                      _width);
                }

            } else
            {
                // top buttons

                if (_hightlighted)
                {
                    painter->DrawFillRoundRectangle(_x, _y + _labelHeight, _width, _valueHeight, 3, valueTextColor);
                    painter->SetFont(_valueFont);

                    uint8_t fontOffset = GetFontYOffset(_valueHeight);
                    painter->DrawText(_x, _y + _labelHeight + fontOffset + 16, _value, valueBackgroundColor,
                                      TextAlign::TEXT_ALIGN_CENTER, _width);
                } else
                {
                    painter->DrawFillRoundRectangle(_x, _y + _labelHeight, _width, _valueHeight, 3,
                                                    valueBackgroundColor);
                    painter->SetFont(_valueFont);

                    uint8_t fontOffset = GetFontYOffset(_valueHeight);
                    painter->DrawText(_x, _y + _labelHeight + fontOffset + 16, _value, valueTextColor,
                                      TextAlign::TEXT_ALIGN_CENTER, _width);
                }
            }
        }
    }

    uint8_t GetFontYOffset(uint8_t height)
    {
        uint8_t fontOffset = 0;

        switch (_labelFont)
        {
        case Font::FreeSans9pt7b:
            fontOffset = height / 2 - 16 / 2 + 2;
            break;
        case Font::FreeSans12pt7b:
            fontOffset = height / 2 - 24 / 2;
            break;
        case Font::FreeSans18pt7b:
            fontOffset = height / 2 - 34 / 2;
            break;
        case Font::FreeSans24pt7b:
            fontOffset = height / 2 - 44 / 2;
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
    }

    void SetHighlighted(bool hightlighted)
    {
        _hightlighted = hightlighted;
    };
};

#endif /* MAINPAGEBUTTON_H */

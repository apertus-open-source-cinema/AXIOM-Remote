/* 
 * File:   MainMenuButton.h
 * Author: dev
 *
 * Created on 03 August 2019, 23:44
 */

#ifndef MAINMENUBUTTON_H
#define	MAINMENUBUTTON_H

#include "IButton.h"
#include "Painter.h"

#include "Color565.h"


class MainMenuButton : public IButton
{
    uint16_t _x;
    uint16_t _y;

    uint16_t _width;

    uint16_t _captionHeight;
    uint16_t _valueHeight;

    //std::string _caption;
    char* _caption;
    char* _value;

    bool _invertOrder;

    Font _captionFont;
    Font _valueFont;

    bool _hideValue;

public:

    // TODO: Check if customizable height is required for this button, if yes, add it later

    MainMenuButton(uint16_t x, uint16_t y, uint16_t width, const char* caption = "...", bool invertOrder = false) :
    _x(x),
    _y(y),
    _width(width),
    _captionHeight(20),
    _valueHeight(40),
    _caption((char*)caption),
    _value((char*)"..."),
    _invertOrder(invertOrder),
    _captionFont(Font::FreeSans9pt7b),
    _valueFont(Font::FreeSans12pt7b),
    _hideValue(false)
    {

    }

    void Draw(Painter* painter) override
    {
        if(_invertOrder)
        {
            if(!_hideValue)
            {
                painter->SetFont(_valueFont);
                painter->DrawFillRectangle(_x, _y, _width, _valueHeight, Color565::White);
                painter->DrawText(_value, _x, _y + (_valueHeight - 5),  Color565::Black, TextAlign::TEXT_ALIGN_CENTER, _width);
            }

            if(_hideValue)
            {
                _valueHeight = 0;
            }

            painter->SetFont(_captionFont);
            painter->DrawFillRectangle(_x, _y + _valueHeight, _width, _captionHeight,  Color565::Black);
            painter->DrawText(_caption, _x, _y + _valueHeight + (_captionHeight - 5),  Color565::White, TextAlign::TEXT_ALIGN_CENTER, _width);

        }
        else
        {
            painter->SetFont(_captionFont);
            painter->DrawFillRectangle(_x, _y, _width, _captionHeight,  Color565::Black);
            painter->DrawText(_caption, _x, _y + 15,  Color565::White, TextAlign::TEXT_ALIGN_CENTER, _width);

            if(!_hideValue)
            {
                painter->SetFont(_valueFont);
                painter->DrawFillRectangle(_x, _y + _captionHeight, _width, _valueHeight,  Color565::White);
                painter->DrawText(_value, _x, _y + _captionHeight + (_valueHeight - 5),  Color565::Black, TextAlign::TEXT_ALIGN_CENTER, _width);
            }
        }
    }

    void SetCaptionFont(Font font)
    {
        _captionFont = font;
    }

    void HideValue(bool hide)
    {
        _hideValue = hide;
    }

    void SetValue(char* value)
    {
        _value = value;
    }

    void SetCaptionHeight(uint16_t height)
    {
        _captionHeight = height;
    };
};

#endif	/* MAINMENUBUTTON_H */


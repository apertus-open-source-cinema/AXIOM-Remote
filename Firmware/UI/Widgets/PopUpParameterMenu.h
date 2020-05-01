#ifndef POPUPPARAMETERMENU_H
#define POPUPPARAMETERMENU_H

#include "IWidget.h"
#include "ParameterMenuItem.h"
#include "../Painter/IPainter.h"
#include "../../GlobalSettings.h"
#include "../Color565.h"

class PopUpParameterMenu : public IWidget
{
    uint16_t _backgroundColor;
    uint16_t _TextColor;
    uint16_t _borderColor;
    uint16_t _highlightColor;
    int8_t _highlightIndex;       // which item in parameter men is currently highlighted
    int8_t _pressedIndex;         // which item in parameter men is currently pressed
    uint8_t _previousChoiceIndex; // this item is the "old" selected choice but remains in effcet until the parameter
                                  // menu select has been completed
    uint8_t _choiceCount;         // number of parameter menu entries
    uint8_t _menuItemIndex; // this number indicates which menu item the parameter menu was launched from and affects
                            // location/choice positions
    uint8_t _borderwidth;

    ParameterMenuItem _parameterMenuItem[7] = {
        ParameterMenuItem("..."), ParameterMenuItem("..."), ParameterMenuItem("..."), ParameterMenuItem("..."),
        ParameterMenuItem("..."), ParameterMenuItem("..."), ParameterMenuItem("...")};

  public:
    PopUpParameterMenu(uint16_t x, uint16_t y, uint16_t width, uint16_t height) :
        IWidget(x, y, width, height), _backgroundColor(RGB565(97, 92, 91))
    {
        _choiceCount = 4;
        _borderwidth = 2;
        _height = _borderwidth * 2 + _choiceCount * 30;
        _highlightIndex = -1;
        _pressedIndex = -1;
        SetHighlighted(0);
    }

    void SetHighlighted(uint8_t highlightindex)
    {
        if ((highlightindex >= 0) && (highlightindex < _choiceCount))
        {
            UnhighlightAllMenuItems();
            _highlightIndex = highlightindex;
            _parameterMenuItem[highlightindex].SetHighlighted(true);
        }
    }

    void SetChoices(const char* choicelabels[], uint8_t choices)
    {
        if (choices <= 7)
        {
            _choiceCount = choices;
            for (int8_t i = 0; i < choices; i++)
            {
                _parameterMenuItem[i].SetLabel(choicelabels[i]);
            }
        }
    }

    int8_t GetHighlightIndex()
    {
        return _highlightIndex;
    }

    int8_t GetPressedIndex()
    {
        return _pressedIndex;
    }

    void SetPressed(uint8_t pressedindex)
    {
        if ((pressedindex >= 0) && (pressedindex < _choiceCount))
        {
            UnPressAllMenuItems();
            _pressedIndex = pressedindex;
            _parameterMenuItem[pressedindex].SetPressed(true);
        }
    }

    void SetDimensions(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        _x = x - _borderwidth;
        _y = y + _borderwidth;
        _width = width;
        _height = _borderwidth * 2 + _choiceCount * 30;

        // parameter menu would reach outside of screen so we move it down one line until it fits
        while ((_y - _height) < 0)
        {
            _y += 30;
        }
    }

    virtual void Draw(IPainter* painter)
    {
        // dim the rest of the LCD content
        painter->Dim(); // still needs tuning to avoid funky colors

        // draw background
        painter->DrawFillRectangle(_x, _y - _height + _borderwidth, _width, _height, _backgroundColor);

        // draw the popup menu
        for (uint8_t itemIndex = 0; itemIndex < _choiceCount; itemIndex++)
        {
            ParameterMenuItem currentMenuItem = _parameterMenuItem[itemIndex];

            uint16_t y = _y - (itemIndex + 1) * 30;
            currentMenuItem.SetDimensions(_x + _borderwidth, y, _width - 2 * _borderwidth, 29);

            currentMenuItem.Draw(painter);
        }
    }

    void UnPressAllMenuItems()
    {
        uint8_t b;
        for (b = 0; b < _choiceCount; b++)
        {
            _parameterMenuItem[b].SetPressed(false);
        }
    }

    void UnhighlightAllMenuItems()
    {
        uint8_t b;
        for (b = 0; b < _choiceCount; b++)
        {
            _parameterMenuItem[b].SetHighlighted(false);
        }
    }
};

#endif // POPUPPARAMETERMENU_H
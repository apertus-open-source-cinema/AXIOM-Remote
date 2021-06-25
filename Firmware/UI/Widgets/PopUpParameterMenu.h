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
    uint8_t _horizontalTextMargin;
    bool init;

    // use fixed number of maximum 7 choices for the popup menu as more would not fit on the screen at a time
    ParameterMenuItem _parameterMenuItem[7] = {
        ParameterMenuItem("..."), ParameterMenuItem("..."), ParameterMenuItem("..."), ParameterMenuItem("..."),
        ParameterMenuItem("..."), ParameterMenuItem("..."), ParameterMenuItem("...")};

  public:
    PopUpParameterMenu(uint16_t x, uint16_t y) :
        IWidget(x, y, 0, 0), _backgroundColor(utils::RGB565(97, 92, 91)), _highlightIndex(-1), _pressedIndex(-1),
        _previousChoiceIndex(0), _choiceCount(1), _borderwidth(2), _horizontalTextMargin(10)
    {
        _height = _borderwidth * 2 + _choiceCount * 30;
        SetHighlighted(0);
        init = false;

        ClearPreviousChoice();
        _parameterMenuItem[_previousChoiceIndex].SetPreviousChoice(true);
    }

    void SetHighlighted(uint8_t highlightindex)
    {
        if (highlightindex < _choiceCount)
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
                _parameterMenuItem[i].SetHorizontalTextMargin(_horizontalTextMargin);
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
        if (pressedindex < _choiceCount)
        {
            _pressedIndex = pressedindex;
            UnPressAllMenuItems();
            _parameterMenuItem[pressedindex].SetPressed(true);

            _previousChoiceIndex = pressedindex;
            ClearPreviousChoice();
            _parameterMenuItem[pressedindex].SetPreviousChoice(true);
        }
    }

    void CalculateDimensions(IPainter* painter)
    {
        // Calculate the pixel width of the longest choice label in the parameter menu
        uint16_t maxchoicewidth = 0;

        for (uint8_t itemIndex = 0; itemIndex < _choiceCount; itemIndex++)
        {
            uint8_t textWidth = painter->GetStringFramebufferWidth(_parameterMenuItem[itemIndex].GetLabel());
            if (textWidth > maxchoicewidth)
            {
                maxchoicewidth = textWidth;
            }
        }
        _width = maxchoicewidth + 2 * _horizontalTextMargin + 2 * _borderwidth;

        _height = _borderwidth * 2 + _choiceCount * 30;

        // if parameter menu would reach outside of screen we move it down one line until it fits
        while ((_y - _height) < 0)
        {
            _y += 30;
        }

        // move the parameter window right to border the scrollbar
        _x = GlobalSettings::LCDWidth - 16 - _width;
    }
    void SetPosition(uint16_t x, uint16_t y)
    {
        _x = x - _borderwidth;
        _y = y + _borderwidth;

        init = false;
    }

    virtual void Draw(IPainter* painter) override
    {
        if (!init)
        {
            CalculateDimensions(painter);
            init = true;
        }

        // draw background
        painter->DrawFillRectangle(_x, _y - _height + _borderwidth, _width, _height, _backgroundColor);

        // draw the popup menu
        for (uint8_t itemIndex = 0; itemIndex < _choiceCount; itemIndex++)
        {
            ParameterMenuItem currentParameterMenuItem = _parameterMenuItem[itemIndex];

            uint16_t y = _y - (itemIndex + 1) * 30;
            currentParameterMenuItem.SetDimensions(_x + _borderwidth, y, _width - 2 * _borderwidth, 29);

            currentParameterMenuItem.Draw(painter);
        }
    }

    void UnPressAllMenuItems()
    {
        uint8_t b;
        for (b = 0; b < _choiceCount; b++)
        {
            _parameterMenuItem[b].SetPressed(false);
        }
        _pressedIndex = -1;
    }

    void UnhighlightAllMenuItems()
    {
        uint8_t b;
        for (b = 0; b < _choiceCount; b++)
        {
            _parameterMenuItem[b].SetHighlighted(false);
        }
    }

    void ClearPreviousChoice()
    {
        uint8_t b;
        for (b = 0; b < _choiceCount; b++)
        {
            _parameterMenuItem[b].SetPreviousChoice(false);
        }
    }
};

#endif // POPUPPARAMETERMENU_H

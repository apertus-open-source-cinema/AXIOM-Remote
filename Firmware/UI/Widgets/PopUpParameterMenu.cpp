#include "PopUpParameterMenu.h"

PopUpParameterMenu::PopUpParameterMenu(uint16_t x, uint16_t y) :
    IWidget(x, y, 0, 0), _backgroundColor(utils::RGB565(97, 92, 91)), _highlightIndex(-1), _pressedIndex(-1),
    _previousChoiceIndex(0), _choiceCount(1), _borderwidth(2), _horizontalTextMargin(10)
{
    _height = _borderwidth * 2 + _choiceCount * 30;
    SetHighlighted(0);
    init = false;

    ClearPreviousChoice();
    _parameterMenuItem[_previousChoiceIndex].SetPreviousChoice(true);
}

void PopUpParameterMenu::SetHighlighted(uint8_t highlightindex)
{
    if (highlightindex < _choiceCount)
    {
        UnhighlightAllMenuItems();
        _highlightIndex = highlightindex;
        _parameterMenuItem[highlightindex].SetHighlighted(true);
    }
}

void PopUpParameterMenu::SetChoices(const char* choicelabels[], uint8_t choices)
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

int8_t PopUpParameterMenu::GetHighlightIndex()
{
    return _highlightIndex;
}

int8_t PopUpParameterMenu::GetPressedIndex()
{
    return _pressedIndex;
}

void PopUpParameterMenu::SetPressedIndex(uint8_t index)
{
    if (index < _choiceCount)
    {
        _pressedIndex = index;
        UnPressAllMenuItems();
        _parameterMenuItem[_pressedIndex].SetPressed(true);

        _previousChoiceIndex = _pressedIndex;
        ClearPreviousChoice();
        _parameterMenuItem[_pressedIndex].SetPreviousChoice(true);
    }
}

void PopUpParameterMenu::CalculateDimensions(IPainter* painter)
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
void PopUpParameterMenu::SetPosition(uint16_t x, uint16_t y)
{
    _x = x - _borderwidth;
    _y = y + _borderwidth;

    init = false;
}

void PopUpParameterMenu::Draw(IPainter* painter)
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

void PopUpParameterMenu::UnPressAllMenuItems()
{
    uint8_t b;
    for (b = 0; b < _choiceCount; b++)
    {
        _parameterMenuItem[b].SetPressed(false);
    }
    _pressedIndex = -1;
}

void PopUpParameterMenu::UnhighlightAllMenuItems()
{
    uint8_t b;
    for (b = 0; b < _choiceCount; b++)
    {
        _parameterMenuItem[b].SetHighlighted(false);
    }
}

void PopUpParameterMenu::ClearPreviousChoice()
{
    uint8_t b;
    for (b = 0; b < _choiceCount; b++)
    {
        _parameterMenuItem[b].SetPreviousChoice(false);
    }
}
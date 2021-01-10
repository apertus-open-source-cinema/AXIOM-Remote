#ifndef BUTTONBAR_H
#define BUTTONBAR_H

#include "IButton.h"
#include "../Painter/IPainter.h"

#include "../Color565.h"

enum class ButtonPosition : uint8_t
{
    Left = 0,
    Center = 1,
    Right = 2
}; // TODO: reconsider labels to be less confusing

class ButtonBar : public IWidget
{
    static const uint8_t maxButtonCount = 3;
    IButton* _buttons[maxButtonCount] = {nullptr, nullptr, nullptr};

    uint16_t _backgroundColor;

    uint8_t _marginTop; // TODO: Rename, as top is not correct for every orientation
    uint8_t _buttonMargin;

  public:
    ButtonBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height) :
        IWidget(x, y, width, height), _backgroundColor(RGB565(97, 92, 91)), _marginTop(1), _buttonMargin(4)
    {
    }

    virtual void Draw(IPainter* painter) override
    {
        // background
        painter->DrawFillRectangle(_x, _y - _marginTop, _width, _height + _marginTop, _backgroundColor);

        for (int i = 0; i < maxButtonCount; i++)
        {
            IButton* button = _buttons[i];

            if (button == nullptr)
            {
                continue;
            }

            button->Draw(painter);
        }
    }

    uint8_t GetButtonPosition(uint16_t dimension)
    {
        return (dimension / maxButtonCount - _buttonMargin);
    }

    void SetButton(ButtonPosition position, IButton* button)
    {
        // Adjust positions of the button to correspond to the bar area
        if (_width < _height) // Vertical Bar
        {
            uint8_t buttonOffset = (_height + _width - 1) / 4 * ((uint8_t)position + 1) - (_width - 1);
            // we use square shaped buttons in vertical bars
            button->SetDimensions(_x, _y + buttonOffset, _width - 1, _width - 1);
        } else
        { // Horizontal bar
            uint8_t buttonDimension = GetButtonPosition(_width);
            uint8_t buttonMargin = buttonDimension * (uint8_t)position + _buttonMargin * ((uint8_t)position + 1);
            button->SetDimensions(_x + buttonMargin, _y, buttonDimension, _height);
        }

        _buttons[(uint8_t)position] = button;
    }
};

#endif // BUTTONBAR_H

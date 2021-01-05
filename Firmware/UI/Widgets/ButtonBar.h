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
};

class ButtonBar : public IWidget
{
    static const uint8_t maxButtonNumber = 3;
    IButton* _buttons[maxButtonNumber] = {nullptr, nullptr, nullptr};

    uint16_t _backgroundColor;
    uint8_t _marginTop;
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

        for (int i = 0; i < maxButtonNumber; i++)
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
        return (dimension / maxButtonNumber - _buttonMargin);
    }

    void SetButton(ButtonPosition position, IButton* button)
    {
        // Adjust positions of the button to correspond to the bar area
        uint8_t buttonDimension = GetButtonPosition(_width);
        uint8_t buttonMargin = buttonDimension * (uint8_t)position + _buttonMargin * ((uint8_t)position + 1);
        button->SetDimensions(_x + buttonMargin, _y, buttonDimension, _height);  
      
        if (_width < _height)
        {
            buttonDimension = GetButtonPosition(_height);
            buttonMargin = buttonDimension * (uint8_t)position + _buttonMargin * ((uint8_t)position + 1);
            button->SetDimensions(_x, _y + buttonMargin, _width, buttonDimension);  
        }
        
        _buttons[(uint8_t)position] = button;
    }
};

#endif // BUTTONBAR_H

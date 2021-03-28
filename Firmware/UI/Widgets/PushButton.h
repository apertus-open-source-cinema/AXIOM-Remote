#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <cstring>

#include "IButton.h"
#include "ButtonState.h"
#include "../Painter/Painter.h"

class PushButton : public IButton
{
    const char* _label;
    uint8_t _cornerRadius;

    // Color Defintions
    static constexpr uint8_t _stateCount = 3;
    static constexpr uint8_t _colorsPerState = 2;
    uint16_t colors[_stateCount * _colorsPerState];

  public:
    enum Colors : uint8_t
    {
        Text = 0,
        Background = 1,
    };

    explicit PushButton(const char* label, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IButton(_stateCount, _colorsPerState, colors, x, y, width, height), _label(label), _cornerRadius(3)
    {
        SetColor(ButtonState::Default, Colors::Text, (uint16_t)Color565::Black);
        SetColor(ButtonState::Default, Colors::Background, (uint16_t)utils::RGB565(220, 220, 220));

        SetColor(ButtonState::Highlighted, Colors::Text, (uint16_t)Color565::Black);
        SetColor(ButtonState::Highlighted, Colors::Background, (uint16_t)Color565::AXIOM_Blue);
    }

    void SetCornerRadius(uint8_t cornerRadius)
    {
        _cornerRadius = cornerRadius;
    }

    void SetLabel(const char* label)
    {
        _label = label;
    }

    virtual void Draw(IPainter* painter) override
    {
        painter->DrawFillRoundRectangle(_x, _y, _width, _height, _cornerRadius, GetCurrentColor(Colors::Background));

        painter->SetFont(Font::FreeSans12pt7b);
        uint8_t textPosY = _height / 2 + painter->GetCurrentFontHeight() / 2;
        painter->DrawText(_x + _width / 2, _y + textPosY, _label, GetCurrentColor(Colors::Text),
                          TextAlign::TEXT_ALIGN_CENTER, strlen(_label));
    }
};

#endif // PUSHBUTTON_H

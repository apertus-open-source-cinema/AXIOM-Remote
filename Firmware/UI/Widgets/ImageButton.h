#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include "IButton.h"
#include "ButtonState.h"
#include "../Painter/Painter.h"

#include "Icon.h"

enum class ButtonStyle : uint8_t
{
    Icon = 0,
    IconAndText = 1
};

class ImageButton : public IButton
{
    const Icon* _image;
    const char* _label;

    uint8_t _cornerRadius;

    // Color Definitions
    static constexpr uint8_t _stateCount = 3;
    static constexpr uint8_t _colorsPerState = 3;
    uint16_t colors[_stateCount * _colorsPerState];

    uint8_t _imagePositionX;
    uint8_t _textPositionX;
    uint8_t _textPositionY;
    uint8_t _totalWidth; // TODO: remove
    uint8_t _labelWidth;

    ButtonStyle _buttonStyle;

  protected:
    uint16_t* GetStatePtr() override
    {
        return colors;
    }

  public:
    enum Colors : uint8_t
    {
        Text = 0,
        Background = 1,
        Image = 2,
    };

    explicit ImageButton(const Icon* icon, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IButton(_stateCount, _colorsPerState, colors, x, y, width, height), _image(icon), _cornerRadius(3),
        _buttonStyle(ButtonStyle::Icon)
    {
        SetColor(ButtonState::Default, Colors::Image, static_cast<uint16_t>(Color565::Black));
        SetColor(ButtonState::Default, Colors::Background, utils::RGB565(220, 220, 220));
        SetColor(ButtonState::Highlighted, Colors::Background, static_cast<uint16_t>(Color565::AXIOM_Blue));
        _totalWidth = _image->Width;
        _textPositionY = _height / 2;
        _imagePositionX = _width / 2 - _totalWidth / 2;
        _textPositionX = _imagePositionX + _image->Width;
    }

    void SetCornerRadius(uint8_t cornerRadius)
    {
        _cornerRadius = cornerRadius;
    }

    void SetButtonStyle(ButtonStyle buttonstyle)
    {
        _buttonStyle = buttonstyle;
        /*if (buttonstyle == ButtonStyle::IconAndText)
        {
        }*/
    }

    ButtonStyle GetButtonStyle()
    {
        return _buttonStyle;
    }

    void SetLabel(const char* value)
    {
        _label = value;

        // painter->SetFont(Font::FreeSans12pt7b);
        //_labelWidth = painter->GetStringFramebufferWidth(_label);
    }

    char const* GetLabel()
    {
        return _label;
    }

    virtual void Draw(IPainter* painter) override
    {
        painter->DrawFillRoundRectangle(_x, _y, _width, _height, _cornerRadius, GetCurrentColor(Colors::Background));

        if (_buttonStyle == ButtonStyle::IconAndText)
        {
            painter->SetFont(Font::FreeSans12pt7b);

            _totalWidth +=
                painter->GetStringFramebufferWidth(_label); // TODO: This should not be recalculated with every redraw
            _textPositionY +=
                painter->GetCurrentFontHeight() / 2; // TODO: This should not be recalculated with every redraw

            painter->DrawIcon(_image, _x + _imagePositionX, _y + _height / 2 - _image->Height / 2,
                              GetCurrentColor(Colors::Image));
            painter->DrawText(_x + _textPositionX, _y + _textPositionY, _label, GetCurrentColor(Colors::Text),
                              TextAlign::TEXT_ALIGN_LEFT, strlen(_label));
        } else if (_buttonStyle == ButtonStyle::Icon)
        {
            painter->DrawIcon(_image, _x + _width / 2 - _image->Width / 2, _y + _height / 2 - _image->Height / 2,
                              GetCurrentColor(Colors::Image));
        }
    }
};

#endif // IMAGEBUTTON_H

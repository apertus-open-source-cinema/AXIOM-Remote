#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include "IButton.h"
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
    bool _highlighted;

    // Color Defintions
    uint16_t _textColor;
    uint16_t _imageColor;
    uint16_t _backgroundColor;

    uint16_t _textHighlightColor;
    uint16_t _imageHighlightColor;
    uint16_t _backgroundHighlightColor;

    uint16_t _currentTextColor;
    uint16_t _currentImageColor;
    uint16_t _currentBackgroundColor;

    uint8_t _imagePositionX;
    uint8_t _textPositionX;
    uint8_t _textPositionY;
    uint8_t _totalWidth; // TODO: remove
    uint8_t _labelWidth;

    ButtonStyle _buttonStyle;

  public:
    explicit ImageButton(const Icon* icon, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IButton(x, y, width, height), _image(icon), _cornerRadius(3), _highlighted(false),
        _imageColor((uint16_t)Color565::Black), _backgroundColor(utils::RGB565(220, 220, 220)),
        _backgroundHighlightColor((uint16_t)Color565::AXIOM_Blue), _currentImageColor(_imageColor),
        _currentBackgroundColor(utils::RGB565(220, 220, 220)), _buttonStyle(ButtonStyle::Icon)
    {
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
        painter->DrawFillRoundRectangle(_x, _y, _width, _height, _cornerRadius, _currentBackgroundColor);

        if (_buttonStyle == ButtonStyle::IconAndText)
        {
            painter->SetFont(Font::FreeSans12pt7b);

            _totalWidth +=
                painter->GetStringFramebufferWidth(_label); // TODO: This should not be recalculated with every redraw
            _textPositionY +=
                painter->GetCurrentFontHeight() / 2; // TODO: This should not be recalculated with every redraw

            painter->DrawIcon(_image, _x + _imagePositionX, _y + _height / 2 - _image->Height / 2, _currentImageColor);
            painter->DrawText(_x + _textPositionX, _y + _textPositionY, _label, _currentTextColor,
                              TextAlign::TEXT_ALIGN_LEFT, strlen(_label));
        } else if (_buttonStyle == ButtonStyle::Icon)
        {
            painter->DrawIcon(_image, _x + _width / 2 - _image->Width / 2, _y + _height / 2 - _image->Height / 2,
                              _currentImageColor);
        }
    }

    void SetBackgroundColor(uint16_t color)
    {
        _backgroundColor = color;
        SetHighlighted(_highlighted);
    }

    void SetImageColor(uint16_t color)
    {
        _imageColor = color;
        SetHighlighted(_highlighted);
    }

    void SetTextColor(uint16_t color)
    {
        _textColor = color;
        SetHighlighted(_highlighted);
    }

    void SetHighlightBackgroundColor(uint16_t color)
    {
        _backgroundHighlightColor = color;
        SetHighlighted(_highlighted);
    }

    void SetHighlightImageColor(uint16_t color)
    {
        _imageHighlightColor = color;
        SetHighlighted(_highlighted);
    }

    void SetHighlightTextColor(uint16_t color)
    {
        _textHighlightColor = color;
        SetHighlighted(_highlighted);
    }

    void SetHighlighted(bool highlighted)
    {
        _highlighted = highlighted;
        if (highlighted)
        {
            _currentImageColor = _imageHighlightColor;
            _currentBackgroundColor = _backgroundHighlightColor;
            _currentTextColor = _textHighlightColor;

        } else
        {
            _currentImageColor = _imageColor;
            _currentBackgroundColor = _backgroundColor;
            _currentTextColor = _textColor;
        }
    }
};

#endif // IMAGEBUTTON_H

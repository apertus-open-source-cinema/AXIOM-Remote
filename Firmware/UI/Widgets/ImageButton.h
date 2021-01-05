#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include "IButton.h"
#include "../Painter/Painter.h"

#include "Icon.h"

class ImageButton : public IButton
{
    const Icon* _image;

    uint8_t _cornerRadius;
    bool _highlighted;

    // Color Defintions
    uint16_t _imageColor;
    uint16_t _backgroundColor;

    uint16_t _imageHighlightColor;
    uint16_t _backgroundHighlightColor;

    uint16_t _currentImageColor;
    uint16_t _currentBackgroundColor;

  public:
    explicit ImageButton(const Icon* icon, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IButton(x, y, width, height), _image(icon), _cornerRadius(3), _highlighted(false),
        _imageColor((uint16_t)Color565::Black), _currentImageColor(_imageColor),
        _backgroundHighlightColor((uint16_t)Color565::AXIOM_Blue), _currentBackgroundColor(RGB565(220, 220, 220)),
        _backgroundColor(_currentBackgroundColor)
    {
    }

    void SetCornerRadius(uint8_t cornerRadius)
    {
        _cornerRadius = cornerRadius;
    }

    virtual void Draw(IPainter* painter) override
    {
        painter->DrawFillRoundRectangle(_x - 2, _y + 20, 30, 30, _cornerRadius, _currentBackgroundColor);
        painter->DrawIcon(_image, _x + 1, _y + 23, _currentImageColor);
    }

    void SetBackgroundColor(uint16_t color)
    {
        _backgroundColor = color;
    }

    void SetImageColor(uint16_t color)
    {
        _imageColor = color;
    }

    void SetHighlightBackgroundColor(uint16_t color)
    {
        _backgroundHighlightColor = color;
    }

    void SetHighlightImageColor(uint16_t color)
    {
        _imageHighlightColor = color;
    }

    void SetHighlighted(bool highlighted)
    {
        _highlighted = highlighted;
        if (highlighted)
        {
            _currentImageColor = _imageHighlightColor;
            _currentBackgroundColor = _backgroundHighlightColor;

        } else
        {
            _currentImageColor = _imageColor;
            _currentBackgroundColor = _backgroundColor;
        }
    }
};

#endif // IMAGEBUTTON_H


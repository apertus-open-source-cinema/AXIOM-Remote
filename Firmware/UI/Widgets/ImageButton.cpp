#include "ImageButton.h"

ImageButton::ImageButton(const Icon* icon, uint16_t x, uint16_t y, uint16_t width, uint16_t height) :
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

void ImageButton::SetCornerRadius(uint8_t cornerRadius)
{
    _cornerRadius = cornerRadius;
}

void ImageButton::SetButtonStyle(ButtonStyle buttonstyle)
{
    _buttonStyle = buttonstyle;
    /*if (buttonstyle == ButtonStyle::IconAndText)
    {
    }*/
}

ButtonStyle ImageButton::GetButtonStyle() const
{
    return _buttonStyle;
}

void ImageButton::SetLabel(const char* value)
{
    // TODO: Calculate width to prevent doing it in Draw() and reducing performance
    _label = value;

    // painter->SetFont(Font::FreeSans12pt7b);
    //_labelWidth = painter->GetStringFramebufferWidth(_label);
}

const char* ImageButton::GetLabel() const
{
    return _label;
}

void ImageButton::Draw(IPainter* painter)
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

void ImageButton::SetBackgroundColor(uint16_t color)
{
    _backgroundColor = color;
    SetHighlighted(_highlighted);
}

void ImageButton::SetImageColor(uint16_t color)
{
    _imageColor = color;
    SetHighlighted(_highlighted);
}

void ImageButton::SetTextColor(uint16_t color)
{
    _textColor = color;
    SetHighlighted(_highlighted);
}

void ImageButton::SetHighlightBackgroundColor(uint16_t color)
{
    _backgroundHighlightColor = color;
    SetHighlighted(_highlighted);
}

void ImageButton::SetHighlightImageColor(uint16_t color)
{
    _imageHighlightColor = color;
    SetHighlighted(_highlighted);
}

void ImageButton::SetHighlightTextColor(uint16_t color)
{
    _textHighlightColor = color;
    SetHighlighted(_highlighted);
}

void ImageButton::SetHighlighted(bool highlighted)
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
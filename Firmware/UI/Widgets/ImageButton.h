#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include "IButton.h"
#include "../Painter/Painter.h"

#include "Icon.h"

class ImageButton : public IButton
{
    const Icon* _image;
    const char* _label;
    uint8_t _gap;

    uint8_t _cornerRadius;
    bool _highlighted;

    // Color Defintions
    uint16_t _TextColor;
    uint16_t _imageColor;
    uint16_t _backgroundColor;

    uint16_t _textHighlightColor;  
    uint16_t _imageHighlightColor;
    uint16_t _backgroundHighlightColor;

    uint16_t _currentTextColor;
    uint16_t _currentImageColor;
    uint16_t _currentBackgroundColor;
  
    uint8_t totalWidth;
    uint8_t textPosY;
    uint8_t imagePosX;
    uint8_t textPosX;

  public:
    explicit ImageButton(const Icon* icon, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IButton(x, y, width, height), _image(icon), _cornerRadius(3), _highlighted(false),
        _imageColor((uint16_t)Color565::Black), _currentImageColor(_imageColor),
        _backgroundHighlightColor((uint16_t)Color565::AXIOM_Blue), _currentBackgroundColor(RGB565(220, 220, 220)),
        _backgroundColor(_currentBackgroundColor)
    {
        totalWidth = _image->Width + GetGap();
        textPosY = _height / 2;
        imagePosX = _width / 2 - totalWidth / 2;
        textPosX = imagePosX + _image->Width + _gap;
    }

    void SetCornerRadius(uint8_t cornerRadius)
    {
        _cornerRadius = cornerRadius;
    }
  
    void SetGap(uint8_t gap)
    {
        _gap = gap;
    }
  
    uint8_t GetGap()
    {
        return _gap;
    }

    virtual void Draw(IPainter* painter) override
    {
        painter->DrawFillRoundRectangle(_x, _y, _width, _height, _cornerRadius, _currentBackgroundColor);
        
        if(_width > _height)
        {
            painter->SetFont(Font::FreeSans12pt7b);
            
            totalWidth += painter->GetStringFramebufferWidth(_label);
            textPosY += painter->GetCurrentFontHeight() / 2;
            
            painter->DrawIcon(_image, _x + imagePosX, _y + _height / 2 - _image->Height / 2, _currentImageColor);
            painter->DrawText(_x + textPosX, _y + textPosY, _label, _currentTextColor, TextAlign::TEXT_ALIGN_LEFT,
                              strlen(_label));
        }
      
        else
        {
            painter->DrawIcon(_image, _x + _width / 2 - _image->Width / 2, _y + _height / 2 - _image->Height / 2, _currentImageColor);
        }
    }

    void SetBackgroundColor(uint16_t color)
    {
        _backgroundColor = color;
    }

    void SetImageColor(uint16_t color)
    {
        _imageColor = color;
    }
  
    void SetTextColor(uint16_t color)
    {
        _TextColor = color;
        SetHighlighted(_highlighted);
    }

    void SetHighlightBackgroundColor(uint16_t color)
    {
        _backgroundHighlightColor = color;
    }

    void SetHighlightImageColor(uint16_t color)
    {
        _imageHighlightColor = color;
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
            _currentTextColor = _TextColor;
        }
    }
};

#endif // IMAGEBUTTON_H

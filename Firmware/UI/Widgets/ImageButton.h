#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include "IButton.h"
#include "../Painter/Painter.h"

class ImageButton : public IButton
{
      uint8_t _cornerRadius;
      const uint8_t* _image;
      bool _highlighted;

      // Color Defintions
      uint16_t _ImageColor;
      uint16_t _BackgroundColor;

      uint16_t _imageHighlightColor;
      uint16_t _backgroundHighlightColor;

      uint16_t _currentImageColor;
      uint16_t _currentBackgroundColor;
  
    public:
      ImageButton(const uint8_t* data, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
          IButton(x, y, width, height), _image(data), _cornerRadius(3), _highlighted(false)
      {
        _currentBackgroundColor = _BackgroundColor = RGB565(220, 220, 220);
        _backgroundHighlightColor = (uint16_t)Color565::AXIOM_Blue;
      }
  
      void SetCornerRadius(uint8_t cornerRadius)
      {
          _cornerRadius = cornerRadius;
      }

      virtual void Draw(IPainter* painter)
      {
          painter->DrawFillRoundRectangle(_x, _y, _width, _height, _cornerRadius, _currentBackgroundColor);
          painter->DrawIcon(_image, _x + 3, _y + 3, _width, _height, _currentImageColor);
      }

      void SetBackgroundColor(uint16_t color)
      {
          _BackgroundColor = color;
          SetHighlighted(_highlighted);
      }
      
      void SetImageColor(uint16_t color)
      {
          _ImageColor = color;
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

      void SetHighlighted(bool highlighted)
      {
          _highlighted = highlighted;
          if (highlighted)
          {
              _currentImageColor = _imageHighlightColor;
              _currentBackgroundColor = _backgroundHighlightColor;

          } else
          {
              _currentImageColor = _ImageColor;
              _currentBackgroundColor = _BackgroundColor;
          }
      }
};

#endif // IMAGEBUTTON_H

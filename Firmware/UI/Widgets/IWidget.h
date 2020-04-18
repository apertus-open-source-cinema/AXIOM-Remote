#ifndef IWIDGET_H
#define IWIDGET_H

#include "../../Utils.h"

class IPainter;

class IWidget
{
  protected:
    uint16_t _x;
    uint16_t _y;
    uint16_t _width;
    uint16_t _height;

  public:
    IWidget(uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        _x(x), _y(y), _width(width), _height(height)
    {
    }

    void SetDimensions(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        _x = x;
        _y = y;
        _width = width;
        _height = height;
    }

    virtual void Draw(IPainter* painter)
    {
        UNUSED(painter);

        // Default empty implementation to prevent overhead
    }
};

#endif /* IWIDGET_H */

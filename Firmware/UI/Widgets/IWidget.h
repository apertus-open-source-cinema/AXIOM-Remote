#ifndef IWIDGET_H
#define IWIDGET_H

#include "../../Utils.h"

class Painter;

class IWidget
{
  protected:
    uint16_t _x;
    uint16_t _y;
    uint16_t _width;
    uint16_t _height;

  public:
    IWidget() : _x(0), _y(0), _width(0), _height(0)
    {
    }

    virtual void Draw(Painter* painter)
    {
        UNUSED(painter);

        // Default empty implementation to prevent overhead
    }
};

#endif /* IWIDGET_H */

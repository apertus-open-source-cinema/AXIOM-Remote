#ifndef IMENU_H
#define IMENU_H

#include <stdint.h>

#include "Utils.h"

class Painter;
enum class Button;

class IMenu
{
  public:
    virtual uint16_t GetBackgroundColor()
    {
        return RGB565(180, 180, 180);
    }

    virtual void Draw(Painter* painter)
    {
        UNUSED(painter);
    }

    virtual void Update(Button button)
    {
        UNUSED(button);
    }
};

#endif /* IMENU_H */

#ifndef IMENU_H
#define IMENU_H

#include <stdint.h>

#include "../Utils.h"
#include "Color565.h"

class Painter;
enum class Button;

class IMenu
{
  public:
    virtual uint16_t GetBackgroundColor()
    {
        return (uint16_t)Color565::MenuBackground;
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

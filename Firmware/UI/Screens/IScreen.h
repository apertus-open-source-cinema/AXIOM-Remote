#ifndef ISCREEN_H
#define ISCREEN_H

#include "../Color565.h"
#include "../../Utils.h"

class IUSBDevice;
class IPainter;
class IMenuSystem;

enum class Button;

class IScreen
{
  public:
    virtual Color565 GetBackgroundColor()
    {
        return Color565::MenuBackground;
    }

    virtual void Draw(IPainter* painter)
    {
        UNUSED(painter);
    }

    virtual void Update(Button button, int8_t knob, IMenuSystem* menuSystem)
    {
        UNUSED(button);
        UNUSED(knob);
        UNUSED(menuSystem);
    }
};

#endif // ISCREEN_H
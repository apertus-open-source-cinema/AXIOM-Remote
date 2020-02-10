#ifndef ISCREEN_H
#define ISCREEN_H

#include "../Color565.h"
#include "../../Utils.h"

class IUSBDevice;
class Painter;
class IMenuSystem;

enum class Button;

class IScreen
{
  public:
    virtual Color565 GetBackgroundColor()
    {
        return Color565::MenuBackground;
    }

    virtual void Draw(Painter* painter)
    {
        UNUSED(painter);
    }

    virtual void Update(Button button, int8_t knob, IMenuSystem* menuSystem, IUSBDevice* cdcDevice)
    {
        UNUSED(button);
        UNUSED(knob);
        UNUSED(menuSystem);
        UNUSED(cdcDevice);
    }
};

#endif // ISCREEN_H
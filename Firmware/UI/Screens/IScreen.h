#ifndef ISCREEN_H
#define ISCREEN_H

#include "../Color565.h"
#include "../../Utils.h"

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

    virtual void Update(Button button, IMenuSystem* menuSystem)
    {
        UNUSED(button);
        UNUSED(menuSystem);
    }
};

#endif // ISCREEN_H
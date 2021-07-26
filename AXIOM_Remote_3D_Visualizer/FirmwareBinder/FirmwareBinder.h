#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include <cstdint>

#include <emscripten/val.h>

#include "../../Firmware/UI/Painter/Painter.h"
#include "../../Firmware/UI/MenuSystem.h"

enum class ButtonID;

class FirmwareBinder
{
    MenuSystem _menuSystem;
    Painter _painter;

  public:
    FirmwareBinder();

    emscripten::val GetFB();
    void UpdateFB();
    void Update(ButtonID button);
};

#endif // TEST_CLASS_H

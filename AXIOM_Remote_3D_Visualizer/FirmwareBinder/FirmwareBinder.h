#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include <cstdint>

#include <emscripten/val.h>

#include "../../Firmware/UI/Painter/Painter.h"
#include "../../Firmware/UI/MenuSystem.h"

enum class ButtonID;

class FirmwareBinder
{
    Painter _painter;
    MenuSystem _menuSystem;

  public:
    FirmwareBinder();

    emscripten::val GetFB();
    void UpdateFB();
    void Update(ButtonID button, int8_t knobValue);
};

#endif // TEST_CLASS_H

#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include <cstdint>

#include <emscripten/val.h>

#include "ButtonDefinitions.h"

enum class ButtonID;

class FirmwareBinder
{
  public:
    FirmwareBinder();

    emscripten::val GetFB();
    void Update(ButtonID button, int8_t knobValue, ButtonState buttonState);

    emscripten::val GetDirtyRects();
    void ClearDirtyRects();

    void SetDebugMarkersVisible(bool visible);
};

#endif // TEST_CLASS_H

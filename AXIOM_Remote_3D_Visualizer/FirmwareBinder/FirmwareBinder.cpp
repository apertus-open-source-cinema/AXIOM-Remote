#include "FirmwareBinder.h"

#include <emscripten/bind.h>

#define ILI9341_TFTWIDTH 240  ///< ILI9341 max TFT width
#define ILI9341_TFTHEIGHT 320 ///< ILI9341 max TFT height

uint16_t fb[ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT];

// uint8_t framebuffer[ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT * 3];

FirmwareBinder::FirmwareBinder() : _painter(fb, 320, 240), _menuSystem(nullptr, nullptr)
{
    _menuSystem.Draw(&_painter);
}

emscripten::val FirmwareBinder::GetFB()
{
    return emscripten::val(emscripten::typed_memory_view(320 * 240 * 2, &fb[0]));
}

void FirmwareBinder::Update(ButtonID button, int8_t knobValue)
{
    _menuSystem.Update(button, knobValue);
    _menuSystem.Draw(&_painter);
}

// Binding code
EMSCRIPTEN_BINDINGS(firmware_binder)
{
    emscripten::class_<FirmwareBinder>("FirmwareBinder")
        .constructor<>()
        .function("GetFB", &FirmwareBinder::GetFB)
        .function("Update", &FirmwareBinder::Update);

    emscripten::enum_<ButtonID>("ButtonID")
        .value("BUTTON_1_DOWN", ButtonID::BUTTON_1_DOWN)
        .value("BUTTON_1_UP", ButtonID::BUTTON_1_UP)
        .value("BUTTON_2_DOWN", ButtonID::BUTTON_2_DOWN)
        .value("BUTTON_2_UP", ButtonID::BUTTON_2_UP)
        .value("BUTTON_3_DOWN", ButtonID::BUTTON_3_DOWN)
        .value("BUTTON_3_UP", ButtonID::BUTTON_3_UP)
        .value("BUTTON_4_DOWN", ButtonID::BUTTON_4_DOWN)
        .value("BUTTON_4_UP", ButtonID::BUTTON_4_UP)
        .value("BUTTON_5_DOWN", ButtonID::BUTTON_5_DOWN)
        .value("BUTTON_5_UP", ButtonID::BUTTON_5_UP)
        .value("BUTTON_6_DOWN", ButtonID::BUTTON_6_DOWN)
        .value("BUTTON_6_UP", ButtonID::BUTTON_6_UP)
        .value("BUTTON_7_DOWN", ButtonID::BUTTON_7_DOWN)
        .value("BUTTON_7_UP", ButtonID::BUTTON_7_UP)
        .value("BUTTON_8_DOWN", ButtonID::BUTTON_8_DOWN)
        .value("BUTTON_8_UP", ButtonID::BUTTON_8_UP)
        .value("BUTTON_9_DOWN", ButtonID::BUTTON_9_DOWN)
        .value("BUTTON_9_UP", ButtonID::BUTTON_9_UP)
        .value("BUTTON_10_DOWN", ButtonID::BUTTON_10_DOWN)
        .value("BUTTON_10_UP", ButtonID::BUTTON_10_UP)
        .value("BUTTON_11_DOWN", ButtonID::BUTTON_11_DOWN)
        .value("BUTTON_11_UP", ButtonID::BUTTON_11_UP)
        .value("BUTTON_12_DOWN", ButtonID::BUTTON_12_DOWN)
        .value("BUTTON_12_UP", ButtonID::BUTTON_12_UP)
        .value("E1_UP", ButtonID::E_1_UP)
        .value("E1_DOWN", ButtonID::E_1_DOWN)
        .value("BUTTON_NONE", ButtonID::BUTTON_NONE);
}
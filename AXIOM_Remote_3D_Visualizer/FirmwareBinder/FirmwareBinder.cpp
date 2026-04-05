#include "FirmwareBinder.h"

#include <cstring>
#include <vector>

#include <emscripten.h>
#include <emscripten/bind.h>

#include "src/UI/ButtonDefinitions.h"
#include "src/UI/LvglUI.h"

#define ILI9341_TFTWIDTH 320
#define ILI9341_TFTHEIGHT 240

// Framebuffer (ARGB8888 - Matches LV_COLOR_DEPTH 32)
static uint32_t fb[ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT];

// Dirty rectangles
struct DirtyRect
{
    int x, y, w, h;
};

static std::vector<DirtyRect> dirtyRects;

static LvglUI& GetUI()
{
    static LvglUI instance;
    return instance;
}

/**
 * LVGL Flush Callback
 */
static void flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map)
{
    uint32_t* buf32 = reinterpret_cast<uint32_t*>(px_map);

    int32_t width = (area->x2 - area->x1 + 1);

    for (int32_t y = area->y1; y <= area->y2; y++)
    {
        uint32_t* line_fb = &fb[y * ILI9341_TFTWIDTH + area->x1];
        for (int32_t x = 0; x < width; x++)
        {
            uint32_t color = buf32[x];
            // LVGL ARGB8888 (0xAARRGGBB) in Little Endian memory is [B, G, R, A]
            // WebGL RGBA expects [R, G, B, A]
            // We swap Byte 0 (Blue) and Byte 2 (Red) in the uint32_t
            line_fb[x] = (color & 0xFF00FF00) |           // Keep Alpha and Green
                         ((color & 0x00FF0000) >> 16) |   // Move Red (Byte 2) to Byte 0
                         ((color & 0x000000FF) << 16);    // Move Blue to Byte 2
        }
        buf32 += width;
    }

    // Store dirty rect
    dirtyRects.push_back({
        area->x1,
        area->y1,
        width,
        area->y2 - area->y1 + 1
    });

    lv_display_flush_ready(disp);
}

FirmwareBinder::FirmwareBinder()
{
    lv_init();

    lv_display_t* disp = lv_display_create(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT);

    // Explicitly set 32-bit format to match our uint32_t framebuffer logic.
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_ARGB8888);
    // Using a full-screen draw buffer prevents clipping bugs across chunk boundaries
    static uint32_t draw_buf[ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT];
    lv_display_set_buffers(
        disp,
        draw_buf,
        NULL,
        sizeof(draw_buf),
        LV_DISPLAY_RENDER_MODE_PARTIAL
    );

    lv_display_set_flush_cb(disp, flush_cb);


    GetUI();

    emscripten_set_main_loop(
        []() {
            lv_tick_inc(16);
            lv_timer_handler();
        },
        0, false);
}

emscripten::val FirmwareBinder::GetFB()
{
    return emscripten::val(
        emscripten::typed_memory_view(
            ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT * sizeof(uint32_t),
            reinterpret_cast<uint8_t*>(fb)
        )
    );
}

void FirmwareBinder::Update(ButtonID button, int8_t knobValue, ButtonState buttonState)
{
    GetUI().TriggerButtonEvent(button, buttonState);

    if (button == ButtonID::KNOB)
    {
        LvglUI::UpdateEncoder(knobValue);
    }
}

// --- NEW: class methods ---

emscripten::val FirmwareBinder::GetDirtyRects()
{
    emscripten::val arr = emscripten::val::array();

    for (size_t i = 0; i < dirtyRects.size(); i++)
    {
        emscripten::val obj = emscripten::val::object();
        obj.set("x", dirtyRects[i].x);
        obj.set("y", dirtyRects[i].y);
        obj.set("w", dirtyRects[i].w);
        obj.set("h", dirtyRects[i].h);
        arr.set(i, obj);
    }

    return arr;
}

void FirmwareBinder::ClearDirtyRects()
{
    dirtyRects.clear();
}

void FirmwareBinder::SetDebugMarkersVisible(bool visible)
{
    GetUI().SetDebugMarkersVisible(visible);
}

EMSCRIPTEN_BINDINGS(firmware_binder)
{
    emscripten::enum_<ButtonID>("ButtonID")
        .value("BUTTON_NONE", ButtonID::BUTTON_NONE)
        .value("BUTTON_1", ButtonID::BUTTON_1)
        .value("BUTTON_2", ButtonID::BUTTON_2)
        .value("BUTTON_3", ButtonID::BUTTON_3)
        .value("BUTTON_4", ButtonID::BUTTON_4)
        .value("BUTTON_5", ButtonID::BUTTON_5)
        .value("BUTTON_6", ButtonID::BUTTON_6)
        .value("BUTTON_7", ButtonID::BUTTON_7)
        .value("BUTTON_8", ButtonID::BUTTON_8)
        .value("BUTTON_9", ButtonID::BUTTON_9)
        .value("BUTTON_10", ButtonID::BUTTON_10)
        .value("BUTTON_11", ButtonID::BUTTON_11)
        .value("BUTTON_12", ButtonID::BUTTON_12)
        .value("BUTTON_A", ButtonID::BUTTON_A)
        .value("BUTTON_B", ButtonID::BUTTON_B)
        .value("BUTTON_PHOTO", ButtonID::BUTTON_PHOTO)
        .value("BUTTON_RECORD", ButtonID::BUTTON_RECORD)
        .value("KNOB", ButtonID::KNOB);

    emscripten::enum_<ButtonState>("ButtonState")
        .value("Pressed", ButtonState::Pressed)
        .value("Released", ButtonState::Released);

    emscripten::class_<FirmwareBinder>("FirmwareBinder")
        .constructor<>()
        .function("GetFB", &FirmwareBinder::GetFB)
        .function("Update", &FirmwareBinder::Update)
        .function("GetDirtyRects", &FirmwareBinder::GetDirtyRects)
        .function("ClearDirtyRects", &FirmwareBinder::ClearDirtyRects)
        .function("SetDebugMarkersVisible", &FirmwareBinder::SetDebugMarkersVisible);
}

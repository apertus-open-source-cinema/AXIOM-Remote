#pragma once

#include "lvgl.h"

#include "ILvglPlatformDriver.h"

namespace Rp2040Config
{
constexpr int DisplayWidth    = 240;
constexpr int DisplayHeight   = 320;
using DrawBufferColorFormat   = lv_color16_t;
constexpr size_t DrawBufLines = DisplayHeight / 4;
constexpr size_t DrawBufSize  = DisplayWidth * DrawBufLines;
} // namespace Rp2040Config

class RP2040HALDriver : public ILvglPlatformDriver
{
  public:
    RP2040HALDriver();
    ~RP2040HALDriver() override;

    bool Initialize() override;
    lv_coord_t GetHorRes() const override
    {
        return Rp2040Config::DisplayWidth;
    }
    lv_coord_t GetVerRes() const override
    {
        return Rp2040Config::DisplayHeight;
    }
    lv_display_t* GetLvDisplay() const override
    {
        return m_lvglDisplay;
    }
    lv_indev_t* GetLvInputDevice() const override
    {
        return m_lvglInputDevice;
    }

    RP2040HALDriver(const RP2040HALDriver&)            = delete;
    RP2040HALDriver& operator=(const RP2040HALDriver&) = delete;
    RP2040HALDriver(RP2040HALDriver&&)                 = delete;
    RP2040HALDriver& operator=(RP2040HALDriver&&)      = delete;

  private:
    static Rp2040Config::DrawBufferColorFormat s_drawBuf1[Rp2040Config::DrawBufSize];
    static Rp2040Config::DrawBufferColorFormat s_drawBuf2[Rp2040Config::DrawBufSize];

    lv_display_t* m_lvglDisplay   = nullptr;
    lv_indev_t* m_lvglInputDevice = nullptr;

    bool InitializeDisplayHardware();
    bool InitializeInputHardware();

    static void FlushCallbackStatic(lv_display_t* disp, const lv_area_t* area, uint8_t* pixmap);
    static void InputReadCallbackStatic(lv_indev_t* indev, lv_indev_data_t* data);
};

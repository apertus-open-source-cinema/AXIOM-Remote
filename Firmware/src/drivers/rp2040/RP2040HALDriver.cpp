#include "RP2040HALDriver.h"
#include "../drivers/ili9341/ILI9341Display.h"

#include "pico/stdlib.h"
#include "pico/time.h"
#include <cstdio>
#include <src/display/lv_display.h>
#include <stdexcept>

ILI9341Display ili9341Display;

static constexpr uint32_t FLUSH_TE_WAIT_TIMEOUT_US = 30000;
volatile bool g_request_lvgl_refresh               = false;

Rp2040Config::DrawBufferColorFormat RP2040HALDriver::s_drawBuf1[Rp2040Config::DrawBufSize];
Rp2040Config::DrawBufferColorFormat RP2040HALDriver::s_drawBuf2[Rp2040Config::DrawBufSize];

RP2040HALDriver::RP2040HALDriver()
{
}

RP2040HALDriver::~RP2040HALDriver()
{
    printf("RP2040HALDriver destroyed.\n");
}

void te_interrupt_handler(uint gpio, uint32_t events)
{
    g_request_lvgl_refresh = true;
}

bool RP2040HALDriver::Initialize()
{
    printf("RP2040HALDriver Initializing...\n");

    if (!InitializeDisplayHardware())
    {
        printf("ERROR: Failed to initialize display hardware!\n");
        return false;
    }

    if (!InitializeInputHardware())
    {
        printf("WARNING: Failed to initialize input hardware.\n");
    }

    m_lvglDisplay = lv_display_create(Rp2040Config::DisplayWidth, Rp2040Config::DisplayHeight);
    if (!m_lvglDisplay)
    {
        printf("ERROR: lv_display_create failed!\n");
        return false;
    }

    lv_display_set_default(m_lvglDisplay);
    lv_display_set_rotation(m_lvglDisplay, LV_DISPLAY_ROTATION_90);
    lv_display_set_buffers(m_lvglDisplay, s_drawBuf1, s_drawBuf2, sizeof(s_drawBuf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(m_lvglDisplay, FlushCallbackStatic);

    printf("RP2040HALDriver Initialization Complete.\n");
    return true;
}

bool RP2040HALDriver::InitializeDisplayHardware()
{
    printf("Initializing ILI9341 Display Hardware...\n");
    ili9341Display.Init();
    sleep_ms(10);
    ili9341Display.MeasureTEInterval();
    printf("ILI9341 Display Hardware Initialized.\n");
    return true;
}

bool RP2040HALDriver::InitializeInputHardware()
{
    printf("Initializing Input Hardware (Placeholder)...\n");
    return true;
}

void RP2040HALDriver::FlushCallbackStatic(lv_display_t* pDisp, const lv_area_t* pArea, uint8_t* pixmap)
{
    int32_t width  = lv_area_get_width(pArea);
    int32_t height = lv_area_get_height(pArea);

    uint32_t overall_flush_start_us = time_us_32();
    uint32_t draw_pixmap_start_us   = time_us_32();

    while (ili9341Display.IsBusy())
    {
        tight_loop_contents();
    }

    ili9341Display.DrawPixmap(pArea->x1, pArea->y1, width, height, pixmap);
    uint32_t draw_pixmap_duration_us = time_us_32() - draw_pixmap_start_us;

    lv_display_flush_ready(pDisp);
}

void RP2040HALDriver::InputReadCallbackStatic(lv_indev_t* pIndev, lv_indev_data_t* pData)
{
    pData->state = LV_INDEV_STATE_RELEASED;
}

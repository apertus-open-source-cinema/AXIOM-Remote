#include "RP2040HALDriver.h"
#include "../drivers/ili9341/ILI9341Display.h" // Include the parallel display driver

#include "pico/stdlib.h"
#include <cstdio> // For printf
#include <src/display/lv_display.h>
#include <stdexcept> // Potentially for errors, though printf is used here

#include "pico/time.h" // For time_us_32
// --- Global Display Driver Instance ---
// This instance is used by the static FlushCallbackStatic.
// Ensure it's constructed before LVGL initialization needs it.
ILI9341Display ili9341Display;

static constexpr uint32_t FLUSH_TE_WAIT_TIMEOUT_US = 30000; // 30ms timeout for TE signal in flush

// volatile bool te_ready = false; // This flag appears unused by the current
// main loop logic
volatile bool g_request_lvgl_refresh = false; // Flag for main loop to request LVGL refresh

// --- Static Draw Buffers ---
// Define static members (ensure they are linked)
Rp2040Config::DrawBufferColorFormat Rp2040HALDriver::s_drawBuf1[Rp2040Config::DrawBufSize];
Rp2040Config::DrawBufferColorFormat Rp2040HALDriver::s_drawBuf2[Rp2040Config::DrawBufSize];

Rp2040HALDriver::Rp2040HALDriver()
{
    // Basic construction
}

Rp2040HALDriver::~Rp2040HALDriver()
{
    // Deinitialize hardware if needed (destructor of ili9341Display handles
    // PIO/DMA)
    printf("RP2040HALDriver destroyed.\n");
}

void te_interrupt_handler(uint gpio, uint32_t events)
{
    // printf("TE IRQ!\n"); // Uncomment for verbose TE interrupt logging
    // te_ready = true; // This flag is not directly used by the flush_cb or main
    // loop's active logic
    g_request_lvgl_refresh = true; // Signal the main loop
}

bool Rp2040HALDriver::Initialize() // Removed parameters
{
    printf("RP2040HALDriver Initializing...\n");

    // 1. Initialize Hardware
    if (!InitializeDisplayHardware()) {
        printf("ERROR: Failed to initialize display hardware!\n");
        return false;
    }
    if (!InitializeInputHardware()) {
        // Continue even if input fails for now, but log it
        printf("WARNING: Failed to initialize input hardware (using placeholder).\n");
    }

    // 2. Create LVGL Display Driver
    m_lvglDisplay = lv_display_create(Rp2040Config::DisplayWidth, Rp2040Config::DisplayHeight);
    if (!m_lvglDisplay) {
        printf("ERROR: lv_display_create failed!\n");
        return false;
    }

    lv_display_set_default(m_lvglDisplay);
    lv_display_set_rotation(m_lvglDisplay, LV_DISPLAY_ROTATION_90);
    // lv_display_delete_refr_timer(m_lvglDisplay);

    // 3. Set Display Buffers and Flush Callback
    // Pass the static buffers directly. Size is in bytes.
    lv_display_set_buffers(m_lvglDisplay, s_drawBuf1, s_drawBuf2, sizeof(s_drawBuf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_display_set_flush_cb(m_lvglDisplay, FlushCallbackStatic);

    // 4. Initialize Input Driver (Placeholder)
    // lv_indev_t* indev = lv_indev_create();
    // if (!indev) {
    //     printf("ERROR: lv_indev_create failed!\n");
    //     // Clean up display?
    //     return false;
    // }
    // lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); // Or BUTTON, ENCODER etc.
    // lv_indev_set_read_cb(indev, InputReadCallbackStatic);
    // No user_data needed for static callback approach here

    // gpio_set_irq_enabled_with_callback(ILI_PIN_TE, GPIO_IRQ_EDGE_RISE, true, &te_interrupt_handler);

    printf("RP2040HALDriver Initialization Complete.\n");
    return true;
}

// --- Hardware Initialization Helpers ---
bool Rp2040HALDriver::InitializeDisplayHardware()
{
    printf("Initializing ILI9341 Display Hardware...\n");
    // The global ili9341Display object's Init() method handles PIO, DMA, and
    // commands
    ili9341Display.Init(); // Assumes constructor claimed DMA channel successfully

    sleep_ms(10); // Add a small delay before measuring TE to allow display to
                  // stabilize
    ili9341Display.MeasureTEInterval();
    // Set desired orientation after Init
    // Use LANDSCAPE_X_FLIP for 320 width, 240 height based on common ILI9341
    // modules ili9341Display.SetOrientation(Orientation::LANDSCAPE_X_FLIP);
    // ili9341Display.SetOrientation(Orientation::LANDSCAPE_90);

    // Optional: Fill screen with a color after init to verify
    // uint16_t black = 0x0000;
    // ili9341Display.DrawPixmap(0, 0, Rp2040Config::DisplayWidth,
    // Rp2040Config::DisplayHeight, &black); // Needs a buffer Or implement a
    // FillRect function in ILI9341Display

    printf("ILI9341 Display Hardware Initialized.\n");
    return true; // Add error checking if Init() could fail and return bool
}

bool Rp2040HALDriver::InitializeInputHardware()
{
    // Placeholder: Initialize touch controller (e.g., XPT2046 via SPI)
    // or GPIO buttons here.
    printf("Initializing Input Hardware (Placeholder)...\n");
    return true; // Return false if actual init fails
}

// #define DEBUG_FLUSH_STATS
// --- Static and Instance Callbacks ---

/* static */ void Rp2040HALDriver::FlushCallbackStatic(lv_display_t* pDisp, const lv_area_t* pArea, uint8_t* pixmap)
{
    printf("FlushCallbackStatic called for area (%ld,%ld) to (%ld,%ld)\n", pArea->x1, pArea->y1, pArea->x2, pArea->y2);
    int32_t width = lv_area_get_width(pArea);
    int32_t height = lv_area_get_height(pArea);
    uint32_t overall_flush_start_us = time_us_32(); // Moved here for better total timing

    uint32_t draw_pixmap_start_us = time_us_32();

    // --- Intelligent TE Wait ---
    // The TE interrupt (rising edge) signals g_request_lvgl_refresh.
    // LVGL processes and then calls this flush callback.
    //
    // If TE pin is currently HIGH: We are in a V-Blank period. Proceed to draw.
    // This happens if LVGL processing was fast enough after the TE interrupt.
    //
    // If TE pin is currently LOW: The V-Blank (that might have triggered the
    // refresh) is over, and the display is in active scan. We MUST wait for the
    // *next* V-Blank (TE to go HIGH) to avoid tearing.
    //
    // Note: ILI_PIN_TE is defined in ILI9341Display.h
    // if (!gpio_get(ILI_PIN_TE)) { // If TE is LOW (not in V-Blank)
    //     uint32_t wait_start_us = time_us_32();
    //     // printf("FlushCB: TE LOW, waiting for HIGH...\n"); // Debug: very verbose
    //     while (!gpio_get(ILI_PIN_TE)) { // Wait for TE to go HIGH
    //         if (time_us_32() - wait_start_us > FLUSH_TE_WAIT_TIMEOUT_US) {
    //             // printf("FlushCB: TE Timeout waiting for HIGH\n"); // Debug: log
    //             // timeout
    //             break; // Proceed with drawing, may tear if timeout was too short or TE
    //                    // is stuck/misconfigured
    //         }
    //         tight_loop_contents(); // Essential for performance in tight loops on
    //                                // RP2040
    //     }
    // }
    // At this point, either TE was already high, or we've waited for it to become
    // high (or timed out). The original ili9341Display.WaitForTearingEffect()
    // (which waits low then high) is NOT called here.

    while (ili9341Display.IsBusy()) {
        tight_loop_contents(); // Use this to prevent the core from stalling completely
    }

    ili9341Display.DrawPixmap(pArea->x1, pArea->y1, width, height, pixmap);
    uint32_t draw_pixmap_duration_us = time_us_32() - draw_pixmap_start_us;
    printf("Pixmap drawn in %lu ms\n", draw_pixmap_duration_us / 1000);

    // IMPORTANT: Inform LVGL that the flushing is finished and the buffer is free
    lv_display_flush_ready(pDisp);

    // #define DEBUG_FLUSH_STATS // Uncomment this line to enable flush stats
    // printing Print flush statistics for debugging DMA and display performance
    // #ifdef DEBUG_FLUSH_STATS
    //     printf("Flush: Area(%ld,%ld %ldx%ld), DrawPixmap: %lu us, Total: %lu us\n", pArea->x1, pArea->y1, width,
    //     height,
    //            draw_pixmap_duration_us, (time_us_32() - overall_flush_start_us));
    // #endif

    //     static int flush_count = 0;
    //     flush_count++;
    //     if (overall_flush_start_us / 1000.0f >= 1000) {
    //         float fps = (flush_count * 1000.0f) / overall_flush_start_us / 1000.0f;
    //         printf("Display FPS: %.1f\n", fps);
    //         flush_count = 0;
    //     }
}

/* static */ void Rp2040HALDriver::InputReadCallbackStatic(lv_indev_t* pIndev, lv_indev_data_t* pData)
{
    // Placeholder: Read touch coordinates or button states
    // Example for no touch:
    pData->state = LV_INDEV_STATE_RELEASED;

    // If touch detected:
    // pData->state = LV_INDEV_STATE_PRESSED;
    // pData->point.x = touch_x;
    // pData->point.y = touch_y;

    // Example for button press:
    // uint32_t btn_id = GetPressedButton(); // Your function
    // if (btn_id != NO_BUTTON) {
    //     pData->state = LV_INDEV_STATE_PRESSED;
    //     pData->btn_id = btn_id;
    // } else {
    //     pData->state = LV_INDEV_STATE_RELEASED;
    // }
}

// Instance callbacks are not strictly needed with the static approach using a
// global driver object void Rp2040HALDriver::FlushCallbackImpl(const lv_area_t*
// pArea, uint8_t* pixmap) { ... } void
// Rp2040HALDriver::InputReadCallbackImpl(lv_indev_data_t* pData) { ... }

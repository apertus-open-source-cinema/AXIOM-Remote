#include "ILvglPlatformDriver.h"
#include <chrono>
#include <cstdio>
#include <cstring>
#include <memory>
#include <src/core/lv_refr.h>
#include <src/lv_api_map_v8.h>
#include <sstream>

#include <hardware/clocks.h>
#include <pico/stdlib.h>
#include <string>

#include "UI/LvglCore.h" // Assumed to handle lv_init()
#include "UI/LvglUI.h"
// #include "drivers/ili9341/ILI9341Display.h" // Driver included via
// RP2040HALDriver
#include "drivers/rp2040/RP2040HALDriver.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG 1

/**
 * @brief A custom idle percentage function for LVGL's system monitor.
 * Since we are on a bare-metal system without a full OS to track idle time,
 * this function can return a fixed value. Returning 0 implies 0% idle (100% CPU
 * usage).
 * @return uint32_t Idle percentage (0-100).
 */
uint32_t custom_get_idle_percent(void)
{
    return 0; // Or any other fixed value, e.g., 100 if you want to imply 0% CPU
              // usage. For FPS monitoring, the exact value here is less critical.
}

#ifdef __cplusplus
}
#endif

#define CMD_BUFFER_SIZE 64
char cmd_buffer[CMD_BUFFER_SIZE];
uint32_t cmd_buffer_idx = 0;

// Access the flag set by the TE interrupt in RP2040HALDriver.cpp
extern volatile bool g_request_lvgl_refresh;

std::unique_ptr<LvglUI> lvglUI;

// const int LVGL_TICK_PERIOD_MS = 10; // Original
const int LVGL_TICK_PERIOD_MS = 5; // Adjusted to match LV_DEF_REFR_PERIOD,
// or try 15ms

bool repeating_timer_callback(struct repeating_timer* t)
{
    LvglCore::tick_inc(LVGL_TICK_PERIOD_MS); // Convert microseconds to milliseconds
    // lv_task_handler() should be called in the main loop, not in an ISR context.
    return true; // Keep repeating
}

// --- FPS Counter Variables ---
static lv_obj_t* fps_label_obj = nullptr; // Renamed to avoid conflict if LvglUI has fps_label
static std::chrono::steady_clock::time_point fps_last_update_time;
static int fps_frame_count = 0;

static void setup_fps_display()
{
    if (lv_screen_active()) { // Ensure screen is active
        fps_label_obj = lv_label_create(lv_screen_active());
        lv_obj_set_align(fps_label_obj, LV_ALIGN_TOP_RIGHT); // Position top-right
        lv_obj_set_style_text_color(fps_label_obj, lv_color_white(), 0);
        lv_label_set_text(fps_label_obj, "FPS: --");
    }
}

// --- USB Connection Check (Keep as is) ---
bool is_stdio_usb_connected(uint32_t timeout_ms)
{
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) - start_time < timeout_ms) {
        if (stdio_usb_connected()) { // Check if USB CDC is connected.
            return true;
        }
        sleep_ms(10); // Poll every 10ms
    }
    return false; // Timeout
}

// --- Command Parsing (Keep as is, adapt ButtonID/Action mapping if needed) ---
void ParseAndProcessCommand(const char* command_str)
{
    printf("Processing command: %s\n", command_str); // Debug echo

    const char* separator = strchr(command_str, ':');
    if (!separator) {
        printf("Invalid command format (no :)\n");
        return;
    }

    std::string state_str(command_str, separator - command_str);
    std::string id_str(separator + 1);

    ButtonState state = ButtonState::Released; // Default
    if (state_str == "PRESS") {
        state = ButtonState::Pressed;
    }
    else if (state_str == "RELEASE") {
        state = ButtonState::Released;
    }
    else {
        printf("Invalid command state: %s\n", state_str.c_str());
        return; // Unknown state
    }

    // --- Map id_str to ButtonID ---
    // This mapping depends on your UI/Button definitions
    ButtonID targetButton = ButtonID::BUTTON_NONE; // Default
    if (id_str == "BUTTON_1")
        targetButton = ButtonID::BUTTON_1; // Example
    else if (id_str == "BUTTON_2")
        targetButton = ButtonID::BUTTON_2; // Example
    else if (id_str == "BUTTON_3")
        targetButton = ButtonID::BUTTON_3; // Example
    else if (id_str == "BUTTON_4")
        targetButton = ButtonID::BUTTON_4; // Example
    // Add mappings for all your command IDs

    if (lvglUI && targetButton != ButtonID::BUTTON_NONE) {
        printf("Triggering Event: Button=%d, State=%d\n", (int)targetButton, (int)state);
        lvglUI->TriggerButtonEvent(targetButton, state);
    }
    else if (!lvglUI) {
        printf("Error: lvglUI not initialized.\n");
    }
    else {
        printf("Unknown command ID: %s\n", id_str.c_str());
    }
}

uint32_t my_get_millis(void)
{
    return timer_hw->timelr / 1000.0f;
    // return (timer_hw->timehr << 32 | timer_hw->timelr) / 1000.0f;
}

int main()
{
    stdio_init_all(); // Initialize all stdio (USB, UART, etc.)

    // Optional USB connection check
    if (is_stdio_usb_connected(1000)) {
        printf("USB CDC connected!\n");
    }
    else {
        printf("USB CDC connection timed out.\n");
    }

    // Onboard LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    printf("=== AXIOM Remote Prototype v2 ===\n");
    printf("System Clock: %lu Hz\n", clock_get_hz(clk_sys));

    // --- Initialization Order ---
    // 1. LVGL Core (assumed to call lv_init())
    std::unique_ptr<LvglCore> lvglCore = std::make_unique<LvglCore>();
    if (!lvglCore) {
        printf("FATAL: Failed to create LvglCore\n");
        return -1;
    }
    printf("LvglCore created.\n");

    // 2. Platform Driver (initializes hardware and registers LVGL display driver)
    std::unique_ptr<Rp2040HALDriver> platformDriver = std::make_unique<Rp2040HALDriver>();
    if (!platformDriver) {
        printf("FATAL: Failed to create Rp2040HALDriver\n");
        return -1;
    }
    printf("Rp2040HALDriver created. Initializing...\n");
    if (!platformDriver->Initialize()) { // Call simplified Initialize
        printf("FATAL: Failed to initialize platform driver!\n");
        return -1;
    }
    printf("Platform Driver Initialized.\n");

    // --- Measure TE Interval (for debugging) ---
    // Access the global ili9341Display instance directly for this test
    // --- End TE Interval Measurement ---

    // 3. LVGL UI (creates widgets, etc.)
    lvglUI = std::make_unique<LvglUI>();
    if (!lvglUI) {
        printf("FATAL: Failed to create LvglUI\n");
        return -1;
    }
    printf("LvglUI created.\n");
    lv_tick_set_cb(my_get_millis);

    // 4. Initialize Theme (if separate from LvglUI creation)
    InitializeTheme(); // Make sure this is defined and
    // Explicitly invalidate the screen after all UI and theme initialization.
    // This ensures that lv_task_handler() or lv_refr_now() sees a dirty screen
    // for the first frame.
    // if (platformDriver && platformDriver->GetLvDisplay()) {
    //     lv_display_t* disp_to_invalidate = platformDriver->GetLvDisplay();
    //     lv_obj_t* active_screen_on_disp = lv_display_get_screen_active(disp_to_invalidate);
    //     if (active_screen_on_disp) {
    //         lv_obj_invalidate(active_screen_on_disp);
    //         printf("Main.cpp: Active screen on display explicitly invalidated for "
    //                "initial draw.\n");
    //     }
    // }
    // static struct repeating_timer lvgl_tick_timer;
    // if (!add_repeating_timer_ms(LVGL_TICK_PERIOD_MS, // Negative for repeating, positive for one-shot
    //                             repeating_timer_callback,
    //                             NULL, // No user data passed to callback
    //                             &lvgl_tick_timer)) {
    //     printf("FATAL: Failed to add LVGL tick timer\n");
    //     // Handle error, perhaps by falling back to main loop ticking or halting
    // }

    // setup_fps_display(); // Create the FPS label
    // fps_last_update_time = std::chrono::steady_clock::now(); // Initialize FPS
    // timer

    // --- Main Loop ---
    printf("Entering main loop...\n");
    while (true) {
        // uint32_t lv_handler_start_us = time_us_32();
        lv_timer_handler(); // Process LVGL tasks, events, and rendering.
        // uint32_t lv_handler_duration_us = time_us_32() - lv_handler_start_us;
        //  printf("LVGL handler took %lu ms\n", lv_handler_duration_us / 1000);
        //  Check if TE interrupt requested an LVGL refresh
        //  if (g_request_lvgl_refresh) {
        //      g_request_lvgl_refresh = false; // Consume the flag
        //      if (platformDriver && platformDriver->GetLvDisplay()) {
        //          // printf("MainLoop: Calling lv_display_refr_timer() due to TE
        //          // pulse.\n"); // Verbose lv_refr_now(platformDriver->GetLvDisplay());
        //          // // lv_refr_now is immediate, can be heavy in main loop
        //          lv_display_refr_timer(nullptr); // Schedules a refresh for the next lv_task_handler call
        //      }
        //  }

        // Periodically print lv_task_handler duration, e.g., once per second along
        // with FPS This will be grouped with the FPS update logic below.

        // --- FPS Calculation ---
        // #ifdef DEBUG
        //         fps_frame_count++;
        //         auto current_time = std::chrono::steady_clock::now();
        //         auto elapsed_ms =
        //             std::chrono::duration_cast<std::chrono::milliseconds>(current_time -
        //             fps_last_update_time).count();

        //         if (elapsed_ms >= 1000) { // Update every second
        //             float fps_val = static_cast<float>(fps_frame_count * 1000.0f) / elapsed_ms;
        //             if (fps_label_obj) {
        //                 char fps_buf[32]; // Increased buffer size for handler time
        //                 snprintf(fps_buf, sizeof(fps_buf), "FPS: %.1f\nHandler: %lu us", fps_val,
        //                 lv_handler_duration_us); lv_label_set_text(fps_label_obj, fps_buf);
        //             }
        //             // Optional: Print to console as well
        //             // printf("FPS: %.1f, LVGL Handler Time: %lu us\n", fps_val, lv_handler_duration_us);

        //             fps_frame_count = 0;
        //             fps_last_update_time = current_time;
        //         }
        // #endif
        // --- Sleep ---
        sleep_ms(LVGL_TICK_PERIOD_MS);
    }

    // Should not reach here
    return 0;
}

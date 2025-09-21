#include <cstdint>
#include <cstdio>
#include <memory>

#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <hardware/structs/clocks.h>
#include <pico/stdio_usb.h>
#include <pico/time.h>

#include "LvglCore.h"
#include "UI/LvglUI.h"
#include "drivers/rp2040/RP2040HALDriver.h"

const int LVGL_TICK_PERIOD_MS = 5;

bool is_stdio_usb_connected(uint32_t timeout_ms)
{
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) - start_time < timeout_ms) {
        if (stdio_usb_connected()) {
            return true;
        }
        sleep_ms(10);
    }
    return false;
}

uint32_t get_system_milliseconds(void)
{
    return timer_hw->timelr / 1000.0f;
}

int main()
{
    stdio_init_all();

    if (is_stdio_usb_connected(1000)) {
        printf("USB CDC connected!\n");
    }
    else {
        printf("USB CDC connection timed out.\n");
    }

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    printf("=== AXIOM Remote Prototype v2 ===\n");
    printf("System Clock: %lu Hz\n", clock_get_hz(clk_sys));

    std::unique_ptr<LvglCore> lvglCore = std::make_unique<LvglCore>();
    if (!lvglCore) {
        printf("FATAL: Failed to create LvglCore\n");
        return -1;
    }
    printf("LvglCore created.\n");

    std::unique_ptr<RP2040HALDriver> platformDriver = std::make_unique<RP2040HALDriver>();
    if (!platformDriver) {
        printf("FATAL: Failed to create Rp2040HALDriver\n");
        return -1;
    }
    printf("Rp2040HALDriver created. Initializing...\n");
    if (!platformDriver->Initialize()) {
        printf("FATAL: Failed to initialize platform driver!\n");
        return -1;
    }
    printf("Platform Driver Initialized.\n");

    std::unique_ptr<LvglUI> lvglUI = std::make_unique<LvglUI>();
    if (!lvglUI) {
        printf("FATAL: Failed to create LvglUI\n");
        return -1;
    }

    lv_tick_set_cb(get_system_milliseconds);
    printf("LvglUI created.\n");

    InitializeTheme();

    printf("Entering main loop...\n");
    while (true) {
        lv_timer_handler();
        sleep_ms(LVGL_TICK_PERIOD_MS);
    }

    return 0;
}

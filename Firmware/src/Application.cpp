#include "Application.h"
#include "UI/LvglUI.h"
#include "drivers/rp2040/RP2040HALDriver.h"

#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <pico/stdio_usb.h>
#include <pico/time.h>

const int LVGL_TICK_PERIOD_MS = 16;

Application::Application() = default;

Application::~Application() {
    shutdown();
}

bool Application::initializeHardware()
{
    stdio_init_all();
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    return true;
}

bool Application::initializeLVGL()
{
    lv_init();

    platformDriver = std::make_unique<RP2040HALDriver>();
    if (!platformDriver || !platformDriver->Initialize()) {
        printf("Failed to initialize LVGL platform driver!\n");
        return false;
    }

    return true;
}

bool Application::initializeUI()
{
    lvglUI = std::make_unique<LvglUI>();
    return lvglUI != nullptr;
}

bool Application::initialize()
{
    printf("Initializing AXIOM Remote Application...\n");

    if (!initializeHardware()) {
        printf("Hardware initialization failed!\n");
        return false;
    }

    if (!initializeLVGL()) {
        printf("LVGL initialization failed!\n");
        return false;
    }


    if (!initializeUI()) {
        printf("UI initialization failed!\n");
        return false;
    }

    initialized = true;
    printf("Application initialized successfully!\n");
    return true;
}

void Application::run()
{
    printf("Starting main loop...\n");
    runMainLoop();
}

void Application::runMainLoop()
{
    while (true) {
        lv_timer_handler();
        sleep_ms(LVGL_TICK_PERIOD_MS);
    }
}

void Application::shutdown()
{
    if (initialized) {
        printf("Shutting down application...\n");
        lvglUI.reset();
        platformDriver.reset();
        initialized = false;
    }
}
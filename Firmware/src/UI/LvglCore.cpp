#include "LvglCore.h"

#include <stdexcept> // For runtime_error (optional)

#include <lvgl.h>

// Include lv_conf.h if needed
// #include "lv_conf.h"

LvglCore::LvglCore() {
    lv_init();
    // Add custom memory manager initialization here if you use one
    // lv_mem_init(...);
    // SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "LVGL Core Initialized."); // Requires SDL header
}

LvglCore::~LvglCore() {
    // SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Deinitializing LVGL Core."); // Requires SDL header
    // lv_deinit(); // Usually called to clean up all LVGL resources
}

void LvglCore::tick_inc(uint32_t millisec_elapsed) {
    lv_tick_inc(millisec_elapsed);
}

uint32_t LvglCore::timer_handler() {
    return lv_timer_handler();
}
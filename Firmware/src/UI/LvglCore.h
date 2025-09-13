#pragma once
#include <cstdint>

class LvglCore {
    public:
        // Initializes LVGL on construction
        LvglCore();
    
        // Deinitializes LVGL on destruction (RAII)
        ~LvglCore();
    
        // Non-copyable and non-movable
        LvglCore(const LvglCore&) = delete;
        LvglCore& operator=(const LvglCore&) = delete;
        LvglCore(LvglCore&&) = delete;
        LvglCore& operator=(LvglCore&&) = delete;
    
        /**
         * @brief Provides a tick source for LVGL timekeeping.
         * Call this periodically (e.g., every 1ms or more) from your main loop or a timer interrupt.
         * @param millisec_elapsed Milliseconds elapsed since the last call.
         */
        static void tick_inc(uint32_t millisec_elapsed);
    
        /**
         * @brief Handles LVGL tasks like animations, input handling, and rendering updates.
         * Call this frequently in your main application loop.
         * @return Time in milliseconds until the next task needs to run (can be used for sleep optimisation).
         */
        static uint32_t timer_handler();
    };
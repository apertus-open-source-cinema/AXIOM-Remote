#pragma once

#include <cstdint>
#include <cstdio>

// Error codes for the AXIOM Remote firmware
enum class ErrorCode {
    SUCCESS = 0,
    HARDWARE_INIT_FAILED,
    UI_INIT_FAILED,
    DRIVER_INIT_FAILED,
    USB_INIT_FAILED,
    GPIO_INIT_FAILED,
    LVGL_INIT_FAILED,
    THEME_INIT_FAILED,
    CONFIG_INIT_FAILED,
    EVENT_INIT_FAILED,
    POWER_INIT_FAILED,
    LOGGER_INIT_FAILED
};

// Error handling framework for robust error management
class ErrorHandler {
public:
    // Handle critical errors that require system reset or safe mode
    static void handleCriticalError(ErrorCode code, const char* message);

    // Handle recoverable errors that can be logged and handled gracefully
    static void handleRecoverableError(ErrorCode code, const char* message);

    // Generic error handler - routes to appropriate handler based on severity
    static void handleError(ErrorCode code, const char* message);
};
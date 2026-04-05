#include "ErrorHandler.h"
#include "Logger.h"
#include <hardware/gpio.h>
#include <pico/time.h>

void ErrorHandler::handleCriticalError(ErrorCode code, const char* message) {
    printf("CRITICAL ERROR [%d]: %s\n", static_cast<int>(code), message);
    LOG_CRITICAL("ERROR_HANDLER", message);

    // In a real system, you might trigger a watchdog reset or safe mode
    // For now, enter an error blink pattern
    while (true) {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(100);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(100);
    }
}

void ErrorHandler::handleRecoverableError(ErrorCode code, const char* message) {
    printf("ERROR [%d]: %s\n", static_cast<int>(code), message);
    LOG_ERROR("ERROR_HANDLER", message);
    // Log error and attempt recovery - could implement retry logic here
}

void ErrorHandler::handleError(ErrorCode code, const char* message) {
    // For now, treat all errors as recoverable
    // In production, you might have different handling based on error severity
    handleRecoverableError(code, message);
}
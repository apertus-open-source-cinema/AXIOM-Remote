#include <cstdio>
#include <hardware/clocks.h>

#include "Application.h"
#include "Logger.h"

int main()
{
    printf("=== AXIOM Remote Prototype v2 ===\n");
    printf("System Clock: %lu Hz\n", clock_get_hz(clk_sys));

    LOG_INFO("MAIN", "Starting AXIOM Remote application");

    Application app;

    // Initialize application with error handling
    if (!app.initialize()) {
        LOG_CRITICAL("MAIN", "Application initialization failed!");
        printf("FATAL: Application initialization failed!\n");
        return -1;
    }

    LOG_INFO("MAIN", "Application initialized successfully, starting main loop");

    // Run the application
    app.run();

    // Clean shutdown (though main loop is infinite)
    app.shutdown();

    LOG_INFO("MAIN", "Application shutdown complete");
    return 0;
}

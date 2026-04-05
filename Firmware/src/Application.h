#pragma once

#include <memory>
#include <cstdint>

// Forward declarations
class RP2040HALDriver;
class LvglUI;

// Simplified Application class
class Application {
private:
    std::unique_ptr<RP2040HALDriver> platformDriver;
    std::unique_ptr<LvglUI> lvglUI;

    bool initialized = false;

    // Simplified initialization
    bool initializeHardware();
    bool initializeLVGL();
    bool initializeUI();

    // Main loop
    void runMainLoop();

public:
    Application();
    ~Application();

    // Main lifecycle
    bool initialize();
    void run();
    void shutdown();
};
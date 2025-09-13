#pragma once
#include <functional>   // For std::function (callbacks)
#include <string>       // For state values like shutter speed
#include <vector>       // For holding options
#include <atomic>       // Optional: If state might be accessed from multiple threads
#include "ButtonDefinitions.h"
#include "UIIdentifiers.h" // Bring in ScreenID and UIAction::Identifier

class AppContext {
public:
    // --- Type definitions for Callbacks ---
    // Called whenever relevant application state changes
    using StateChangeCallback = std::function<void()>;
    // Called when an action requests navigation to a different screen
    using ScreenChangeRequestHandler = std::function<void(ScreenID screenId, bool animate)>;

    AppContext(); // Constructor for defaults

    // --- Action Processing ---
    // Single entry point called by input handlers (VirtualUI, hardware handlers)
    void ProcessAction(UIAction action);

    // --- State Getters (for UI display) ---
    int GetFpsValue() const;
    std::string GetAGainValue() const;
    std::string GetDGainValue() const;
    std::string GetShutterValue() const;
    std::string GetWbValue() const;
    // Add getters for any other state your UI needs...

    // --- Callback Registration ---
    // The UI layer (e.g., LvglAppUI/ScreenManager) calls this to know when to update
    void SetStateChangeCallback(StateChangeCallback callback);
    // The owner/creator (e.g., main or LvglAppUI) calls this to link screen change requests
    void SetScreenChangeRequestHandler(ScreenChangeRequestHandler handler);

private:
    // --- Internal State Variables ---
    // Examples based on UI screenshot. Use appropriate types.
    // std::atomic might be overkill if strictly single-threaded access.
    std::atomic<int> m_fpsValue;
    std::atomic<int> m_aGainIndex;
    std::atomic<int> m_dGainIndex;
    std::atomic<int> m_shutterIndex;
    std::atomic<int> m_wbIndex;
    // Add more state variables as needed...

    // --- Lookup Data (if using indices for state) ---
    const std::vector<std::string> m_aGainOptions;
    const std::vector<std::string> m_dGainOptions;
    const std::vector<std::string> m_shutterOptions;
    const std::vector<std::string> m_wbOptions;

    // --- Stored Callback Functions ---
    StateChangeCallback m_stateChangeCallback = nullptr;
    ScreenChangeRequestHandler m_screenChangeRequestHandler = nullptr;

    // --- Internal Helper Methods ---
    void NotifyStateChanged(); // Call after modifying state
    void RequestScreenChange(ScreenID targetId, bool animate = true); // Call to ask ScreenManager to switch

    // --- Specific Action Handlers --- (Called by ProcessAction)
    void HandleFPSAction();
    void HandleAGainAction();
    void HandleDGainAction();
    void HandleMenuAction();
    void HandleShutterAction();
    void HandleWbAction();
};

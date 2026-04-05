#include "AppContext.h"
#include "UIIdentifiers.h"

#include <cstdio>
#include <vector>

// --- Constructor ---
AppContext::AppContext()
    : m_fpsValue(24),                                      // Initialize with default values
      m_aGainIndex(0), m_dGainIndex(0), m_shutterIndex(1), // Default to 1/100 example
      m_wbIndex(0),
      // Initialize lookup tables
      m_aGainOptions({"Low", "Medium", "High"}), m_dGainOptions({"0dB", "+3dB", "+6dB", "+9dB"}),
      m_shutterOptions({"1/50", "1/100", "1/200", "1/500"}), m_wbOptions({"Auto", "Daylight", "Cloudy", "Tungsten"})
{
    printf("AppContext created\n");
}

// --- Callback Setters ---
void AppContext::SetStateChangeCallback(StateChangeCallback callback)
{
    m_stateChangeCallback = callback;
}

void AppContext::SetScreenChangeRequestHandler(ScreenChangeRequestHandler handler)
{
    m_screenChangeRequestHandler = handler;
}

// --- State Getters ---
int AppContext::GetFpsValue() const
{
    return m_fpsValue.load();
}

std::string AppContext::GetAGainValue() const
{
    int index = m_aGainIndex.load();
    return (index >= 0 && index < m_aGainOptions.size()) ? m_aGainOptions[index] : "ERR";
}

std::string AppContext::GetDGainValue() const
{
    int index = m_dGainIndex.load();
    return (index >= 0 && index < m_dGainOptions.size()) ? m_dGainOptions[index] : "ERR";
}

std::string AppContext::GetShutterValue() const
{
    int index = m_shutterIndex.load();
    return (index >= 0 && index < m_shutterOptions.size()) ? m_shutterOptions[index] : "ERR";
}

std::string AppContext::GetWbValue() const
{
    int index = m_wbIndex.load();
    return (index >= 0 && index < m_wbOptions.size()) ? m_wbOptions[index] : "ERR";
}
// ... other getters ...

// --- Internal Notification Helpers ---
void AppContext::NotifyStateChanged()
{
    printf("AppContext: Notifying state change.");
    if (m_stateChangeCallback)
    {
        m_stateChangeCallback(); // Call the registered callback (likely LvglUI::UpdateDisplay)
    }
}

void AppContext::RequestScreenChange(ScreenID targetId, AnimationDirection direction)
{
    printf("AppContext: Requesting screen change to %d with direction %d.", (int)targetId, (int)direction);
    if (m_screenChangeRequestHandler)
    {
        // Call the registered handler (likely calls ScreenManager::SwitchToScreen)
        m_screenChangeRequestHandler(targetId, direction);
    }
    else
    {
        printf("AppContext: No ScreenChangeRequestHandler set.");
    }
}

// Overload for backward compatibility and pending animation direction
void AppContext::RequestScreenChange(ScreenID targetId)
{
    RequestScreenChange(targetId, m_pendingAnimationDirection);
    m_pendingAnimationDirection = AnimationDirection::NONE; // Reset
}

// --- Action Processing Dispatcher ---
void AppContext::ProcessAction(UIAction action)
{
    printf("AppContext: Processing action %d\n", (int)action);
    // Based on the action ID, call the appropriate internal handler
     switch (action) {
        case UIAction::OPEN_SETTINGS_MENU:
            HandleMenuAction();
            break;
        case UIAction::OPEN_SHUTTER_SETTINGS:
            HandleShutterSettingsAction();
            break;
        case UIAction::OPEN_WHITE_BALANCE_SETTINGS:
            HandleWhiteBalanceSettingsAction();
            break;
        case UIAction::OPEN_DEBUG_SCREEN:
            RequestScreenChange(ScreenID::DEBUG, AnimationDirection::NONE);
            break;
        case UIAction::GO_TO_PREVIOUS_SCREEN:
            HandleGoBackAction();
            break;
    //     case UIAction::Identifier::ACTION_1_FPS:     HandleFPSAction();     break;
    //     case UIAction::Identifier::ACTION_2_A_GAIN:  HandleAGainAction();   break;
    //     case UIAction::Identifier::ACTION_3_D_GAIN:  HandleDGainAction();   break;
    //     case UIAction::Identifier::ACTION_4_MENU:    HandleMenuAction();    break;
    //     case UIAction::Identifier::ACTION_5_SHUTTER: HandleShutterAction(); break;
    //     case UIAction::Identifier::ACTION_6_WB:      HandleWbAction();      break;
    //     case UIAction::Identifier::NONE:
    //     default:
    //          SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "AppContext: Unhandled action ID %d", (int)actionId);
    //         break;
    }
}

void AppContext::ProcessActionWithAnimation(UIAction action, AnimationDirection direction)
{
    // Store the animation direction for the next screen change
    m_pendingAnimationDirection = direction;
    ProcessAction(action);
}

// --- Individual Action Handlers ---
// These contain the actual logic for what happens when a button is pressed.
// They modify the internal state (m_... variables) and then call NotifyStateChanged().
// They can also request screen changes.

void AppContext::HandleFPSAction()
{
    // Example: Simple toggle for demonstration
    m_fpsValue = (m_fpsValue == 24) ? 30 : 24;
    printf("Action: FPS set to %d\n", m_fpsValue.load());
    NotifyStateChanged(); // Tell the UI to update
}

void AppContext::HandleAGainAction()
{
    // Example: Cycle through options
    int current_index = m_aGainIndex.load();
    int next_index = (current_index + 1) % m_aGainOptions.size();
    m_aGainIndex = next_index;
    printf("Action: A.Gain set to %s\n", GetAGainValue().c_str());
    NotifyStateChanged();
}

void AppContext::HandleDGainAction()
{
    int current_index = m_dGainIndex.load();
    int next_index = (current_index + 1) % m_dGainOptions.size();
    m_dGainIndex = next_index;
    printf("Action: D.Gain set to %s", GetDGainValue().c_str());
    NotifyStateChanged();
}

void AppContext::HandleMenuAction()
{
    printf("Action: MENU pressed.\n");
    // This action requests navigation, handled by ScreenManager via callback
    RequestScreenChange(ScreenID::SETTINGS_MENU, AnimationDirection::NONE);
    // It might also change state, e.g., m_isMenuOpen = true; NotifyStateChanged();
}

void AppContext::HandleShutterAction()
{
    int current_index = m_shutterIndex.load();
    int next_index = (current_index + 1) % m_shutterOptions.size();
    m_shutterIndex = next_index;
    printf("Action: Shutter set to %s\n", GetShutterValue().c_str());
    NotifyStateChanged();
}

void AppContext::HandleWbAction()
{
    int current_index = m_wbIndex.load();
    int next_index = (current_index + 1) % m_wbOptions.size();
    m_wbIndex = next_index;
    printf("Action: WB set to %s\n", GetWbValue().c_str());
    NotifyStateChanged();
}

void AppContext::HandleShutterSettingsAction()
{
    printf("Action: Open Shutter Settings\n");
    RequestScreenChange(ScreenID::SHUTTER_SPEED);
}

void AppContext::HandleWhiteBalanceSettingsAction()
{
    printf("Action: Open White Balance Settings\n");
    RequestScreenChange(ScreenID::WHITE_BALANCE);
}

void AppContext::HandleGoBackAction()
{
    printf("Action: Go Back\n");
    RequestScreenChange(ScreenID::MAIN); // For now, just go back to main
}
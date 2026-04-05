#pragma once
#include <functional>
#include <string>
#include <vector>
#include <atomic>
#include "ButtonDefinitions.h"
#include "UIIdentifiers.h"

enum class AnimationDirection
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE
};

class AppContext {
public:
    using StateChangeCallback = std::function<void()>;
    using ScreenChangeRequestHandler = std::function<void(ScreenID screenId, AnimationDirection direction)>;

    AppContext();

    void ProcessAction(UIAction action);
    void ProcessActionWithAnimation(UIAction action, AnimationDirection direction);

    int GetFpsValue() const;
    std::string GetAGainValue() const;
    std::string GetDGainValue() const;
    std::string GetShutterValue() const;
    std::string GetWbValue() const;

    void SetStateChangeCallback(StateChangeCallback callback);
    void SetScreenChangeRequestHandler(ScreenChangeRequestHandler handler);

private:
    std::atomic<int> m_fpsValue;
    std::atomic<int> m_aGainIndex;
    std::atomic<int> m_dGainIndex;
    std::atomic<int> m_shutterIndex;
    std::atomic<int> m_wbIndex;

    const std::vector<std::string> m_aGainOptions;
    const std::vector<std::string> m_dGainOptions;
    const std::vector<std::string> m_shutterOptions;
    const std::vector<std::string> m_wbOptions;

    StateChangeCallback m_stateChangeCallback = nullptr;
    ScreenChangeRequestHandler m_screenChangeRequestHandler = nullptr;
    AnimationDirection m_pendingAnimationDirection = AnimationDirection::NONE;

    void NotifyStateChanged();
    void RequestScreenChange(ScreenID targetId, AnimationDirection direction);
    void RequestScreenChange(ScreenID targetId);

    void HandleFPSAction();
    void HandleAGainAction();
    void HandleDGainAction();
    void HandleMenuAction();
    void HandleShutterAction();
    void HandleWbAction();
    void HandleShutterSettingsAction();
    void HandleWhiteBalanceSettingsAction();
    void HandleGoBackAction();
};

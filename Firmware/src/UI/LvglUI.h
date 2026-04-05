#pragma once

#include <lvgl.h>
#include <memory>
#include <vector>

#include "AppContext.h"
#include "Screens/BaseScreen.h"

// Forward declaration
class DebugScreen;

// Simplified UI class
class LvglUI
{
private:
    std::unique_ptr<AppContext> _appContext;
    lv_obj_t* _tileView        = nullptr;
    lv_indev_t* _encoderDevice = nullptr;
    lv_indev_t* _buttonDevice  = nullptr;
    static void ButtonReadCallback(lv_indev_t* drv, lv_indev_data_t* data);

    // Screen instances
    std::unique_ptr<BaseScreen> _mainScreen;
    std::unique_ptr<BaseScreen> _settingsScreen;
    std::unique_ptr<BaseScreen> _whiteBalanceScreen;
    std::unique_ptr<BaseScreen> _shutterScreen;
    std::unique_ptr<DebugScreen> _debugScreen;

    lv_group_t* _inputGroup = nullptr;

    static void EncoderReadCallback(lv_indev_t* drv, lv_indev_data_t* data);
    void SwitchToScreen(uint8_t col, uint8_t row, AnimationDirection direction = AnimationDirection::NONE);

    // Helper methods for button events
    int MapButtonIDToIndex(ButtonID id) const;
    BaseScreen* GetActiveScreen() const;

    static LvglUI* _instance;

    // Static state for debug
    static int32_t _encoderPosition;
    static bool _encoderButtonPressed;
    static const std::vector<lv_point_t> _button_points;

    static uint8_t ReadPhysicalButtons();

    // Map storing button states
    static int _pressedBtnId;
    static lv_indev_state_t _btnState;

    std::vector<lv_obj_t*> _debugMarkerObjects;

public:
    void SetDebugMarkersVisible(bool visible);
    LvglUI();
    ~LvglUI();

    void InitializeScreens();

    void TriggerButtonEvent(ButtonID id, ButtonState state) const;
    static void UpdateEncoder(int32_t delta);

    // Static getters for debug screen
    static int32_t GetEncoderPosition();
    static bool IsEncoderButtonPressed();
    static const std::vector<lv_point_t>& GetButtonPoints();
};

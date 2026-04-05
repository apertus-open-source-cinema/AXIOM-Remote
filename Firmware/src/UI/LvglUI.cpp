#include "LvglUI.h"

#include <iostream>
#include <memory>

#include "AppContext.h"
#include "ButtonDefinitions.h"
#include "Media/ApertusLogo_full.h"
#include "Screens/DebugScreen.h"
#include "Screens/MainScreen.h"
#include "Screens/SettingsMenuScreen.h"
#include "Screens/ShutterScreen.h"
#include "Screens/WhiteBalanceScreen.h"
#include "Theme.h"

#ifndef LV_HOR_RES
#define LV_HOR_RES 320
#define LV_VER_RES 240
#endif

// Define static members
int32_t LvglUI::_encoderPosition   = 0;
bool LvglUI::_encoderButtonPressed = false;
int LvglUI::_pressedBtnId          = 0;
lv_indev_state_t LvglUI::_btnState = LV_INDEV_STATE_RELEASED;
LvglUI* LvglUI::_instance          = nullptr;

const std::vector<lv_point_t> LvglUI::_button_points = {
    // 3x Left (IDs 0-2), 3x Right (IDs 3-5), 3x Top (IDs 6-8), 3x Bottom (IDs 9-11)
    {60, 10},  {160, 10},  {260, 10},  // Top (Buttons 1, 2, 3) - Index 0, 1, 2
    {60, 230}, {160, 230}, {260, 230}, // Bottom (Buttons 4, 5, 6) - Index 3, 4, 5
    {10, 70},  {10, 120},  {10, 170},  // Left (Buttons 7, 8, 9) - Index 6, 7, 8
    {310, 70}, {310, 120}, {310, 170}, // Right (Buttons 10, 11, 12) - Index 9, 10, 11
    {10, 10},  {10, 10},               // Buttons A and B (virtual/special) - Index 12, 13
    {0, 0},    {0, 0},     {0, 0},     // Photo (14), Record (15), Knob (16)
    {50, 50}                           // Button 17 (Encoder Press)
};

void LvglUI::SetDebugMarkersVisible(bool visible)
{
    if (_debugMarkerObjects.empty() && visible)
    {
        for (size_t i = 0; i < _button_points.size(); i++)
        {
            // Create a small dot on the system layer
            lv_obj_t* dot = lv_obj_create(lv_layer_sys());
            lv_obj_set_size(dot, 6, 6);
            lv_obj_set_style_bg_color(dot, lv_color_hex(0xFF0000), 0);
            lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
            lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
            lv_obj_set_style_border_width(dot, 0, 0);
            lv_obj_remove_flag(dot, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_pos(dot, _button_points[i].x - 3, _button_points[i].y - 3);
            _debugMarkerObjects.push_back(dot);
            
            // Add a label with the Index ID
            lv_obj_t* label = lv_label_create(lv_layer_sys());
            lv_label_set_text_fmt(label, "%d", (int)i);
            lv_obj_set_style_text_color(label, lv_color_hex(0xFF0000), 0);
            lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
            lv_obj_align_to(label, dot, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
            lv_obj_remove_flag(label, LV_OBJ_FLAG_CLICKABLE);
            _debugMarkerObjects.push_back(label);
        }
    }

    for (auto* obj : _debugMarkerObjects)
    {
        if (visible) lv_obj_remove_flag(obj, LV_OBJ_FLAG_HIDDEN);
        else lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}

LvglUI::LvglUI()
{
    _instance = this;
    InitializeTheme();

    _appContext = std::make_unique<AppContext>();
    _appContext->SetScreenChangeRequestHandler([&](ScreenID id, AnimationDirection direction) {
        // Map ScreenID to screen instances
        switch (id)
        {
        case ScreenID::MAIN:
            SwitchToScreen(1, 0, direction); // tile (1,0)
            break;
        case ScreenID::SETTINGS_MENU:
            SwitchToScreen(2, 0, direction); // tile (2,0)
            break;
        case ScreenID::WHITE_BALANCE:
            SwitchToScreen(2, 1, direction); // tile (2,1) - below main row
            break;
        case ScreenID::SHUTTER_SPEED:
            SwitchToScreen(1, 1, direction); // tile (1,1) - below main screen
            break;
        case ScreenID::DEBUG:
            SwitchToScreen(0, 0, direction); // tile (0,0) - left of main screen
            break;
        default:
            break;
        }
    });

    // Create Input Group
    _inputGroup = lv_group_create();
    lv_group_set_default(_inputGroup);

    // Create tile view for screen management
    _tileView = lv_tileview_create(lv_screen_active());
    lv_obj_set_size(_tileView, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(_tileView, 0, 0);
    lv_obj_remove_flag(_tileView, LV_OBJ_FLAG_SCROLLABLE);

    InitializeScreens();

    // Create encoder input device
    _encoderDevice = lv_indev_create();
    lv_indev_set_type(_encoderDevice, LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(_encoderDevice, EncoderReadCallback);
    lv_indev_set_group(_encoderDevice, _inputGroup);

    // Create button input device
    _buttonDevice = lv_indev_create();
    lv_indev_set_type(_buttonDevice, LV_INDEV_TYPE_BUTTON);
    lv_indev_set_read_cb(_buttonDevice, ButtonReadCallback);

    // Set the points for the button input device
    lv_indev_set_button_points(_buttonDevice, _button_points.data());

    // Start with main screen (tile 1,0)
    lv_tileview_set_tile_by_index(_tileView, 1, 0, LV_ANIM_OFF);
}

LvglUI::~LvglUI()
{
    _instance = nullptr;
    if (_tileView)
    {
        lv_obj_del(_tileView);
    }
    if (_inputGroup)
    {
        lv_group_delete(_inputGroup);
    }
}

void LvglUI::InitializeScreens()
{
    // Create tiles for each screen
    // Row 0: Debug(0,0), Main(1,0), Settings(2,0), WhiteBalance(3,0)
    // Row 1: Shutter(1,1) below main screen

    lv_obj_t* debugTile         = lv_tileview_add_tile(_tileView, 0, 0, (lv_dir_t)(LV_DIR_RIGHT));
    lv_obj_t* mainTile          = lv_tileview_add_tile(_tileView, 1, 0, (lv_dir_t)(LV_DIR_LEFT | LV_DIR_RIGHT | LV_DIR_BOTTOM));
    lv_obj_t* settingsTile      = lv_tileview_add_tile(_tileView, 2, 0, (lv_dir_t)(LV_DIR_LEFT));
    lv_obj_t* shutterTile       = lv_tileview_add_tile(_tileView, 1, 1, (lv_dir_t)(LV_DIR_TOP));
    lv_obj_t* whiteBalanceTile  = lv_tileview_add_tile(_tileView, 2, 1, (lv_dir_t)(LV_DIR_TOP));

    _mainScreen         = std::make_unique<MainScreen>(_appContext.get());
    _settingsScreen     = std::make_unique<SettingsMenuScreen>(_appContext.get());
    _whiteBalanceScreen = std::make_unique<WhiteBalanceScreen>(_appContext.get());
    _shutterScreen      = std::make_unique<ShutterScreen>(_appContext.get());
    _debugScreen        = std::make_unique<DebugScreen>(_appContext.get());

    // Initialize screens with their respective tiles
    _mainScreen->Create(mainTile);
    _settingsScreen->Create(settingsTile);
    _whiteBalanceScreen->Create(whiteBalanceTile);
    _shutterScreen->Create(shutterTile);
    _debugScreen->Create(debugTile);
}

void LvglUI::SwitchToScreen(uint8_t col, uint8_t row, AnimationDirection direction)
{
    lv_anim_enable_t anim = (direction == AnimationDirection::NONE) ? LV_ANIM_OFF : LV_ANIM_ON;
    lv_tileview_set_tile_by_index(_tileView, col, row, anim);
}

void LvglUI::EncoderReadCallback(lv_indev_t* drv, lv_indev_data_t* data)
{
    // Provide the current encoder state to LVGL
    data->state = _encoderButtonPressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;

    // Calculate the difference since last read
    static int32_t last_position = 0;
    data->enc_diff               = _encoderPosition - last_position;
    last_position                = _encoderPosition;
}

uint8_t LvglUI::ReadPhysicalButtons()
{
    return 0; // TODO: Implement hardware button reading logic
}

void LvglUI::ButtonReadCallback(lv_indev_t* drv, lv_indev_data_t* data)
{
    if (_pressedBtnId < 0 || static_cast<size_t>(_pressedBtnId) >= _button_points.size())
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    // Simply supply the state from member variables to LVGL
    data->btn_id = _pressedBtnId;
    data->state  = _btnState;
}

// Static Getters
int32_t LvglUI::GetEncoderPosition()
{
    return _encoderPosition;
}

bool LvglUI::IsEncoderButtonPressed()
{
    return _encoderButtonPressed;
}

const std::vector<lv_point_t>& LvglUI::GetButtonPoints()
{
    return _button_points;
}

void LvglUI::UpdateEncoder(int32_t delta)
{
    bool handled = false;
    if (_instance)
    {
        if (auto* screen = _instance->GetActiveScreen())
        {
            handled = screen->HandleKnob(delta);
        }
    }

    if (!handled)
    {
        _encoderPosition += delta;
    }
}

void LvglUI::TriggerButtonEvent(ButtonID id, ButtonState state) const
{
    if (id == ButtonID::BUTTON_NONE)
    {
        return;
    }

    if (id == ButtonID::KNOB)
    {
        _encoderButtonPressed = (state == ButtonState::Pressed);
    }

    _btnState = (state == ButtonState::Pressed) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;

    _pressedBtnId = static_cast<int>(id);

    std::cout << "Button Event: ID=" << (int)id
              << " State=" << (state == ButtonState::Pressed ? "Pressed" : "Released")
              << " MappedIndex=" << _pressedBtnId << std::endl;

    if (auto* screen = GetActiveScreen())
    {
        screen->HandleAction(id, state);
    }
}

BaseScreen* LvglUI::GetActiveScreen() const
{
    lv_obj_t* activeTile = lv_tileview_get_tile_active(_tileView);
    if (!activeTile)
        return nullptr;

    int32_t col = lv_obj_get_x(activeTile) / LV_HOR_RES;
    int32_t row = lv_obj_get_y(activeTile) / LV_VER_RES;

    if (row == 0)
    {
        switch (col)
        {
        case 0: return _debugScreen.get();
        case 1: return _mainScreen.get();
        case 2: return _settingsScreen.get();
        }
    }
    else if (row == 1)
    {
        switch (col)
        {
        case 1: return _shutterScreen.get();
        case 2: return _whiteBalanceScreen.get();
        }
    }

    return nullptr;
}

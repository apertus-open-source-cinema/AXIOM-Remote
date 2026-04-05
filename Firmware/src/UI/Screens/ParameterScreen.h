#pragma once

#include "BaseScreen.h"
#include "Theme.h"
#include "UIIdentifiers.h"
#include <functional>
#include <string>
#include <vector>

class ParameterScreen : public BaseScreen
{
  protected:
    std::string _title;
    std::string _parameterName;
    std::function<void(int)> _onValueChanged;
    std::function<void()> _onBackPressed;

    lv_obj_t* _titleLabel = nullptr;
    lv_obj_t* _backButton = nullptr;
    lv_obj_t* _setButton  = nullptr;

  public:
    ParameterScreen(AppContext* context, const std::string& title, const std::string& parameterName)
        : BaseScreen(context), _title(title), _parameterName(parameterName)
    {
    }

    void SetValueChangedCallback(std::function<void(int)> callback)
    {
        _onValueChanged = callback;
    }
    void SetBackPressedCallback(std::function<void()> callback)
    {
        _onBackPressed = callback;
    }

    virtual void SetCurrentValue(int value) = 0;
    virtual int GetCurrentValue() const     = 0;

    /**
     * @brief Handle hardware buttons that are NOT mapped to screen coordinates (e.g., Record, Photo).
     * Perimeter buttons (1-12) are handled automatically by LVGL via the Button input device.
     * @return true if the event was handled.
     */
    virtual bool HandleParameterAction(ButtonID id, ButtonState state) { return false; }

    void Create(lv_obj_t* parent) override
    {
        BaseScreen::Create(parent);

        lv_obj_t* titleBar = lv_obj_create(parent);
        lv_obj_remove_style_all(titleBar);
        lv_obj_set_size(titleBar, lv_pct(100), 40);
        lv_obj_align(titleBar, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_set_style_bg_color(titleBar, lv_color_hex(0x333333), 0);

        _titleLabel = lv_label_create(titleBar);
        lv_obj_add_style(_titleLabel, &g_style_label_white, 0);
        lv_label_set_text(_titleLabel, _title.c_str());
        lv_obj_center(_titleLabel);

        _backButton = lv_button_create(_bottomContainer);
        lv_obj_add_style(_backButton, &g_style_lightgray_rounded_button, 0);
        lv_obj_set_width(_backButton, 80);
        // Align with hardware Button 4 (Bottom Left) center at X=60
        lv_obj_align(_backButton, LV_ALIGN_LEFT_MID, 20, 0);
        lv_obj_add_style(_backButton, &g_style_lightgray_rounded_button_pressed, LV_STATE_PRESSED);

        lv_obj_t* backIcon = lv_label_create(_backButton);
        lv_label_set_text(backIcon, "Cancel");
        lv_obj_center(backIcon);
        lv_obj_remove_flag(backIcon, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_add_event_cb(
            _backButton,
            [](lv_event_t* e) {
                auto screen = static_cast<ParameterScreen*>(lv_event_get_user_data(e));
                if (screen->_onBackPressed)
                    screen->_onBackPressed();
            },
            LV_EVENT_CLICKED, this);

        _setButton = lv_button_create(_bottomContainer);
        lv_obj_add_style(_setButton, &g_style_lightgray_rounded_button, 0);
        lv_obj_set_width(_setButton, 80);
        // Align with hardware Button 6 (Bottom Right) center at X=240
        lv_obj_align(_setButton, LV_ALIGN_LEFT_MID, 200, 0);
        lv_obj_add_style(_setButton, &g_style_lightgray_rounded_button_pressed, LV_STATE_PRESSED);

        lv_obj_t* setIcon = lv_label_create(_setButton);
        lv_label_set_text(setIcon, "Set");
        lv_obj_center(setIcon);
        lv_obj_remove_flag(setIcon, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_add_event_cb(
            _setButton,
            [](lv_event_t* e) {
                auto screen = static_cast<ParameterScreen*>(lv_event_get_user_data(e));
                if (screen->_onValueChanged)
                    screen->_onValueChanged(screen->GetCurrentValue());
                if (screen->_onBackPressed) 
                    screen->_onBackPressed();
            },
            LV_EVENT_CLICKED, this);

        CreateParameterUI(_centerContainer);
    }

    virtual void CreateParameterUI(lv_obj_t* parent) = 0;
};
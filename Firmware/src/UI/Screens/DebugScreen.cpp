#include "DebugScreen.h"

#include "Theme.h"

#include <iostream>
#include <vector>

#include "../LvglUI.h"

DebugScreen::DebugScreen(AppContext* context) : BaseScreen(context)
{
}

void DebugScreen::Create(lv_obj_t* parent)
{
    // Call base class create
    BaseScreen::Create(parent);

    // Create a visual indicator for each of the 12 buttons
    for (int i = 0; i < 12; ++i)
    {
        lv_obj_t* target = nullptr;
        if (i < 3)      target = _topContainer;    // IDs 0,1,2 -> B1, B2, B3
        else if (i < 6) target = _bottomContainer; // IDs 3,4,5 -> B4, B5, B6
        else if (i < 9) target = _leftContainer;   // IDs 6,7,8 -> B7, B8, B9
        else            target = _rightContainer;  // IDs 9,10,11 -> B10, B11, B12

        // Add a bit of gap between buttons
        if (target) {
            lv_obj_set_style_pad_column(target, 10, 0);
            lv_obj_set_style_pad_row(target, 10, 0);
        }

        lv_obj_t* btn = lv_button_create(target);
        lv_obj_set_size(btn, 50, 40);
        lv_obj_add_style(btn, &g_style_lightgray_rounded_button, 0);
        lv_obj_add_style(btn, &g_style_lightgray_rounded_button_pressed, LV_STATE_PRESSED);

        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text_fmt(label, "B%d", i + 1);
        lv_obj_center(label);

        // Add event handler for button click
        lv_obj_add_event_cb(btn, [](lv_event_t * e) {
            lv_obj_t * btn = (lv_obj_t*)lv_event_get_target(e);
            lv_obj_t * label = lv_obj_get_child(btn, 0);
            const char * txt = lv_label_get_text(label);
            std::cout << "Clicked: " << txt << std::endl;
        }, LV_EVENT_CLICKED, NULL);
    }

    // Create labels for encoder data
    lv_obj_t* encoderPosTitle = lv_label_create(_centerContainer);
    lv_label_set_text(encoderPosTitle, "Encoder Position:");

    _encoderPosLabel = lv_label_create(_centerContainer);
    lv_obj_add_style(_encoderPosLabel, &g_style_value_dark, 0);
    lv_label_set_text(_encoderPosLabel, "0");

    lv_obj_t* encoderBtnTitle = lv_label_create(_centerContainer);
    lv_label_set_text(encoderBtnTitle, "Encoder Button:");
    lv_obj_set_style_margin_top(encoderBtnTitle, 20, 0);

    _encoderBtnLabel = lv_label_create(_centerContainer);
    lv_obj_add_style(_encoderBtnLabel, &g_style_value_dark, 0);
    lv_label_set_text(_encoderBtnLabel, "Released");

    // Create a timer to periodically update the labels
    lv_timer_create(
        [](lv_timer_t* timer) {
            auto* screen = static_cast<DebugScreen*>(lv_timer_get_user_data(timer));
            if (screen) screen->UpdateLabels();
        },
        100, // 100ms
        this);
}

void DebugScreen::UpdateLabels()
{
    lv_label_set_text_fmt(_encoderPosLabel, "%d", LvglUI::GetEncoderPosition());
    lv_label_set_text(_encoderBtnLabel, LvglUI::IsEncoderButtonPressed() ? "Pressed" : "Released");
}

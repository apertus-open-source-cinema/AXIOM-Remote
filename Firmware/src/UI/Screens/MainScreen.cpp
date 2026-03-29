#include "MainScreen.h"

#include <iostream>

#include "ButtonDefinitions.h"
#include "Media/ApertusLogo_full.h"

#include "AppContext.h"
#include "Theme.h"
#include "UIIdentifiers.h"

MainScreen::MainScreen(AppContext* context) : BaseScreen(context)
{
}

void MainScreen::Create(lv_obj_t* parent)
{
    BaseScreen::Create(parent);

    lv_obj_set_layout(parent, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    _fpsBlock = CreateControlBlock(_topContainer, "FPS");
    lv_obj_add_event_cb(_fpsBlock, [](lv_event_t* e) { std::cout << "FPS" << std::endl; }, LV_EVENT_CLICKED, this);

    _aGainBlock = CreateControlBlock(_topContainer, "A. Gain");
    lv_obj_add_event_cb(_aGainBlock, [](lv_event_t* e) { std::cout << "A. Gain" << std::endl; }, LV_EVENT_CLICKED, this);

    _dGainBlock = CreateControlBlock(_topContainer, "D. Gain");
    lv_obj_add_event_cb(_dGainBlock, [](lv_event_t* e) { std::cout << "D. Gain" << std::endl; }, LV_EVENT_CLICKED, this);

    lv_obj_t* apertusLogo = lv_img_create(_centerContainer);
    lv_img_set_src(apertusLogo, &ApertusLogo_full);
    lv_obj_align(apertusLogo, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(apertusLogo, ApertusLogo_full.header.w, ApertusLogo_full.header.h);

    _menuButton = lv_button_create(_bottomContainer);
    lv_obj_add_style(_menuButton, &g_style_menu_button_black, 0);
    lv_obj_set_size(_menuButton, 90, 40);

    lv_obj_add_event_cb(
        _menuButton,
        [](lv_event_t* e) {
            auto* screen = static_cast<MainScreen*>(lv_event_get_user_data(e));
            screen->GetContext()->ProcessAction(UIAction::OPEN_SETTINGS_MENU);
        },
        LV_EVENT_CLICKED, this);
    lv_obj_add_style(_menuButton, &g_style_menu_button_black_pressed, LV_STATE_PRESSED);

    lv_obj_t* menuLabel = lv_label_create(_menuButton);
    lv_label_set_text(menuLabel, "MENU");
    lv_obj_center(menuLabel);

    _debugButton = lv_button_create(parent);
    lv_obj_add_flag(_debugButton, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(_debugButton, &g_style_menu_button_black, 0);
    lv_obj_set_size(_debugButton, 80, 25);
    lv_obj_align(_debugButton, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_style(_debugButton, &g_style_menu_button_black_pressed, LV_STATE_PRESSED);
    lv_obj_add_event_cb(
        _debugButton,
        [](lv_event_t* e) {
            auto* screen = static_cast<MainScreen*>(lv_event_get_user_data(e));
            screen->GetContext()->ProcessActionWithAnimation(UIAction::OPEN_DEBUG_SCREEN, AnimationDirection::UP);
        },
        LV_EVENT_CLICKED, this);

    lv_obj_t* debugLabel = lv_label_create(_debugButton);
    lv_label_set_text(debugLabel, "DEBUG");
    lv_obj_center(debugLabel);

    _shutterBlock = CreateInvertedControlBlock(_bottomContainer, "Shutter");
    lv_obj_add_event_cb(
        _shutterBlock,
        [](lv_event_t* e) {
            auto* screen = static_cast<MainScreen*>(lv_event_get_user_data(e));
            screen->GetContext()->ProcessActionWithAnimation(UIAction::OPEN_SHUTTER_SETTINGS, AnimationDirection::UP);
        },
        LV_EVENT_CLICKED, this);

    _wbBlock = CreateInvertedControlBlock(_bottomContainer, "WB");
    lv_obj_add_event_cb(
        _wbBlock,
        [](lv_event_t* e) {
            auto* screen = static_cast<MainScreen*>(lv_event_get_user_data(e));
            screen->GetContext()->ProcessActionWithAnimation(UIAction::OPEN_WHITE_BALANCE_SETTINGS, AnimationDirection::UP);
        },
        LV_EVENT_CLICKED, this);
    UpdateDisplay();
}

bool MainScreen::HandleAction(ButtonID id, ButtonState state)
{
    // Add handling for other buttons here if needed, beyond the 12 around the display
    return false;
}

void MainScreen::OnEnter()
{
    UpdateDisplay();
}

void MainScreen::OnExit()
{
}

void MainScreen::UpdateDisplay()
{
}

static void apply_pressed_recursive(lv_obj_t* obj, bool pressed)
{
    if (pressed)
        lv_obj_add_state(obj, LV_STATE_PRESSED);
    else
        lv_obj_clear_state(obj, LV_STATE_PRESSED);

    uint32_t count = lv_obj_get_child_cnt(obj);
    for (uint32_t i = 0; i < count; i++)
    {
        apply_pressed_recursive(lv_obj_get_child(obj, i), pressed);
    }
}

static lv_obj_t* create_box(lv_obj_t* parent, const lv_style_t* style_default, const lv_style_t* style_pressed, lv_coord_t height, const char* text,
                            const lv_style_t* label_style)
{
    lv_obj_t* box = lv_obj_create(parent);
    lv_obj_remove_flag(box, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(box, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_add_style(box, style_default, 0);
    lv_obj_add_style(box, style_pressed, LV_STATE_PRESSED);

    lv_obj_set_size(box, lv_pct(90), height);

    lv_obj_t* label = lv_label_create(box);
    lv_obj_add_style(label, label_style, 0);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    return box;
}

lv_obj_t* MainScreen::CreateControlBlock(lv_obj_t* parent, const char* label_text, const char* value_text)
{
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);
    lv_obj_set_size(container, 90, 70);
    lv_obj_remove_flag(container, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* value_bg = lv_obj_create(container);
    lv_obj_remove_flag(value_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(value_bg, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(value_bg, &g_style_lightgray_rounded_box, 0);
    lv_obj_set_size(value_bg, lv_pct(90), 45);
    lv_obj_align(value_bg, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t* value = lv_label_create(value_bg);
    lv_obj_add_style(value, &g_style_value_dark, 0);
    lv_label_set_text(value, value_text);
    lv_obj_center(value);

    lv_obj_t* button = lv_button_create(container);
    lv_obj_remove_style_all(button);
    lv_obj_add_style(button, &g_style_black_sharp_box, 0);
    lv_obj_add_style(button, &g_style_menu_button_black_pressed, LV_STATE_PRESSED);
    lv_obj_set_size(button, lv_pct(90), 25);
    lv_obj_align(button, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t* label = lv_label_create(button);
    lv_obj_add_style(label, &g_style_label_white, 0);
    lv_label_set_text(label, label_text);
    lv_obj_center(label);

    return button;
}

lv_obj_t* MainScreen::CreateInvertedControlBlock(lv_obj_t* parent, const char* label_text, const char* value_text)
{
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);
    lv_obj_set_size(container, 90, 70);
    lv_obj_remove_flag(container, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* value_bg = lv_obj_create(container);
    lv_obj_remove_flag(value_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(value_bg, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(value_bg, &g_style_lightgray_rounded_box, 0);
    lv_obj_set_size(value_bg, lv_pct(90), 45);
    lv_obj_align(value_bg, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t* value = lv_label_create(value_bg);
    lv_obj_add_style(value, &g_style_value_dark, 0);
    lv_label_set_text(value, value_text);
    lv_obj_center(value);

    lv_obj_t* button = lv_button_create(container);
    lv_obj_remove_style_all(button);
    lv_obj_add_style(button, &g_style_black_sharp_box, 0);
    lv_obj_add_style(button, &g_style_menu_button_black_pressed, LV_STATE_PRESSED);
    lv_obj_set_size(button, lv_pct(90), 25);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t* label = lv_label_create(button);
    lv_obj_add_style(label, &g_style_label_white, 0);
    lv_label_set_text(label, label_text);
    lv_obj_center(label);

    return button;
}
#include "SettingsMenuScreen.h"
#include "ButtonDefinitions.h"
#include <iostream>

#include "AppContext.h"
#include "UIIdentifiers.h"

SettingsMenuScreen::SettingsMenuScreen(AppContext* context) : BaseScreen(context)
{
}

void SettingsMenuScreen::Create(lv_obj_t* parent)
{
    BaseScreen::Create(parent);

    lv_obj_t* menu = lv_menu_create(parent);
    lv_obj_set_size(menu, lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL));
    
    lv_obj_t* back_btn          = lv_menu_get_main_header_back_button(menu);
    lv_obj_t* back_button_label = lv_label_create(back_btn);
    lv_label_set_text(back_button_label, "Back");

    lv_obj_t* main_page = lv_menu_page_create(menu, "Settings");

    lv_group_t* group = lv_group_create();
    
    lv_obj_t* cont    = lv_menu_cont_create(main_page);
    lv_obj_t* label   = lv_label_create(cont);
    lv_label_set_text(label, "General");
    lv_group_add_obj(group, cont);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_CLICKABLE);

    cont  = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Output");
    lv_group_add_obj(group, cont);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_CLICKABLE);

    lv_menu_set_page(menu, main_page);
}

bool SettingsMenuScreen::HandleAction(ButtonID id, ButtonState state)
{
    bool handled = true;
    switch (id)
    {
    case ButtonID::BUTTON_7: // Back button (left side)
        if (_appContext && state == ButtonState::Released)
            _appContext->ProcessAction(UIAction::GO_TO_PREVIOUS_SCREEN);
        break;
    default:
        handled = false;
        break;
    }

    return handled;
}

void SettingsMenuScreen::OnEnter()
{
    UpdateDisplay();
}

void SettingsMenuScreen::OnExit()
{
}

void SettingsMenuScreen::UpdateDisplay()
{
}

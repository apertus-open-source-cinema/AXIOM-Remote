#include "SettingsMenuScreen.h"
#include "ButtonDefinitions.h"
#include "Media/ApertusLogo_full.h"
#include <iostream>

#include "AppContext.h"
#include "UIIdentifiers.h"

static void my_event_cb(lv_event_t* event)
{
    std::cout << "Clicked" << std::endl;
}

// #include "AppContext.hpp" // Include full definition

// Include image headers needed
// extern const lv_image_dsc_t apertus_logo;

// Define styles needed by CreateControlBlock etc. if not global
// static lv_style_t style_black_sharp_box; ...

SettingsMenuScreen::SettingsMenuScreen(AppContext* context) : BaseScreen(context)
{
}

void SettingsMenuScreen::Create(lv_obj_t* parent)
{
    BaseScreen::Create(parent); // Call base class to store parent

    lv_obj_t* menu = lv_menu_create(parent);
    lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_center(menu);

    lv_obj_t* cont;
    lv_obj_t* label;

    /*Create a main page*/
    lv_obj_t* main_page = lv_menu_page_create(menu, NULL);

    cont = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 1");

    lv_menu_separator_create(main_page);

    cont = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 2");

    lv_obj_t* separator = lv_menu_separator_create(main_page);
    lv_obj_set_style_bg_color(separator, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_height(separator, 2, 0); // Set a visible height
    // lv_obj_set_height(separator, 2);  // Force it to have some height
    // lv_obj_set_style_bg_color(separator, lv_color_black(), 0);
    // lv_obj_set_style_bg_opa(separator, LV_OPA_COVER, 0);
    // static lv_style_t separator_style;
    // lv_style_init(&separator_style);
    // lv_style_set_border_width(&separator_style, 1);
    // lv_style_set_border_color(&separator_style, lv_palette_main(LV_PALETTE_GREY));
    // lv_style_set_pad_top(&separator_style, 10);
    // lv_style_set_pad_bottom(&separator_style, 10);

    // separator = lv_menu_separator_create(main_page);
    // lv_obj_add_style(separator, &separator_style, 0);

    cont = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 3");

    lv_menu_set_page(menu, main_page);

    UpdateDisplay();
}

bool SettingsMenuScreen::HandleAction(ButtonID id, ButtonState state)
{
    lv_obj_t* targetObject = nullptr;

    bool handled = true; // Assume handled unless proven otherwise
    switch (id)
    {
    case ButtonID::BUTTON_4:
        // targetObject = _menuButton;
        if (_appContext)
            // _appContext->ProcessAction(UIAction::OPEN_SETTINGS_MENU);
            // _appContext->RequestScreenChange(ScreenID::SETTINGS_MENU, true);
            // Example: m_appContext->CycleFPS();
            break;
    default:
        handled = false;
        break;
        //     case UIAction::Identifier::ACTION_1_FPS:
        //         // Example: m_appContext->CycleFPS();
        //         break;
        //     case UIAction::Identifier::ACTION_2_A_GAIN:
        //         // Example: m_appContext->NextAGain();
        //         break;
        //     case UIAction::Identifier::ACTION_3_D_GAIN:
        //          // Example: m_appContext->NextDGain();
        //         break;
        //     case UIAction::Identifier::ACTION_4_MENU:
        //         // Example: Request screen change via AppContext or ScreenManager
        //         SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "MENU Action - Requesting Screen Change (TODO)");
        //         // m_appContext->RequestScreenChange(ScreenID::SETTINGS_LIST);
        //         break;
        //     case UIAction::Identifier::ACTION_5_SHUTTER:
        //         // Example: Request screen change or cycle shutter setting
        //         // m_appContext->RequestScreenChange(ScreenID::SHUTTER_SETTINGS); // Or cycle directly
        //         break;
        //     case UIAction::Identifier::ACTION_6_WB:
        //         // Example: Request screen change or cycle WB setting
        //         // m_appContext->RequestScreenChange(ScreenID::WB_SETTINGS); // Or cycle directly
        //         break;
        //     default:
        //         handled = false; // Action not relevant to this screen
        //         break;
        // }
        // If an action modified state that needs immediate display update:
        // if(handled && m_appContext) UpdateDisplay(); // Update based on new context state
    }

    if (targetObject)
    {
        lv_event_code_t eventCode = state == ButtonState::Pressed ? LV_EVENT_PRESSED : LV_EVENT_RELEASED;
        lv_obj_send_event(targetObject, eventCode, nullptr);
    }

    return handled;
}

void SettingsMenuScreen::OnEnter()
{
    // Update display when screen becomes active
    UpdateDisplay();
}

void SettingsMenuScreen::OnExit()
{
    // Clean up anything specific to this screen if needed
}

void SettingsMenuScreen::UpdateDisplay()
{
    // Example: Read state from m_appContext and update labels
    // if(m_appContext && m_fpsValueLabel) {
    //    lv_label_set_text_fmt(m_fpsValueLabel, "%d", m_appContext->GetFpsValue());
    // }
    // ... update other labels ...
}

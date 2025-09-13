#include "MainScreen.h"
#include "ButtonDefinitions.h"
#include "Media/ApertusLogo_full.h"
#include <iostream>

#include "AppContext.h"
#include "UIIdentifiers.h"
// #include "drivers/ili9341/ili_def.h"

static void my_event_cb(lv_event_t* event)
{
    printf("Clicked\n");
}

static void anim_x_cb(void * var, int32_t v)
{
    lv_obj_set_x((lv_obj_t*)var, v);
}

static void anim_size_cb(void * var, int32_t v)
{
    lv_obj_set_size((lv_obj_t*)var, v, v);
}

// #include "AppContext.hpp" // Include full definition

// Include image headers needed
// extern const lv_image_dsc_t apertus_logo;

// Define styles needed by CreateControlBlock etc. if not global
// static lv_style_t style_black_sharp_box; ...

MainScreen::MainScreen(AppContext* context) : BaseScreen(context)
{
}

void MainScreen::Create(lv_obj_t* parent)
{
    BaseScreen::Create(parent); // Call base class to store parent

    // lv_obj_set_style_bg_color(parent, lv_color_hex(0x808080), 0); // Medium gray background
    // lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, 0);

    lv_obj_set_layout(parent, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* topRowContainer = lv_obj_create(parent);
    lv_obj_remove_style_all(topRowContainer);
    lv_obj_set_width(topRowContainer, lv_pct(100));
    lv_obj_set_height(topRowContainer, LV_SIZE_CONTENT);
    lv_obj_set_layout(topRowContainer, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(topRowContainer, LV_FLEX_FLOW_ROW);
    // Distribute blocks evenly within the row container
    lv_obj_set_flex_align(topRowContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    // Add padding between the blocks if SPACE_EVENLY doesn't give enough
    // lv_obj_set_style_pad_column(topRowContainer, 5, 0);

    lv_obj_t* fpsBlock = CreateControlBlock(topRowContainer, "FPS");
    lv_obj_t* aGainBlock = CreateControlBlock(topRowContainer, "A. Gain");
    lv_obj_t* dGainBlock = CreateControlBlock(topRowContainer, "D. Gain");

    lv_obj_t* apertusLogo = lv_img_create(parent);
    lv_img_set_src(apertusLogo, &ApertusLogo_full);
    // lv_obj_align(apertusLogo, LV_ALIGN_TOP_LEFT, 58, 59);
    lv_obj_align(apertusLogo, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(apertusLogo, ApertusLogo_full.header.w, ApertusLogo_full.header.h);

    // --- Bottom Row ---
    lv_obj_t* bottomRowContainer = lv_obj_create(parent);
    lv_obj_remove_style_all(bottomRowContainer);
    lv_obj_set_width(bottomRowContainer, lv_pct(100));
    lv_obj_set_height(bottomRowContainer, LV_SIZE_CONTENT);
    lv_obj_set_layout(bottomRowContainer, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(bottomRowContainer, LV_FLEX_FLOW_ROW);
    // lv_obj_set_flex_align(bottomRowContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER,
    // LV_FLEX_ALIGN_CENTER);
    lv_obj_set_flex_align(bottomRowContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER);
    // lv_obj_set_style_pad_column(bottomRowContainer, 5, 0); // Optional padding

    // lv_obj_t* menuBlock = create_control_block(bottomRowContainer, "MENU");
    // Create the larger MENU button separately (as a direct child of the row container)
    _menuButton = lv_button_create(bottomRowContainer);
    lv_obj_add_style(_menuButton, &g_style_menu_button_black, 0);
    lv_obj_set_size(_menuButton, 90, 40); // Example larger size (same height as blocks)
    lv_obj_align_to(_menuButton, bottomRowContainer, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_add_event_cb(_menuButton, my_event_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_style(_menuButton, &g_style_menu_button_black_pressed, LV_STATE_PRESSED);

    lv_obj_t* menuLabel = lv_label_create(_menuButton);
    // lv_obj_add_style(menuLabel, &g_style_label_white, 0); // Use white label style
    lv_label_set_text(menuLabel, "MENU");
    lv_obj_center(menuLabel);

    // lv_obj_add_event_cb(menuButton, MenuButtonCallback, LV_EVENT_CLICKED, nullptr);

    lv_obj_t* shutterBlock = CreateInvertedControlBlock(bottomRowContainer, "Shutter");
    lv_obj_t* wbBlock = CreateInvertedControlBlock(bottomRowContainer, "WB");

    // lv_obj_t * obj = lv_obj_create(lv_scr_act());
    // lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), 0);
    // lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

    // lv_obj_align(obj, LV_ALIGN_LEFT_MID, 10, 0);

    // lv_anim_t a;
    // lv_anim_init(&a);
    // lv_anim_set_var(&a, obj);
    // lv_anim_set_values(&a, 10, 50);
    // lv_anim_set_time(&a, 1000);
    // lv_anim_set_playback_delay(&a, 100);
    // lv_anim_set_playback_time(&a, 300);
    // lv_anim_set_repeat_delay(&a, 500);
    // lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    // lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

    // lv_anim_set_exec_cb(&a, anim_size_cb);
    // lv_anim_start(&a);
    // lv_anim_set_exec_cb(&a, anim_x_cb);
    // lv_anim_set_values(&a, 10, 240);
    // lv_anim_start(&a);

    printf("Setting up tearing test animation...\n");

    // Get screen dimensions from your platformDriver


    // Optional: Create a solid black background to ensure max contrast
    // If your default theme is already dark, this might not be strictly necessary
    // but it guarantees the contrast for the test.
    // lv_obj_t* bg_for_tear_test = lv_obj_create(lv_screen_active());
    // lv_obj_remove_style_all(bg_for_tear_test); // Remove default styles
    // lv_obj_set_size(bg_for_tear_test, screen_width, screen_height);
    // lv_obj_set_style_bg_color(bg_for_tear_test, lv_color_black(), 0);
    // lv_obj_set_style_bg_opa(bg_for_tear_test, LV_OPA_COVER, 0);
    // lv_obj_align(bg_for_tear_test, LV_ALIGN_TOP_LEFT, 0, 0);
    // Ensure it's behind the bar if you add it to lv_screen_active()
    // lv_obj_send_to_back(bg_for_tear_test); // If tear_bar is also child of screen_active

    // Create the moving bar
    // Parent to lv_screen_active() to avoid flex layout conflicts from _screenRoot
    lv_obj_t* tear_bar = lv_obj_create(lv_screen_active()); 
    lv_obj_remove_style_all(tear_bar); // Remove default styles for a clean bar
    lv_obj_set_size(tear_bar, 30, 320); // 30px wide, full height
    lv_obj_set_style_bg_color(tear_bar, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(tear_bar, LV_OPA_COVER, 0);
    lv_obj_align(tear_bar, LV_ALIGN_LEFT_MID, 0, 0); // Start at left edge

    // Animation for the bar
    // Needs to be static if 'tear_anim' is a local variable in main,
    // so its lifetime extends beyond the setup block.
    static lv_anim_t tear_anim_obj;
    lv_anim_init(&tear_anim_obj);
    lv_anim_set_var(&tear_anim_obj, tear_bar);

    // Define the animation execution callback (moves the object horizontally)
    lv_anim_set_exec_cb(&tear_anim_obj, [](void* var, int32_t v) {
        lv_obj_set_x(static_cast<lv_obj_t*>(var), v);
    });

    lv_anim_set_values(&tear_anim_obj, 0, 240 - 30); // Animate x from 0 to (screen_width - bar_width)
    lv_anim_set_time(&tear_anim_obj, 750);                   // Time for one sweep (e.g., 750 milliseconds)
    lv_anim_set_playback_time(&tear_anim_obj, 750);          // Time for the sweep back
    lv_anim_set_repeat_count(&tear_anim_obj, LV_ANIM_REPEAT_INFINITE); // Loop indefinitely
    lv_anim_set_path_cb(&tear_anim_obj, lv_anim_path_linear); // Ensures constant speed, best for spotting tears

    lv_anim_start(&tear_anim_obj);

    printf("Tearing test animation started.\n");



    // Ensure styles are initialized (if local to this screen, otherwise global)
    // if (!styles_initialized) { InitializeStyles(); styles_initialized = true; }

    // --- Layout for this specific screen ---
    // Example: Use Flexbox like before, but now parent is the TileView Tile

    // lv_obj_set_layout(m_screenRoot, LV_LAYOUT_FLEX);
    // lv_obj_set_flex_flow(m_screenRoot, LV_FLEX_FLOW_COLUMN);
    // lv_obj_set_flex_align(m_screenRoot, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    // lv_obj_set_style_pad_all(m_screenRoot, 5, 0); // Padding within the tile

    // // Create Top Row Container and its blocks (using helper)
    // lv_obj_t* topRowContainer = lv_obj_create(m_screenRoot);
    // // ... configure topRowContainer layout (ROW, SPACE_BETWEEN/EVENLY) ...
    // lv_obj_t* fpsBlock = CreateControlBlock(topRowContainer, "FPS");
    // lv_obj_t* aGainBlock = CreateControlBlock(topRowContainer, "A. Gain");
    // lv_obj_t* dGainBlock = CreateControlBlock(topRowContainer, "D. Gain");
    // // Store value label pointers if needed for UpdateDisplay
    // // m_fpsValueLabel = lv_obj_get_child(lv_obj_get_child(fpsBlock, 1), 0); // Example: Find label nested in
    // value_bg

    // // Create Logo
    // lv_obj_t* logoImage = lv_image_create(m_screenRoot);
    // // lv_image_set_src(logoImage, &apertus_logo);

    // // Create Bottom Row Container and its items (using helpers/button)
    // lv_obj_t* bottomRowContainer = lv_obj_create(m_screenRoot);
    // // ... configure bottomRowContainer layout (ROW, START, padding) ...
    // lv_obj_t* menuButton = lv_button_create(bottomRowContainer);
    // // ... create menuButton content ...
    // lv_obj_t* shutterBlock = CreateInvertedControlBlock(bottomRowContainer, "Shutter");
    // lv_obj_t* wbBlock = CreateInvertedControlBlock(bottomRowContainer, "WB");

    // IMPORTANT: Add ACTUAL LVGL event callbacks if these blocks/buttons
    // need to DO something when physically interacted with (in addition to simulation)
    // Example:
    // lv_obj_add_event_cb(menuButton, [](lv_event_t* e){
    //      MainScreen* screen = static_cast<MainScreen*>(lv_event_get_user_data(e));
    //      if(screen && screen->m_appContext) {
    //          screen->m_appContext->ActionMenu(); // Trigger action via context
    //      }
    // }, LV_EVENT_CLICKED, this);

    // Initial update
    UpdateDisplay();
}

bool MainScreen::HandleAction(ButtonID id, ButtonState state)
{
    lv_obj_t* targetObject = nullptr;

    bool handled = true; // Assume handled unless proven otherwise
    switch (id)
    {
    case ButtonID::BUTTON_4:
        targetObject = _menuButton;
        if (_appContext && state == ButtonState::Released)
            _appContext->ProcessAction(UIAction::OPEN_SETTINGS_MENU);
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

void MainScreen::OnEnter()
{
    // Update display when screen becomes active
    UpdateDisplay();
}

void MainScreen::OnExit()
{
    // Clean up anything specific to this screen if needed
}

void MainScreen::UpdateDisplay()
{
    // Example: Read state from m_appContext and update labels
    // if(m_appContext && m_fpsValueLabel) {
    //    lv_label_set_text_fmt(m_fpsValueLabel, "%d", m_appContext->GetFpsValue());
    // }
    // ... update other labels ...
}

// Implement CreateControlBlock / CreateInvertedControlBlock helpers here
// (copied/adapted from previous LvglUI implementation, ensure styles are accessible)
lv_obj_t* MainScreen::CreateControlBlock(lv_obj_t* parent, const char* label_text, const char* value_text)
{
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);                // Start clean
    lv_obj_set_size(container, 90, 70);                // Adjust size as needed to match screenshot proportions
    lv_obj_add_flag(container, LV_OBJ_FLAG_CLICKABLE); // Make it interactive

    // Black label area (top part)
    lv_obj_t* label_bg = lv_obj_create(container);
    lv_obj_add_style(label_bg, &g_style_black_sharp_box, 0);
    lv_obj_set_size(label_bg, lv_pct(90), 25); // Example height, full width
    lv_obj_align(label_bg, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_remove_flag(label_bg, LV_OBJ_FLAG_SCROLLABLE); // Prevent scrolling

    lv_obj_t* label = lv_label_create(label_bg); // Label inside black area
    lv_obj_add_style(label, &g_style_label_white, 0);
    lv_label_set_text(label, label_text);
    lv_obj_center(label);

    // Gray value area (bottom part)
    lv_obj_t* value_bg = lv_obj_create(container);
    lv_obj_add_style(value_bg, &g_style_lightgray_rounded_box, 0);
    lv_obj_set_size(value_bg, lv_pct(90), 45); // Example height, full width
    lv_obj_align(value_bg, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_remove_flag(value_bg, LV_OBJ_FLAG_SCROLLABLE); // Prevent scrolling

    lv_obj_t* value = lv_label_create(value_bg); // Value text inside gray area
    lv_obj_add_style(value, &g_style_value_dark, 0);
    lv_label_set_text(value, value_text);
    lv_obj_center(value);

    lv_obj_remove_flag(container, LV_OBJ_FLAG_SCROLLABLE); // Prevent scrolling

    return container;
}

lv_obj_t* MainScreen::CreateInvertedControlBlock(lv_obj_t* parent, const char* label_text, const char* value_text)
{
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);
    lv_obj_set_size(container, 90, 70); // Adjust size
    lv_obj_add_flag(container, LV_OBJ_FLAG_CLICKABLE);

    // Gray value area (TOP, rounded)
    lv_obj_t* value_bg = lv_obj_create(container);
    lv_obj_add_style(value_bg, &g_style_lightgray_rounded_box, 0); // Rounded
    lv_obj_set_size(value_bg, lv_pct(90), 45);                     // Top portion
    lv_obj_align(value_bg, LV_ALIGN_TOP_MID, 0, 0);                // Align TOP
    lv_obj_remove_flag(value_bg, LV_OBJ_FLAG_SCROLLABLE);          // Prevent scrolling

    lv_obj_t* value = lv_label_create(value_bg);
    lv_obj_add_style(value, &g_style_value_dark, 0);
    lv_label_set_text(value, value_text);
    lv_obj_center(value);

    // Black label area (BOTTOM, sharp)
    lv_obj_t* label_bg = lv_obj_create(container);
    lv_obj_add_style(label_bg, &g_style_black_sharp_box, 0); // Sharp
    lv_obj_set_size(label_bg, lv_pct(90), 25);               // Bottom portion
    lv_obj_align(label_bg, LV_ALIGN_BOTTOM_MID, 0, 0);       // Align BOTTOM
    lv_obj_remove_flag(label_bg, LV_OBJ_FLAG_SCROLLABLE);    // Prevent scrolling

    lv_obj_t* label = lv_label_create(label_bg);
    lv_obj_add_style(label, &g_style_label_white, 0);
    lv_label_set_text(label, label_text);
    lv_obj_center(label);

    return container;
}

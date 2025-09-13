#include "LvglUI.h"

#include <iostream>
#include <memory>
#include <src/misc/lv_event.h>
#include <src/misc/lv_types.h>

#include "AppContext.h"
#include "ButtonDefinitions.h"
#include "Media/ApertusLogo_full.h"
#include "Screens/MainScreen.h"
#include "Screens/SettingsMenuScreen.h"
#include "Theme.h" // Include Theme.h to access global styles

// #include <functional>
// #include <lvgl.h>

// static lv_draw_buf_t drawBuffer;

// UI::UI(uint16_t width, uint16_t height)
// {
//     // lv_draw_buf_init(&drawBuffer, width, height, LV_COLOR_FORMAT_RGB888 /*LV_COLOR_FORMAT_RGB565*/, 0,
//     frameBuffer,
//     //                  sizeof(frameBuffer));

//     // lv_display_t* disp = lv_display_create(width, height);
// }

LvglUI::LvglUI()
{
    // ...
    lv_obj_t* currentScreen = lv_scr_act(); // Renamed local var
    // if (currentScreen)
    // {
    //     // CreateWidgets(currentScreen); // Pass renamed local var
    //     // MainPage(currentScreen);
    //      _mainScreen.Create(currentScreen);
    // }
    // // ...
    _appContext = std::make_unique<AppContext>();
    _appContext->SetScreenChangeRequestHandler(
        // Lambda captures the screenManager pointer
        [&](ScreenID id, bool anim) {
            _screenManager->SwitchToScreen(id, anim);
        }
    );
    
    _screenManager = std::make_unique<ScreenManager>(_appContext.get());
    InitializeScreens();
    _screenManager->Initialize(currentScreen, ScreenID::MAIN); // Start on MAIN screen
}

LvglUI::~LvglUI()
{ /* ... */
}

void LvglUI::InitializeScreens()
{
    // Create screen instances and add them to the manager with their ID and tile position
    // Example: Horizontal layout (row 0, columns 0, 1, 2...)
    _screenManager->AddScreen(ScreenID::MAIN, std::make_unique<MainScreen>(_appContext.get()), 0, 0);
    _screenManager->AddScreen(ScreenID::SETTINGS_MENU, std::make_unique<SettingsMenuScreen>(_appContext.get()), 0, 1);

    // _screenManager->AddScreen(ScreenID::SETTINGS_LIST, std::make_unique<SettingsScreen>(m_appContext), 0, 1); //
    // Example m_screenManager->AddScreen(ScreenID::FPS_SETTINGS, std::make_unique<FpsScreen>(m_appContext), 0, 2); //
    // Example Add more screens...
}

void LvglUI::CreateWidgets(lv_obj_t* parentScreen) // Renamed param
{
    lv_obj_t* button = lv_btn_create(parentScreen); // Renamed local var
    lv_obj_set_pos(button, 50, 50);
    lv_obj_set_size(button, 130, 50);
    lv_obj_add_event_cb(button, CounterButtonEventCallbackStatic, LV_EVENT_ALL, this);

    m_counterButtonLabel = lv_label_create(button);
    lv_label_set_text(m_counterButtonLabel, "Click Count: 0");
    lv_obj_center(m_counterButtonLabel);

    m_statusLabel = lv_label_create(parentScreen);
    lv_label_set_text(m_statusLabel, "LVGL (No Hungarian)");
    lv_obj_align(m_statusLabel, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t* apertusLogo = lv_img_create(parentScreen);
    lv_img_set_src(apertusLogo, &ApertusLogo_full);
    // lv_obj_align(apertusLogo, LV_ALIGN_TOP_LEFT, 58, 59);
    lv_obj_align(apertusLogo, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(apertusLogo, ApertusLogo_full.header.w, ApertusLogo_full.header.h);

    static lv_style_t style_sharp_corners;
    lv_style_init(&style_sharp_corners);
    lv_style_set_radius(&style_sharp_corners, 0);

    lv_obj_t* buttonRowContainer = lv_obj_create(parentScreen);
    lv_obj_remove_style_all(buttonRowContainer);            // Make it invisible, no padding/border/bg affecting layout
    lv_obj_set_width(buttonRowContainer, lv_pct(100));      // Span the full width of the parent
    lv_obj_set_height(buttonRowContainer, LV_SIZE_CONTENT); // Height will adjust based on tallest button
    lv_obj_align(buttonRowContainer, LV_ALIGN_TOP_MID, 0, 0); // Align container to Top-Center, 10px margin from top

    // 3. Set the container's layout to Flexbox
    lv_obj_set_layout(buttonRowContainer, LV_LAYOUT_FLEX);

    // 4. Configure Flexbox flow and alignment for equal spacing
    lv_obj_set_flex_flow(buttonRowContainer, LV_FLEX_FLOW_ROW); // Arrange children in a row

    // Set alignment:
    // - Main Axis (Row): LV_FLEX_ALIGN_SPACE_EVENLY -> Equal space between items AND before first/after last item.
    // - Cross Axis (Column): LV_FLEX_ALIGN_CENTER -> Vertically center buttons in the row.
    // - Track Cross Axis: LV_FLEX_ALIGN_CENTER -> Center items if wrapping occurs (less relevant here).
    lv_obj_set_flex_align(buttonRowContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Alternative Spacing Options for main axis:
    // lv_obj_set_flex_align(buttonRowContainer, LV_FLEX_ALIGN_SPACE_BETWEEN, ...); // Space only BETWEEN items
    // lv_obj_set_flex_align(buttonRowContainer, LV_FLEX_ALIGN_SPACE_AROUND, ...); // Equal space AROUND each item (half
    // space at ends) lv_obj_set_flex_align(buttonRowContainer, LV_FLEX_ALIGN_CENTER, ...); // Buttons clustered in
    // center lv_obj_set_flex_align(buttonRowContainer, LV_FLEX_ALIGN_START, ...); // Buttons clustered at start (left)

    // 5. Create the buttons as CHILDREN of the container
    // Button 1
    lv_obj_t* button1 = lv_button_create(buttonRowContainer); // Parent is the container
    lv_obj_t* label1 = lv_label_create(button1);
    lv_label_set_text(label1, "Button 1");
    lv_obj_center(label1); // Center label on button
    // Optional: Set specific size for button if needed
    // lv_obj_set_size(button1, 100, 40);
    // lv_obj_add_style(button1, &style_sharp_corners, 0);

    // Button 2
    lv_obj_t* button2 = lv_button_create(buttonRowContainer); // Parent is the container
    lv_obj_t* label2 = lv_label_create(button2);
    lv_label_set_text(label2, "Button 2");
    lv_obj_center(label2);
    // lv_obj_set_size(button2, 100, 40);

    // Button 3
    lv_obj_t* button3 = lv_button_create(buttonRowContainer); // Parent is the container
    lv_obj_t* label3 = lv_label_create(button3);
    lv_label_set_text(label3, "Button 3");
    lv_obj_center(label3);
    // lv_obj_set_size(button3, 100, 40);

    // Flexbox will now automatically position button1, button2, and button3
    // horizontally within buttonRowContainer with equal spacing based on the
    // LV_FLEX_ALIGN_SPACE_EVENLY setting.
}

void LvglUI::TriggerButtonEvent(ButtonID id, ButtonState state)
{
    // _mainScreen->HandleAction(id, state);
    _screenManager->RouteAction(id, state);
    // lv_obj_t* targetObject = nullptr;
    // const char* blockName = "Unknown"; // For logging

    // switch (id)
    // {
    // case ButtonID::BUTTON_4:
    //     targetObject = _menuButton;
    //     std::cout << "LVGL Button 1" << std::endl;
    //     break;
    // case ButtonID::BUTTON_2:
    //     std::cout << "LVGL Button 2" << std::endl;
    //     break;
    // case ButtonID::BUTTON_3:
    //     std::cout << "LVGL Button 3" << std::endl;
    //     break;
    // }

    // if (targetObject)
    // {
    //     // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Sending CLICK event directly to: %s", blockName);
    //     // Send the CLICKED event programmatically to the target object
    //     lv_event_code_t eventCode = state == ButtonState::Pressed ? LV_EVENT_PRESSED : LV_EVENT_RELEASED;

    //     lv_obj_send_event(targetObject, eventCode, nullptr);
    // }
    // else
    // {
    //     // SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "TriggerBlockClick: Target object not found for ID %d",
    //     // static_cast<int>(id));
    // }
}

static void my_event_cb(lv_event_t* event)
{
    printf("Clicked\n");
}

void LvglUI::MainPage(lv_obj_t* parentScreen)
{
    // Ensure global theme is initialized (usually done once at startup)
    // InitializeTheme(); // This should be called once, e.g., after lv_init()

    // NOTE: The styles style_black_sharp_box, style_lightgray_rounded_box,
    // style_label_white, style_value_dark, and style_menu_button_black_sharp
    // were defined locally here. They will now be replaced by the global styles
    // g_style_black_sharp_box, g_style_lightgray_rounded_box, g_style_label_white,
    // g_style_value_dark, and g_style_menu_button_black from Theme.cpp.
    // Make sure InitializeTheme() has been called elsewhere in your application's setup.


    // --- Main Screen Setup ---
    // Assuming parentScreen is lv_scr_act() or a fullscreen container
    lv_obj_set_style_bg_color(parentScreen, lv_color_hex(0x808080), 0); // Medium gray background
    lv_obj_set_style_bg_opa(parentScreen, LV_OPA_COVER, 0);

    // Set main screen layout to arrange rows vertically
    lv_obj_set_layout(parentScreen, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(parentScreen, LV_FLEX_FLOW_COLUMN);
    // Align content vertically (e.g., space between pushes top/bottom rows to edges)
    // Align content horizontally (e.g., center items like the logo)
    lv_obj_set_flex_align(parentScreen, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    // Add padding to the screen itself if needed
    // lv_obj_set_style_pad_all(parentScreen, 5, 0);

    // --- Helper Function to Create a Control Block ---
    // (Reduces code duplication)
    auto create_control_block = [&](lv_obj_t* parent, const char* label_text,
                                    const char* value_text = "...") -> lv_obj_t* {
        // Base container for the block
        lv_obj_t* container = lv_obj_create(parent);
        lv_obj_remove_style_all(container);                // Start clean
        lv_obj_set_size(container, 90, 70);                // Adjust size as needed to match screenshot proportions
        lv_obj_add_flag(container, LV_OBJ_FLAG_CLICKABLE); // Make it interactive

        // Black label area (top part)
        lv_obj_t* label_bg = lv_obj_create(container);
        lv_obj_add_style(label_bg, &g_style_black_sharp_box, 0); // Use global style
        lv_obj_set_size(label_bg, lv_pct(100), 25); // Use 100% width of container
        lv_obj_align(label_bg, LV_ALIGN_TOP_MID, 0, 0);

        lv_obj_t* label = lv_label_create(label_bg); // Label inside black area
        lv_obj_add_style(label, &g_style_label_white, 0); // Use global style
        lv_label_set_text(label, label_text);
        lv_obj_set_width(label, lv_pct(100)); // Label widget takes full width of label_bg
        // lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP); // Clip if text too long
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0); // Correct way to set text align
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); // Center label widget in its parent (label_bg)

        // Gray value area (bottom part)
        lv_obj_t* value_bg = lv_obj_create(container);
        lv_obj_add_style(value_bg, &g_style_lightgray_rounded_box, 0); // Use global style
        lv_obj_set_size(value_bg, lv_pct(100), 45); // Use 100% width of container
        lv_obj_align(value_bg, LV_ALIGN_BOTTOM_MID, 0, 0);

        lv_obj_t* value = lv_label_create(value_bg); // Value text inside gray area
        lv_obj_add_style(value, &g_style_value_dark, 0); // Use global style
        lv_label_set_text(value, value_text);
        lv_obj_set_width(value, lv_pct(100)); // Value widget takes full width of value_bg
        // lv_label_set_long_mode(value, LV_LABEL_LONG_CLIP); // Clip if text too long
        lv_obj_set_style_text_align(value, LV_TEXT_ALIGN_CENTER, 0); // Correct way to set text align
        lv_obj_align(value, LV_ALIGN_CENTER, 0, 0); // Center value widget in its parent (value_bg)

        // Optional: Add event callback to the container
        // lv_obj_add_event_cb(container, ControlBlockCallback, LV_EVENT_CLICKED, (void*)label_text); // Example
        // callback

        return container; // Return the main container for this block
    };

    auto create_inverted_control_block = [&](lv_obj_t* parent, const char* label_text,
                                             const char* value_text = "...") -> lv_obj_t* {
        // Base container
        lv_obj_t* container = lv_obj_create(parent);
        lv_obj_remove_style_all(container);
        lv_obj_set_size(container, 90, 70); // Adjust size
        lv_obj_add_flag(container, LV_OBJ_FLAG_CLICKABLE);

        // Gray value area (TOP, rounded)
        lv_obj_t* value_bg = lv_obj_create(container);
        lv_obj_add_style(value_bg, &g_style_lightgray_rounded_box, 0); // Use global style
        lv_obj_set_size(value_bg, lv_pct(100), 45);                   // Top portion, 100% width
        lv_obj_align(value_bg, LV_ALIGN_TOP_MID, 0, 0);              // Align TOP

        lv_obj_t* value = lv_label_create(value_bg);
        lv_obj_add_style(value, &g_style_value_dark, 0); // Use global style
        lv_label_set_text(value, value_text);
        lv_obj_set_width(value, lv_pct(100));
        lv_label_set_long_mode(value, LV_LABEL_LONG_CLIP);
        lv_obj_set_style_text_align(value, LV_TEXT_ALIGN_CENTER, 0); // Correct way to set text align
        lv_obj_align(value, LV_ALIGN_CENTER, 0, 0);

        // Black label area (BOTTOM, sharp)
        lv_obj_t* label_bg = lv_obj_create(container);
        lv_obj_add_style(label_bg, &g_style_black_sharp_box, 0); // Use global style
        lv_obj_set_size(label_bg, lv_pct(100), 25);             // Bottom portion, 100% width
        lv_obj_align(label_bg, LV_ALIGN_BOTTOM_MID, 0, 0);     // Align BOTTOM

        lv_obj_t* label = lv_label_create(label_bg);
        lv_obj_add_style(label, &g_style_label_white, 0); // Use global style
        lv_label_set_text(label, label_text);
        lv_obj_set_width(label, lv_pct(100));
        lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0); // Correct way to set text align
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

        return container;
    };

    // --- Top Row ---
    lv_obj_t* topRowContainer = lv_obj_create(parentScreen);
    lv_obj_remove_style_all(topRowContainer);
    lv_obj_set_width(topRowContainer, lv_pct(100));
    lv_obj_set_height(topRowContainer, LV_SIZE_CONTENT);
    lv_obj_set_layout(topRowContainer, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(topRowContainer, LV_FLEX_FLOW_ROW);
    // Distribute blocks evenly within the row container
    lv_obj_set_flex_align(topRowContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    // Add padding between the blocks if SPACE_EVENLY doesn't give enough
    // lv_obj_set_style_pad_column(topRowContainer, 5, 0);

    lv_obj_t* fpsBlock = create_control_block(topRowContainer, "FPS");
    lv_obj_t* aGainBlock = create_control_block(topRowContainer, "A. Gain");
    lv_obj_t* dGainBlock = create_control_block(topRowContainer, "D. Gain");

    // --- Center Logo ---
    // Assumes apertus_logo descriptor is available (declared via header)
    extern const lv_image_dsc_t apertus_logo; // Make sure this is declared

    // lv_obj_t* logoImage = lv_image_create(parentScreen);
    // lv_image_set_src(logoImage, &apertus_logo);
    // Flexbox on parentScreen with SPACE_BETWEEN should place this nicely in the middle vertically.
    // Horizontal alignment set to CENTER on parentScreen flexbox handles horizontal centering.

    lv_obj_t* apertusLogo = lv_img_create(parentScreen);
    lv_img_set_src(apertusLogo, &ApertusLogo_full);
    // lv_obj_align(apertusLogo, LV_ALIGN_TOP_LEFT, 58, 59);
    lv_obj_align(apertusLogo, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(apertusLogo, ApertusLogo_full.header.w, ApertusLogo_full.header.h);

    // lv_obj_t* apertusLogoRing = lv_img_create(parentScreen);
    // lv_img_set_src(apertusLogoRing, &apertus_ring);
    // // lv_obj_align(apertusLogoRing, LV_ALIGN_TOP_LEFT, 58 + apertus_logo.header.w, 59);
    // lv_obj_set_size(apertusLogoRing, apertus_ring.header.w, apertus_ring.header.h); // Set size if needed}
    // lv_obj_align_to(apertusLogoRing, apertusLogo, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);

    // --- Bottom Row ---
    lv_obj_t* bottomRowContainer = lv_obj_create(parentScreen);
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
    lv_obj_add_style(_menuButton, &g_style_menu_button_black, LV_STATE_DEFAULT); // Use global style
    lv_obj_add_style(_menuButton, &g_style_menu_button_black_pressed, LV_STATE_PRESSED); // Use global style for pressed
    lv_obj_set_size(_menuButton, 90, 40); // Example larger size (same height as blocks)
    // Alignment is handled by flex layout of bottomRowContainer

    lv_obj_t* menuLabel = lv_label_create(_menuButton);
    // The button's style (g_style_menu_button_black) should define text color and font.
    // g_style_label_white can be added if specific overrides are needed for the label within the button
    // that are not covered by the button's own text style properties.
    // However, g_style_menu_button_black already sets font and text_align.
    // Text color for the label will be inherited from the button's style in default state.
    // For pressed state, g_style_menu_button_black_pressed sets text_color.
    lv_label_set_text(menuLabel, "MENU");
    lv_obj_center(menuLabel);
    // lv_obj_add_event_cb(menuButton, MenuButtonCallback, LV_EVENT_CLICKED, nullptr);
    // lv_obj_add_event_cb(_menuButton, my_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* shutterBlock = create_inverted_control_block(bottomRowContainer, "Shutter");
    lv_obj_t* wbBlock = create_inverted_control_block(bottomRowContainer, "WB");

    // --- Add interactive blocks to Input Group ---
    // if (m_defaultGroup) // Assuming m_defaultGroup is created in the constructor
    // {
    //     // Add the main container for each block
    //     lv_group_add_obj(m_defaultGroup, fpsBlock);
    //     lv_group_add_obj(m_defaultGroup, aGainBlock);
    //     lv_group_add_obj(m_defaultGroup, dGainBlock);
    //     lv_group_add_obj(m_defaultGroup, menuBlock);
    //     lv_group_add_obj(m_defaultGroup, shutterBlock);
    //     lv_group_add_obj(m_defaultGroup, wbBlock);
    // }
}

void LvglUI::UpdateStatusLabel(const std::string& text)
{ /* ... */
}

// --- Static Event Callback ---
/* static */ void LvglUI::CounterButtonEventCallbackStatic(lv_event_t* event) // Renamed param
{
    auto* instance = static_cast<LvglUI*>(lv_event_get_user_data(event));
    if (instance)
    {
        instance->CounterButtonEventCallbackImpl(event); // Pass renamed param
    }
}

// --- Instance Event Handler ---
void LvglUI::CounterButtonEventCallbackImpl(lv_event_t* event) // Renamed param
{
    lv_event_code_t code = lv_event_get_code(event);
    // lv_obj_t* button = lv_event_get_target(event); // Get target if needed

    if (code == LV_EVENT_CLICKED)
    {
        static uint8_t count = 0;
        count++;
        if (m_counterButtonLabel)
        {
            lv_label_set_text_fmt(m_counterButtonLabel, "Click Count: %d", count);
        }
        UpdateStatusLabel("Button clicked " + std::to_string(count) + " times!");
    }
}

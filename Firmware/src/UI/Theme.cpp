#include "Theme.h"
#include <src/misc/lv_color.h>
#include <src/misc/lv_style_gen.h>

// Define the actual style variables globally within this .cpp file
// (Using 'g_' prefix for global is a common convention, but optional)
lv_style_t g_style_black_sharp_box;
lv_style_t g_style_lightgray_rounded_box;
lv_style_t g_style_label_white;
lv_style_t g_style_value_dark;
lv_style_t g_style_menu_button_black;
lv_style_t g_style_menu_button_black_pressed;
// Define other styles here...

void InitializeTheme()
{
    // --- Black background, sharp corners ---
    lv_style_init(&g_style_black_sharp_box);
    lv_style_set_radius(&g_style_black_sharp_box, 0); // Sharp
    lv_style_set_bg_color(&g_style_black_sharp_box, lv_color_black());
    lv_style_set_bg_opa(&g_style_black_sharp_box, LV_OPA_COVER);
    lv_style_set_border_width(&g_style_black_sharp_box, 0);
    lv_style_set_text_align(&g_style_black_sharp_box, LV_TEXT_ALIGN_CENTER);

    // --- Light gray background, rounded corners ---
    lv_style_init(&g_style_lightgray_rounded_box);
    lv_style_set_radius(&g_style_lightgray_rounded_box, 4); // Example radius
    lv_style_set_bg_color(&g_style_lightgray_rounded_box, lv_color_hex(0xC0C0C0));
    lv_style_set_bg_opa(&g_style_lightgray_rounded_box, LV_OPA_COVER);
    lv_style_set_border_width(&g_style_lightgray_rounded_box, 0);

    // --- White text label style ---
    lv_style_init(&g_style_label_white);
    lv_style_set_text_color(&g_style_label_white, lv_color_white());
    // Ensure the desired font is enabled in lv_conf.h
    lv_style_set_bg_opa(&g_style_label_white, LV_OPA_TRANSP);

    lv_style_set_text_font(&g_style_label_white, &titillium_18);

    // --- Dark text value style ---
    lv_style_init(&g_style_value_dark);
    lv_style_set_text_color(&g_style_value_dark, lv_color_hex(0x303030));
    // Set font if needed (e.g., same as labels or different)    
    lv_style_set_bg_opa(&g_style_value_dark, LV_OPA_TRANSP);
    lv_style_set_text_font(&g_style_value_dark, &titillium_18);

    // static const lv_style_prop_t no_props_to_animate[] = {
    //     LV_STYLE_PROP_INV   /* Array terminator ONLY */
    // };

    static const lv_style_prop_t props_to_make_instant[] = {
        LV_STYLE_BG_COLOR,   // We want background color change to be instant
        LV_STYLE_TEXT_COLOR, // We want text color change to be instant
        // Add any other properties that change and should be instant
        // OMIT TRANSFORM properties if you want to keep size change disabled
        LV_STYLE_PROP_INV // End marker
    };

    static lv_style_transition_dsc_t no_transition_dsc; // static if defined here
    lv_style_transition_dsc_init(&no_transition_dsc,
                                 props_to_make_instant, // Empty property list
                                 lv_anim_path_linear,   // Path doesn't matter
                                 0,                     // Duration 0
                                 0,                     // Delay 0
                                 NULL);

    // --- Style for the larger MENU button ---
    lv_style_init(&g_style_menu_button_black);
    lv_style_set_radius(&g_style_menu_button_black, 4); // Sharp corners
    lv_style_set_bg_color(&g_style_menu_button_black, lv_color_black());
    lv_style_set_bg_opa(&g_style_menu_button_black, LV_OPA_COVER);
    lv_style_set_border_width(&g_style_menu_button_black, 0);
    lv_style_set_text_align(&g_style_menu_button_black, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_font(&g_style_menu_button_black, &titillium_24);
    lv_style_set_transition(&g_style_menu_button_black, &no_transition_dsc);

    lv_style_init(&g_style_menu_button_black_pressed);
    lv_style_set_text_color(&g_style_menu_button_black_pressed, lv_color_black());
    lv_style_set_bg_color(&g_style_menu_button_black_pressed, lv_color_hex(0xFA8756)); // Dark gray on press
    lv_style_set_transition(&g_style_menu_button_black_pressed, &no_transition_dsc);
    lv_style_set_transform_width(&g_style_menu_button_black_pressed, 0);
    lv_style_set_transform_height(&g_style_menu_button_black_pressed, 0);
}
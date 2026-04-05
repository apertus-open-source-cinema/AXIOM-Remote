#pragma once
#include <lvgl.h>

#define MEDIUM_GRAY_BACKGROUND lv_color_hex(0x808080)
#define APERTUS_ORANGE lv_color_hex(0xFA8756)

LV_FONT_DECLARE(Titillium);
LV_FONT_DECLARE(titillium_18);
LV_FONT_DECLARE(titillium_24);

extern lv_style_t g_style_black_sharp_box;
extern lv_style_t g_style_lightgray_rounded_box;
extern lv_style_t g_style_lightgray_rounded_button;
extern lv_style_t g_style_lightgray_rounded_button_pressed;
extern lv_style_t g_style_label_white;
extern lv_style_t g_style_value_dark;
extern lv_style_t g_style_menu_button_black;
extern lv_style_t g_style_menu_button_black_pressed;

void InitializeTheme();
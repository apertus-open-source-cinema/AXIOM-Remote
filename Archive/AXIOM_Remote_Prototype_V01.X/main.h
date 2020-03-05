/*	AXIOM Remote
 **
 **	Copyright (C) 2018 Sebastian Pichelhofer
 * 
 **
 **	This program is free software; you can redistribute it and/or modify
 **    	it under the terms of the GNU General Public License 2 as published 
 **	by the Free Software Foundation.
 **
 **	Compile with -O6 for best experience
 */

#ifndef MAIN_H
#define MAIN_H
/*
#include "definitions.h"

#include "gfxfont.h"
#include "media/trash-icon.c"
#include "media/help-icon.c"
#include "media/home-icon.c"
#include "media/home-icon-highlight.c"
#include "media/back-icon.c"
#include "media/back-icon-highlight.c"
#include "media/up-icon.c"
#include "media/up-icon-highlight.c"
#include "media/down-icon.c"
#include "media/down-icon-highlight.c"
#include "media/wheel-illustration.c"*/

/*extern uint16_t _cursor_x;
extern uint16_t _cursor_y;*/

//extern enum _menu_id_t _current_menu;

//extern uint8_t _menu_offset; // when scrolling the menu this is the offset for the items

//extern uint8_t _parameter_menu_active; // is a parameter menu currently visible (0 = no)
//extern uint8_t _parameter_selection_index; // index of the item currently selected in a parameter menu
/*
enum _menu_id_t {
    menu_none, menu_main, menu_submenu1, menu_submenu2
};
enum _menu_id_t _current_menu;

enum _page_id_t {
    page_none, page_home, page_wb, page_wb_help
};
enum _page_id_t _current_page;

enum _item_type_t {
    value_and_label, button
};

typedef struct {
    char label[32];
    bool disabled;
    bool highlighted;
    enum _item_type_t item_type;
    enum _page_id_t link_to_subpage;
    char value[32];
    bool(*action_ptr)(); //function pointer to the action when page item is clicked
    char* (*current_value_ptr)(); //function pointer to return the current value
    GFXfont fontsize;
    uint16_t text_color;
    uint16_t background_color;
} page_item_t;

typedef struct {
    char label[32];
    enum _page_id_t page_id;
    page_item_t page_item[12];
    uint8_t page_items_count;
} page_t;


uint8_t _button_height;

extern page_t _main_page[3];*/

//extern uint8_t _page_count;


/*
typedef enum {
    P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, REC, PHOTO, E1, E2, S1, S2, E1_rot, E2_rot
} ButtonID;*/

// White Balance related
/*
typedef struct {
    char label[32];
    uint16_t Kelvin;
    int8_t ColorShift;
} wb_option_t;

typedef struct {
    wb_option_t white_balance_options[16];
    uint8_t white_balance_options_count;
    uint8_t white_balance_selection_index;
} white_balance_t;

white_balance_t white_balance;
 */

//extern uint8_t _menu_selection_index;

//extern drop_down_choice_t mainMenuItem2Choices[2];
//extern drop_down_choice_t mainMenuItem3Choices[4];

//extern menu_item_t _menu_main_item[10];

//extern menu_item_t _menu_sub1_item[3];
//extern menu_item_t _menu_sub2_item[5];

/*extern menu_t _main_menu[5];

extern char menu_breadcrumbs[64];*/

// Color Definitions
/*extern uint16_t menu_item_color;
extern uint16_t menu_dimmed_item_color;
extern uint16_t _menu_disabled_item_color;
extern uint16_t menu_selected_item_color;
extern uint16_t _menu_hightlighted_item_color;
extern uint16_t menu_background_color;
extern uint16_t menu_text_color;
extern uint16_t _menu_disabled_text_color;
extern uint16_t _menu_selected_text_color;*/
/*
//extern GFXfont gfxFont;
extern GFXfont _FreeSans9pt7b;
extern GFXfont _FreeSans12pt7b;
extern GFXfont _FreeSans18pt7b;
extern GFXfont _FreeSans24pt7b;*/


#endif /* MAIN_H */

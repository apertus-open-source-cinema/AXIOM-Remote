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

#include "definitions.h"
#include "gfxfont.h"

extern uint16_t _cursor_x;
extern uint16_t _cursor_y;

extern enum _menu_id_t _current_menu;

extern uint8_t _menu_offset; // when scrolling the menu this is the offset for the items

extern uint8_t _parameter_menu_active; // is a parameter menu currently visible (0 = no)
extern uint8_t _parameter_selection_index; // index of the item currently selected in a parameter menu

typedef struct {
    char label[32];
    uint8_t value;
} drop_down_choice_t;

enum _menu_id_t {
    menu_none, menu_main, menu_submenu1, menu_submenu2
};
enum _menu_id_t _current_menu;

typedef enum {
    submenu, readonly, numeric, dropdown
} menu_item_type_t;

typedef struct {
    char label[64];
    bool disabled;
    bool hidden;
    enum _menu_id_t link_to_submenu;
    menu_item_type_t type;
    uint8_t value;
    bool(*action_ptr)(); //function pointer to the action when that menu entry is clicked
    char* (*current_value_ptr)(); //function pointer to return the current value
    drop_down_choice_t choices[30];
    uint8_t choice_count;
} menu_item_t;

typedef struct {
    char label[64];
    enum _menu_id_t menu_id;
    menu_item_t menu_item[64];
    uint8_t menu_items_count;
} menu_t;

extern uint8_t _main_menu_count;

enum _page_id_t {
    page_none, page_home, page_1
};
enum _page_id_t _current_page;

typedef struct {
    char label[64];
    bool disabled;
    enum _page_id_t link_to_subpage;
    uint8_t value;
    bool(*action_ptr)(); //function pointer to the action when page item is clicked
    char* (*current_value_ptr)(); //function pointer to return the current value
} page_item_t;

typedef struct {
    char label[64];
    enum _page_id_t page_id;
    page_item_t page_item[6];
    uint8_t page_items_count;
} page_t;

extern page_t _main_page[3];

extern uint8_t _page_count;


typedef enum {
    align_left, align_center, align_right
} textAlign;


// AXIOM Remote buttons and knobs
extern bool btn_E1_pressed;
extern bool btn_E2_pressed;
extern bool btn_S1_pos;
extern bool btn_TS1_pos;
extern bool btn_TS2_pos;
extern uint8_t E1_pos;
extern uint8_t E2_pos;

extern uint8_t _menu_selection_index;

extern drop_down_choice_t mainMenuItem2Choices[2];
extern drop_down_choice_t mainMenuItem3Choices[4];

//extern menu_item_t _menu_main_item[10];

//extern menu_item_t _menu_sub1_item[3];
//extern menu_item_t _menu_sub2_item[5];

extern menu_t _main_menu[5];

extern char menu_breadcrumbs[64];

// Color Definitions
extern uint16_t menu_item_color;
extern uint16_t menu_dimmed_item_color;
extern uint16_t _menu_disabled_item_color;
extern uint16_t menu_selected_item_color;
extern uint16_t _menu_hightlighted_item_color;
extern uint16_t menu_background_color;
extern uint16_t menu_text_color;
extern uint16_t _menu_disabled_text_color;
extern uint16_t _menu_selected_text_color;

extern GFXfont gfxFont;

extern uint16_t _framebuffer[_width][_height];

#endif /* MAIN_H */
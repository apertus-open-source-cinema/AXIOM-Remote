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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
#include <stdbool.h>
#include "gfxfont.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_16bit(addr) (*(uint16_t)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))

typedef enum menu_id_t {
    MENU_NONE, MENU_MAIN, MENU_SUBMENU1, MENU_SUBMENU2
} menu_id_t;
extern enum menu_id_t current_menu;

typedef enum page_id_t {
    PAGE_NONE, PAGE_HOME, PAGE_WB, PAGE_WB_HELP
} page_id_t;
extern enum page_id_t current_page;

typedef enum {
    TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT
} text_align;

typedef enum ButtonID {
    P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, REC, PHOTO, E1, E2, S1, S2, E1_ROT, E2_ROT
} ButtonID;

typedef enum item_type_t {
    VALUE_AND_LABEL,
    BUTTON
} item_type_t;

typedef struct page_item_t {
    char label[32];
    bool disabled;
    bool highlighted;
    enum item_type_t item_type;
    enum page_id_t link_to_subpage;
    char value[32];
    bool(*action_ptr)(); //function pointer to the action when page item is clicked
    char* (*current_value_ptr)(); //function pointer to return the current value
    GFXfont fontsize;
    uint16_t text_color;
    uint16_t background_color;
} page_item_t;

typedef struct page_t {
    char label[32];
    enum page_id_t page_id;
    page_item_t page_item[12];
    uint8_t page_items_count;
} page_t;
extern page_t main_page[3];

uint8_t button_height;

// White Balance related

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

white_balance_t white_balance_parameter;

extern bool btn_E1_pressed;

#define FRAMEBUFFER_WIDTH 320
#define FRAMEBUFFER_HEIGHT 240
#define FRAMEBUFFER_RIGHT 319
#define FRAMEBUFFER_TOP 239

// the central frame buffer that is written to the LCD
uint16_t framebuffer[FRAMEBUFFER_WIDTH][FRAMEBUFFER_HEIGHT];

//second frame buffer for rendering transition effects
uint16_t transition_framebuffer[FRAMEBUFFER_WIDTH][FRAMEBUFFER_HEIGHT];
uint8_t transition_counter;
bool transition_active;
uint8_t transition_animation_speed;

enum transition_animation {
    TRANSITION_PUSH_LEFT, TRANSITION_PUSH_RIGHT, TRANSITION_PUSH_UP, TRANSITION_PUSH_DOWN, TRANSITION_WIPE_LEFT, TRANSITION_WIPE_RIGHT
};

enum transition_animation transition_animation_type;

extern uint8_t menu_offset; // when scrolling the menu this is the offset for the items

uint8_t parameter_menu_active; // is a parameter menu currently visible (0 = no)
uint8_t parameter_selection_index; // index of the item currently selected in a parameter menu

typedef struct {
    char label[32];
    uint8_t value;
} drop_down_choice_t;

typedef enum {
    MENU_ITEM_TYPE_SUBMENU, MENU_ITEM_TYPE_PAGELINK, MENU_ITEM_TYPE_BACKLINK, MENU_ITEM_TYPE_READONLY, MENU_ITEM_TYPE_NUMERIC, MENU_ITEM_TYPE_DROPDOWN
} menu_item_type_t;

typedef struct {
    char label[64];
    bool disabled;
    bool hidden;
    enum menu_id_t link_to_submenu;
    enum page_id_t link_to_page;
    menu_item_type_t type;
    uint8_t value;
    bool selected;
    bool highlighted;
    bool(*action_ptr)(); //function pointer to the action when that menu entry is clicked
    char* (*current_value_ptr)(); //function pointer to return the current value
    drop_down_choice_t choices[5]; // array holding the discreet choices for this particular option
    uint8_t choice_count;
} menu_item_t;

typedef struct {
    char label[32];
    enum menu_id_t menu_id;
    menu_item_t menu_item[32];
    uint8_t menu_items_count;
    uint8_t menu_selection_index;
} menu_t;

uint8_t main_menu_count;

page_t main_page[3];

uint8_t page_count;

// Buttons and knobs
extern bool btn_E1_pressed;
extern bool btn_E2_pressed;
extern bool btn_P7_pressed;
extern bool btn_P8_pressed;
extern bool btn_S1_pos;
extern bool btn_TS1_pos;
extern bool btn_TS2_pos;
extern uint8_t E1_pos;
extern uint8_t E2_pos;


//Fonts
extern GFXfont _FreeSans9pt7b;
extern GFXfont _FreeSans12pt7b;
extern GFXfont _FreeSans18pt7b;
extern GFXfont _FreeSans24pt7b;

// The position of the text drawing caret (cursor))
extern uint16_t cursor_x;
extern uint16_t cursor_y;

#endif //GLOBALS_H


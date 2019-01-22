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

#define FRAMEBUFFER_WIDTH 320
#define FRAMEBUFFER_HEIGHT 240

uint16_t framebuffer[FRAMEBUFFER_WIDTH][FRAMEBUFFER_HEIGHT];

typedef enum menu_id_t 
{
    MENU_NONE, MENU_MAIN, MENU_SUBMENU1, MENU_SUBMENU2
} menu_id_t;
extern enum menu_id_t current_menu;

typedef enum page_id_t 
{
    PAGE_NONE, PAGE_HOME, PAGE_WB, PAGE_WB_HELP
} page_id_t;
extern enum page_id_t current_page;

typedef enum 
{
    TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT
} text_align;

enum transition_animation {
    TRANSITION_PUSH_LEFT, TRANSITION_PUSH_RIGHT, TRANSITION_PUSH_UP, TRANSITION_PUSH_DOWN, TRANSITION_WIPE_LEFT, TRANSITION_WIPE_RIGHT
} transition_animation;

typedef enum ButtonID {
    P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, REC, PHOTO, E1, E2, S1, S2, E1_rot, E2_rot
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

extern bool btn_E1_pressed;

#define _width 320
#define _height 240
#define _right 319
#define _top 239

extern GFXfont _FreeSans9pt7b;
extern GFXfont _FreeSans12pt7b;
extern GFXfont _FreeSans18pt7b;
extern GFXfont _FreeSans24pt7b;

// Check what it is used for
extern uint16_t _cursor_x;
extern uint16_t _cursor_y;

#endif //GLOBALS_H

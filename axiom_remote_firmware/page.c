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

#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "utility.h"

#ifndef PAGE_C
#define PAGE_C

// Color Definitions
uint16_t _page_item_label_color;
uint16_t _page_item_value_color;
uint16_t _page_item_label_background_color;
uint16_t _page_item_value_background_color;
uint16_t _page_item_highlight_color;
uint16_t _page_background_color;
uint16_t _page_itemframebuffer_width;
uint8_t _padding_side;
uint8_t _padding_elements;
uint8_t _page_item_label_height;
uint8_t _page_item_value_height;


/**************************************************************************/
/*!
    @brief    draw a standard page item that contains a value and label 
    @param    screen_index - position of the item on screen, there is space for 3 top and 3 bottom, total 6 items
    @param    page_item - the struct containing details about the page item
    @param    highlighted - is the item currently highlighted
 */

/**************************************************************************/
void draw_page_item(uint8_t screen_index) {

    // don't draw empty items
    if (main_page[PAGE_HOME].page_item[screen_index].label == NULL) {
        return;
    }

    // the screen only provides space for 6 items
    screen_index = limit_range(screen_index, 0, 5);

    uint16_t page_item_height = _page_item_label_height + _page_item_value_height;

    // derive x,y coordinates from screen_index
    uint16_t x, y;
    if (screen_index == 0) {
        x = _padding_side;
        y = _height - page_item_height;
    }
    if (screen_index == 1) {
        x = _padding_side + _padding_elements + _page_itemframebuffer_width;
        y = _height - page_item_height;
    }
    if (screen_index == 2) {
        x = _padding_side + 2 * _padding_elements + 2 * _page_itemframebuffer_width;
        y = _height - page_item_height;
    }
    if (screen_index == 3) {
        x = _padding_side;
        y = 0;
    }
    if (screen_index == 4) {
        x = _padding_side + _padding_elements + _page_itemframebuffer_width;
        y = 0;
    }
    if (screen_index == 5) {
        x = _padding_side + 2 * _padding_elements + 2 * _page_itemframebuffer_width;
        y = 0;
    }

    // handle item hightlighting
    uint16_t page_item_label_color;
    uint16_t page_item_label_background_color;
    uint16_t page_item_value_background_color;
    uint16_t page_item_value_color;
    if (main_page[PAGE_HOME].page_item[screen_index].highlighted) {
        page_item_label_color = _page_item_label_background_color;
        page_item_label_background_color = _page_item_highlight_color;
        page_item_value_color = _page_item_label_color;
        page_item_value_background_color = _page_item_label_background_color;
    } else {
        page_item_label_color = _page_item_label_color;
        page_item_label_background_color = _page_item_label_background_color;
        page_item_value_background_color = _page_item_value_background_color;
        page_item_value_color = _page_item_value_color;
    }

    if (screen_index < 3) {
        // 3 top items

        if (main_page[PAGE_HOME].page_item[screen_index].item_type == BUTTON) {
            //fill_round_rect(x, y + 2 + _button_height, _page_itemframebuffer_width, _button_height, 3, page_item_label_background_color);
            //draw_string(x, y + 9 + _button_height, main_page[PAGE_HOME].page_item[screen_index].label,
            //        page_item_label_color, page_item_label_color,
            //        _FreeSans12pt7b, TEXT_ALIGN_CENTER, _page_itemframebuffer_width);
        }
        if (main_page[PAGE_HOME].page_item[screen_index].item_type == VALUE_AND_LABEL) {
            // draw label
            fill_round_rect(x, y + _page_item_value_height, _page_itemframebuffer_width, _page_item_label_height, 3,
                    page_item_label_background_color);
            fill_rect(x, y + _page_item_value_height, _page_itemframebuffer_width, 3, 3, page_item_label_background_color);
            draw_string(x, y + _page_item_value_height + 7, main_page[PAGE_HOME].page_item[screen_index].label,
                    page_item_label_color, page_item_value_color,
                    _FreeSans9pt7b, TEXT_ALIGN_CENTER, _page_itemframebuffer_width);

            // draw value
            fill_round_rect(x, y, _page_itemframebuffer_width, _page_item_value_height, 3, page_item_value_background_color);
            fill_rect(x, y + _page_item_value_height - 3, _page_itemframebuffer_width, 3, page_item_value_background_color);
            draw_string(x, y + 10, main_page[PAGE_HOME].page_item[screen_index].value, page_item_value_color, page_item_value_color,
                    _FreeSans12pt7b, TEXT_ALIGN_CENTER, _page_itemframebuffer_width);
        }
    } else {
        // 3 bottom items

        if (main_page[PAGE_HOME].page_item[screen_index].item_type == BUTTON) {
            //fill_round_rect(x, y + 2, _page_itemframebuffer_width, _button_height, 3, page_item_label_background_color);
            draw_string(x, y + 9, main_page[PAGE_HOME].page_item[screen_index].label, page_item_label_color, page_item_label_color,
                    _FreeSans12pt7b, TEXT_ALIGN_CENTER, _page_itemframebuffer_width);
        }
        if (main_page[PAGE_HOME].page_item[screen_index].item_type == VALUE_AND_LABEL) {
            // draw label

            fill_round_rect(x, y + 1, _page_itemframebuffer_width, _page_item_label_height, 3, page_item_label_background_color);
            fill_rect(x, y + _page_item_label_height - 3, _page_itemframebuffer_width, 3, 3, page_item_label_background_color);
            draw_string(x, y + 7, main_page[PAGE_HOME].page_item[screen_index].label, page_item_label_color, page_item_label_color,
                    _FreeSans9pt7b, TEXT_ALIGN_CENTER, _page_itemframebuffer_width);

            // draw value
            fill_round_rect(x, y + _page_item_label_height, _page_itemframebuffer_width, _page_item_value_height, 3, page_item_value_background_color);
            fill_rect(x, y + _page_item_label_height, _page_itemframebuffer_width, 3, page_item_value_background_color);
            draw_string(x, y + _page_item_label_height + 6, main_page[PAGE_HOME].page_item[screen_index].value, page_item_value_color, page_item_value_color,
                    _FreeSans12pt7b, TEXT_ALIGN_CENTER, _page_itemframebuffer_width);
        }
    }
}

/**************************************************************************/
/*!
    @brief    draw one page with its 6 page_items
 */

/**************************************************************************/
void draw_page() {
    //clear the screen
    fill_rect(0, 0, FRAMEBUFFER_WIDTH, _height, _page_background_color);

    uint8_t a;
//    for (a = 0; a < _page_count; a++) {
//        if (main_page[a].page_id == current_page) {
//            uint8_t i;
//            for (i = 0; i < main_page[a].page_items_count; i++) {
//                draw_page_item(i);
//            }
//        }
//    }

    /*
     * Testing font sizes and string length calculations
     * 
     * draw_string(15, 60, "test", _page_item_label_color, _page_item_label_color, _FreeSans9pt7b, align_left, 0);
    fill_rect(15, 55, get_stringframebuffer_width("test", _FreeSans9pt7b), 3, _page_item_value_color);

    draw_string(15, 80, "15", _page_item_label_color, _page_item_label_color, _FreeSans18pt7b, align_left, 0);
    fill_rect(15, 75, get_stringframebuffer_width("15", _FreeSans18pt7b), 3, _page_item_value_color);

    draw_string(15, 110, "A Word", _page_item_label_color, _page_item_label_color, _FreeSans18pt7b, align_left, 0);
    fill_rect(15, 105, get_stringframebuffer_width("A Word", _FreeSans18pt7b), 3, _page_item_value_color);

    draw_string(15, 160, "Long 34", _page_item_label_color, _page_item_label_color, _FreeSans12pt7b, align_left, 0);
    fill_rect(15, 155, get_stringframebuffer_width("Long 34", _FreeSans12pt7b), 3, _page_item_value_color);
     */
}

void init_pages() {
    //colors
    _page_item_label_color = color565(255, 255, 255);
    _page_item_value_color = color565(0, 0, 0);
    _page_item_label_background_color = color565(0, 0, 0);
    _page_item_value_background_color = color565(255, 255, 255);
    _page_item_highlight_color = color565(255, 128, 0);

    _page_background_color = color565(200, 200, 200);


    // dimensions
    _padding_side = 8;
    _padding_elements = 11;
    _page_itemframebuffer_width = 94;
    //_page_item_height = 75;
    _page_item_label_height = 25;
    _page_item_value_height = 40;

    uint8_t j = 0;
    strcpy(main_page[PAGE_HOME].label, "Main Page");
    main_page[PAGE_HOME].page_id = PAGE_HOME;

    main_page[PAGE_HOME].page_item[j].disabled = false;
    strcpy(main_page[PAGE_HOME].page_item[j].label, "FPS");
    strcpy(main_page[PAGE_HOME].page_item[j].value, "...");
    main_page[PAGE_HOME].page_item[j].highlighted = false;
    main_page[PAGE_HOME].page_item[j].item_type = VALUE_AND_LABEL;
    j++;
    main_page[PAGE_HOME].page_item[j].disabled = false;
    strcpy(main_page[PAGE_HOME].page_item[j].label, "Item 2");
    strcpy(main_page[PAGE_HOME].page_item[j].value, "...");
    main_page[PAGE_HOME].page_item[j].highlighted = false;
    main_page[PAGE_HOME].page_item[j].item_type = VALUE_AND_LABEL;
    j++;
    main_page[PAGE_HOME].page_item[j].disabled = false;
    strcpy(main_page[PAGE_HOME].page_item[j].label, "Shutter");
    strcpy(main_page[PAGE_HOME].page_item[j].value, "...");
    main_page[PAGE_HOME].page_item[j].highlighted = false;
    main_page[PAGE_HOME].page_item[j].item_type = VALUE_AND_LABEL;
    j++;
    main_page[PAGE_HOME].page_item[j].disabled = false;
    strcpy(main_page[PAGE_HOME].page_item[j].label, "MENU");
    main_page[PAGE_HOME].page_item[j].highlighted = false;
    main_page[PAGE_HOME].page_item[j].item_type = BUTTON;
    j++;
    main_page[PAGE_HOME].page_item[j].disabled = false;
    strcpy(main_page[PAGE_HOME].page_item[j].label, "Item 5");
    strcpy(main_page[PAGE_HOME].page_item[j].value, "...");
    main_page[PAGE_HOME].page_item[j].highlighted = false;
    main_page[PAGE_HOME].page_item[j].item_type = VALUE_AND_LABEL;
    j++;
    main_page[PAGE_HOME].page_item[j].disabled = false;
    strcpy(main_page[PAGE_HOME].page_item[j].label, "WB");
    //sprintf(main_page[PAGE_HOME].page_item[j].value, "%dK", _white_balance.white_balance_options[_white_balance.white_balance_selection_index].Kelvin);
    main_page[PAGE_HOME].page_item[j].link_to_subpage = PAGE_WB;
    main_page[PAGE_HOME].page_item[j].fontsize = _FreeSans12pt7b;
    main_page[PAGE_HOME].page_item[j].highlighted = false;
    main_page[PAGE_HOME].page_item[j].item_type = VALUE_AND_LABEL;

    main_page[PAGE_HOME].page_items_count = j + 1;

    //current_page = page_home;
    //_page_count = 2;


    //_transition_active = false;
    //_transition_counter = 0;
    //_transition_animation_type = push_left;
}

void update_pages() {
    //sprintf(main_page[PAGE_HOME].page_item[5].value, "%dK", _white_balance.white_balance_options[_white_balance.white_balance_selection_index].Kelvin);
}

void navigate_to_page(enum page_id_t target_page, enum transition_animation animation) {
    // transition animation
    //start_framebuffer_transition(animation, 60);

    current_page = target_page;

    if (current_menu != MENU_NONE) {
        current_menu = MENU_NONE; //navigate_to_menu(MENU_NONE);
    }
}

void main_page_button_press_handler(ButtonID button_index) {
    if (button_index == P1) {
        main_page[PAGE_HOME].page_item[0].highlighted = true;
    }
    if (button_index == P2) {
        main_page[PAGE_HOME].page_item[1].highlighted = true;
    }
    if (button_index == P3) {
        main_page[PAGE_HOME].page_item[2].highlighted = true;
    }
    if (button_index == P4) {
        main_page[PAGE_HOME].page_item[3].highlighted = true;
    }
    if (button_index == P5) {
        main_page[PAGE_HOME].page_item[4].highlighted = true;
    }
    if (button_index == P6) {
        main_page[PAGE_HOME].page_item[5].highlighted = true;
    }
}

void main_page_button_release_handler(ButtonID button_index) {
    if (button_index == P1) {
        main_page[PAGE_HOME].page_item[0].highlighted = false;
    }
    if (button_index == P2) {
        main_page[PAGE_HOME].page_item[1].highlighted = false;
    }
    if (button_index == P3) {
        main_page[PAGE_HOME].page_item[2].highlighted = false;
    }
    if (button_index == P4) {
        main_page[PAGE_HOME].page_item[3].highlighted = false;

        navigate_to_menu(MENU_MAIN);
    }
    if (button_index == P5) {
        main_page[PAGE_HOME].page_item[4].highlighted = false;
    }
    if (button_index == P6) {
        main_page[PAGE_HOME].page_item[5].highlighted = false;

      //  navigate_to_page(main_page[PAGE_HOME].page_item[5].link_to_subpage, TRANSITION_PUSH_UP);
    }
}

#endif /* PAGE_C */

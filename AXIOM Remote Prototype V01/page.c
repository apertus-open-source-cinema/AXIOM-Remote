
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

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "definitions.h"
#include "utility.c"

#ifndef PAGE_C
#define PAGE_C

// Color Definitions
uint16_t page_item_label_color;
uint16_t page_item_value_color;
uint16_t page_item_label_background_color;
uint16_t page_item_value_background_color;
uint16_t page_item_width;
uint16_t page_item_height;

void draw_page_item(uint8_t screen_index, page_item_t page_item, bool selected, bool highlighted) {
    
    // don't draw empty items
    if (page_item.label == NULL) {
        return;
    }
    
    // the screen only provides space for 6 items
    limit_range(screen_index, 0, 5);
    
    uint16_t x, y;
    if (screen_index == 0) {
        x = 10;
        y = _height - page_item_height;
    }
    if (screen_index == 1) {
        x = 115;
        y = _height - page_item_height;
    }
    if (screen_index == 2) {
        x = 220;
        y = _height - page_item_height;
    }
    
    // draw label
    fill_round_rect(x, y, page_item_width, page_item_height / 2, page_item_label_background_color);
    drawString(x + 5, y + 19, page_item.label, page_item_label_background_color, page_item_label_background_color, 1, align_left, 0);

    // draw value
    fill_round_rect(x, y + page_item_height / 2, page_item_width, page_item_height / 2, page_item_value_background_color);
    drawString(x + 210, y + 19, page_item.value, page_item_value_color, page_item_value_color, 1, align_right, 80);
}

void draw_page() {
    uint8_t a;
    for (a = 0; a < _page_count; a++) {
        if (_main_page[a].page_id == _current_page) {
            uint8_t i;
            for (i = 0; i < _main_page[a].page_items_count; i++) {
                draw_page_item(a, _main_page[a].page_item[i], false, false);
            }
        }
    }
}

void init_pages() {

    //colors
    page_item_label_color = color565(255, 255, 255);
    page_item_value_color = color565(0, 0, 0);
    page_item_label_background_color = color565(0, 0, 0);
    page_item_value_background_color = color565(255, 255, 255);

    // dimensions
    page_item_width = 90;
    page_item_height = 40;


    uint8_t j = 0;
    strcpy(_main_page[page_home].label, "Main Page");
    _main_page[page_home].page_id = page_home;

    _main_page[page_home].page_item[j].disabled = false;
    strcpy(_main_page[page_home].page_item[j].label, "Item 1");
    //_main_page[page_home].page_item[j].link_to_submenu = menu_submenu1;
    //_main_page[page_home].page_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;

    _current_page = 0;
    _page_count = 2;
}

#endif /* PAGE_C */
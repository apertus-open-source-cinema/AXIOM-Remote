
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
uint16_t _page_item_label_color;
uint16_t _page_item_value_color;
uint16_t _page_item_label_background_color;
uint16_t _page_item_value_background_color;
uint16_t _page_background_color;
uint16_t _page_item_width;
uint16_t _page_item_height;
uint8_t _padding_side;
uint8_t _padding_elements;

void draw_page_item(uint8_t screen_index, page_item_t page_item, bool selected, bool highlighted) {

    // don't draw empty items
    if (page_item.label == NULL) {
        return;
    }

    // the screen only provides space for 6 items
    limit_range(screen_index, 0, 5);

    uint16_t x, y;
    if (screen_index == 0) {
        x = _padding_side;
        y = _height - _page_item_height;
    }
    if (screen_index == 1) {
        x = _padding_side + _padding_elements + _page_item_width;
        y = _height - _page_item_height;
        ;
    }
    if (screen_index == 2) {
        x = _padding_side + 2 * _padding_elements + 2 * _page_item_width;
        y = _height - _page_item_height;
        ;
    }
    if (screen_index == 3) {
        x = _padding_side;
        y = 0;
    }
    if (screen_index == 4) {
        x = _padding_side + _padding_elements + _page_item_width;
        y = 0;
    }
    if (screen_index == 5) {
        x = _padding_side + 2 * _padding_elements + 2 * _page_item_width;
        y = 0;
    }

    if (screen_index < 3) {
        // 3 top items

        // draw label
        fill_round_rect(x, y + _page_item_height * 2 / 3, _page_item_width, _page_item_height / 3, 3, _page_item_label_background_color);
        fill_rect(x, y + _page_item_height * 2 / 3, _page_item_width, 3, 3, _page_item_label_background_color);
        draw_string(x, y + _page_item_height * 2 / 3 + 7, page_item.label, _page_item_label_color, _page_item_label_color, _FreeSans9pt7b, align_center, _page_item_width);

        // draw value
        fill_round_rect(x, y, _page_item_width, _page_item_height * 2 / 3, 3, _page_item_value_background_color);
        fill_rect(x, y + _page_item_height * 2 / 3 - 3, _page_item_width, 3, _page_item_value_background_color);
        char value[16];
        sprintf(value, "%d", page_item.value);
        draw_string(x, y + 10, value, _page_item_value_color, _page_item_value_color, _FreeSans18pt7b, align_center, _page_item_width);
    } else {
        // 3 bottom items

        // draw label
        fill_round_rect(x, y + 1, _page_item_width, _page_item_height / 3, 3, _page_item_label_background_color);
        fill_rect(x, y + _page_item_height / 3 - 3, _page_item_width, 3, 3, _page_item_label_background_color);
        draw_string(x, y + 7, page_item.label, _page_item_label_color, _page_item_label_color, _FreeSans9pt7b, align_center, _page_item_width);

        // draw value
        fill_round_rect(x, y + _page_item_height / 3, _page_item_width, _page_item_height * 2 / 3, 3, _page_item_value_background_color);
        fill_rect(x, y + _page_item_height / 3, _page_item_width, 3, _page_item_value_background_color);
        char value[16];
        sprintf(value, "%d", page_item.value);
        draw_string(x, y + _page_item_height / 3 + 10, value, _page_item_value_color, _page_item_value_color, _FreeSans18pt7b, align_center, _page_item_width);
    }

}

void draw_page() {
    //clear the screen
    fill_rect(0, 0, _width, _height, _page_background_color);

    uint8_t a;
    for (a = 0; a < _page_count; a++) {
        if (_main_page[a].page_id == _current_page) {
            uint8_t i;
            for (i = 0; i < _main_page[a].page_items_count; i++) {
                //draw_page_item(i, _main_page[a].page_item[i], false, false);
            }
        }
    }

    draw_string(15, 60, "test", _page_item_label_color, _page_item_label_color, _FreeSans9pt7b, align_left, 0);
    fill_rect(15, 55, get_string_width("test", _FreeSans9pt7b), 3, _page_item_value_color);

    draw_string(15, 80, "15", _page_item_label_color, _page_item_label_color, _FreeSans18pt7b, align_left, 0);
    fill_rect(15, 75, get_string_width("15", _FreeSans18pt7b), 3, _page_item_value_color);

    draw_string(15, 110, "A Word", _page_item_label_color, _page_item_label_color, _FreeSans18pt7b, align_left, 0);
    fill_rect(15, 105, get_string_width("A Word", _FreeSans18pt7b), 3, _page_item_value_color);

    draw_string(15, 160, "Long 34", _page_item_label_color, _page_item_label_color, _FreeSans12pt7b, align_left, 0);
    fill_rect(15, 155, get_string_width("Long 34", _FreeSans12pt7b), 3, _page_item_value_color);


}

void init_pages() {
    //colors
    _page_item_label_color = color565(255, 255, 255);
    _page_item_value_color = color565(0, 0, 0);
    _page_item_label_background_color = color565(0, 0, 0);
    _page_item_value_background_color = color565(255, 255, 255);
    _page_background_color = color565(200, 200, 200);

    // dimensions
    _padding_side = 8;
    _padding_elements = 11;
    _page_item_width = 94;
    _page_item_height = 75;


    uint8_t j = 0;
    strcpy(_main_page[page_home].label, "Main Page");
    _main_page[page_home].page_id = page_home;

    _main_page[page_home].page_item[j].disabled = false;
    strcpy(_main_page[page_home].page_item[j].label, "FPS");
    _main_page[page_home].page_item[j].value = 100;
    //_main_page[page_home].page_item[j].link_to_submenu = menu_submenu1;
    //_main_page[page_home].page_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_page[page_home].page_item[j].disabled = false;
    strcpy(_main_page[page_home].page_item[j].label, "Item 2");
    _main_page[page_home].page_item[j].value = 5;
    //_main_page[page_home].page_item[j].link_to_submenu = menu_submenu1;
    //_main_page[page_home].page_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_page[page_home].page_item[j].disabled = false;
    strcpy(_main_page[page_home].page_item[j].label, "Shutter");
    _main_page[page_home].page_item[j].value = 200;
    //_main_page[page_home].page_item[j].link_to_submenu = menu_submenu1;
    //_main_page[page_home].page_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_page[page_home].page_item[j].disabled = false;
    strcpy(_main_page[page_home].page_item[j].label, "Item 4");
    _main_page[page_home].page_item[j].value = 0;
    j++;
    _main_page[page_home].page_item[j].disabled = false;
    strcpy(_main_page[page_home].page_item[j].label, "Item 5");
    _main_page[page_home].page_item[j].value = 15;
    j++;
    _main_page[page_home].page_item[j].disabled = false;
    strcpy(_main_page[page_home].page_item[j].label, "WB");
    _main_page[page_home].page_item[j].value = 200;

    _main_page[page_home].page_items_count = j + 1;

    _current_page = page_none;
    _page_count = 2;
}

#endif /* PAGE_C */
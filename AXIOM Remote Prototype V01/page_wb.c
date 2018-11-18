
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
//#include "main.c"

#ifndef PAGE_WB_C
#define PAGE_WB_C

// Color Definitions
uint16_t _page_item_label_color;
uint16_t _page_item_value_color;
uint16_t _page_item_label_background_color;
uint16_t _page_item_value_background_color;
uint16_t _page_item_highlight_color;
uint16_t _page_background_color;
uint16_t _page_options_background_color;
uint16_t _page_item_width;
uint8_t _padding_side;
uint8_t _padding_elements;
uint8_t _page_item_label_height;
uint8_t _page_item_value_height;
menu_t _wb_menu;
uint8_t _wb_menu_selection_index;


/**************************************************************************/
/*!
    @brief    draw a standard page item that contains a value and label 
    @param    screen_index - position of the item on screen, there is space for 3 top and 3 bottom, total 6 items
    @param    page_item - the struct containing details about the page item
    @param    highlighted - is the item currently highlighted
 */

/**************************************************************************/
void draw_wb_page_item(uint8_t screen_index) {

    // don't draw empty items
    //if ((page_item.label == NULL) || (strcmp(page_item.label, ""))) {
    if (_main_page[page_wb].page_item[screen_index].label == NULL) {
        return;
    }

    // the screen only provides space for 6 items
    limit_range(screen_index, 0, 5);

    uint16_t page_item_height = _page_item_label_height + _page_item_value_height;

    // derive x,y coordinates from screen_index
    uint16_t x, y;
    if (screen_index == 0) {
        x = _padding_side;
        y = _height - page_item_height;
    }
    if (screen_index == 1) {
        x = _padding_side + _padding_elements + _page_item_width;
        y = _height - page_item_height;
    }
    if (screen_index == 2) {
        x = _padding_side + 2 * _padding_elements + 2 * _page_item_width;
        y = _height - page_item_height;
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

    // handle item hightlighting
    uint16_t page_item_label_color;
    uint16_t page_item_label_background_color;
    uint16_t page_item_value_background_color;
    uint16_t page_item_value_color;
    if (_main_page[page_wb].page_item[screen_index].highlighted) {
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

        // draw a special page item that only has a label instead of value/label 
        if (_main_page[page_wb].page_item[screen_index].label_only) {
            // draw label
            fill_round_rect(x, y + 1 + page_item_height / 3, _page_item_width, page_item_height * 2 / 3, 3, page_item_label_background_color);
            draw_string(x, y + 10 + page_item_height / 3, _main_page[page_wb].page_item[screen_index].label, page_item_label_color, page_item_label_color,
                    _FreeSans12pt7b, align_center, _page_item_width);
        } else {
            // draw label
            fill_round_rect(x, y + _page_item_value_height, _page_item_width, _page_item_label_height, 3, page_item_value_background_color);
            fill_rect(x, y + _page_item_value_height, _page_item_width, 3, 3, page_item_value_background_color);
            draw_string(x, y + _page_item_value_height + 7, _main_page[page_wb].page_item[screen_index].label, page_item_value_color, page_item_value_color,
                    _FreeSans9pt7b, align_center, _page_item_width);

            // draw value
            fill_round_rect(x, y, _page_item_width, _page_item_value_height, 3, page_item_value_background_color);
            fill_rect(x, y + _page_item_value_height - 3, _page_item_width, 3, page_item_value_background_color);
            char value[16];
            sprintf(value, "%d", _main_page[page_wb].page_item[screen_index].value);
            draw_string(x, y + 10, value, page_item_value_color, page_item_value_color, _FreeSans18pt7b, align_center, _page_item_width);
        }
    } else {
        // 3 bottom items

        // draw a special page item that only has a label instead of value/label 
        if (_main_page[page_wb].page_item[screen_index].label_only) {
            // draw label
            fill_round_rect(x, y + 1, _page_item_width, page_item_height * 2 / 3, 3, page_item_label_background_color);
            draw_string(x, y + 10, _main_page[page_wb].page_item[screen_index].label, page_item_label_color, page_item_label_color,
                    _FreeSans12pt7b, align_center, _page_item_width);
        } else {
            // draw label

            fill_round_rect(x, y + 1, _page_item_width, _page_item_label_height, 3, page_item_label_background_color);
            fill_rect(x, y + _page_item_label_height - 3, _page_item_width, 3, 3, page_item_label_background_color);
            draw_string(x, y + 7, _main_page[page_wb].page_item[screen_index].label, page_item_label_color, page_item_label_color,
                    _FreeSans9pt7b, align_center, _page_item_width);

            // draw value
            fill_round_rect(x, y + _page_item_label_height, _page_item_width, _page_item_value_height, 3, page_item_value_background_color);
            fill_rect(x, y + _page_item_label_height, _page_item_width, 3, page_item_value_background_color);
            char value[16];
            sprintf(value, "%d", _main_page[page_wb].page_item[screen_index].value);
            draw_string(x, y + _page_item_label_height + 6, value, page_item_value_color, page_item_value_color,
                    _FreeSans18pt7b, align_center, _page_item_width);
        }
    }
}

bool are_top_page_items_label_only() {
    return (_main_page[page_wb].page_item[0].label_only && _main_page[page_wb].page_item[1].label_only && _main_page[page_wb].page_item[2].label_only);
}

bool are_bottom_page_items_label_only() {
    return (_main_page[page_wb].page_item[3].label_only && _main_page[page_wb].page_item[4].label_only && _main_page[page_wb].page_item[5].label_only);
}

void draw_wb_option_item(uint16_t x, uint16_t y, uint8_t option_item_index, bool selected) {
    // don't draw empty items
    if (_wb_menu.menu_item[option_item_index].label == NULL) {
        return;
    }

    uint16_t yoffset_label_from_base = 7;

    // is the current line selected (cursor)?
    if (selected) {
        fill_rect(x, y, _width - x, 29, _page_item_highlight_color);
        draw_string(x + 5, y + yoffset_label_from_base, _wb_menu.menu_item[option_item_index].label, _page_item_label_color, _page_item_label_color, _FreeSans9pt7b, align_left, 0);
        //draw_string(x + 210, y + yoffset_label_from_base, value, _menu_selected_text_color, _menu_selected_text_color, _FreeSans9pt7b, align_right, 80);
        return;
    }

    //draw the option item normally
    //fill_rect(x, y, _width, 29, _page_item_value_background_color);
    draw_string(x + 5, y + yoffset_label_from_base, _wb_menu.menu_item[option_item_index].label, _page_item_value_color, _page_item_value_color, _FreeSans9pt7b, align_left, 0);
    //draw_string(x + 210, y + yoffset_label_from_base, value, menu_text_color, menu_text_color, _FreeSans9pt7b, align_right, 80);
}
/**************************************************************************/
/*!
    @brief    draw one page with its 6 page_items
 */

/**************************************************************************/
void draw_wb_page() {
    //clear the screen
    fill_rect(0, 0, _width, _height, _page_background_color);


    //draw page items
    uint8_t i;
    for (i = 0; i < _main_page[page_wb].page_items_count; i++) {
        draw_wb_page_item(i);
    }


    // draw options area
    uint16_t page_item_height = _page_item_label_height + _page_item_value_height;
    uint16_t available_height = _height - 2 * page_item_height - 4;
    uint16_t offset = 0;
    if (are_top_page_items_label_only()) {
        available_height += page_item_height / 3;
    }
    if (are_bottom_page_items_label_only()) {
        available_height += page_item_height / 3;
        offset = page_item_height / 3;
    }

    // Draw header
    fill_rect(0, page_item_height + 2 - offset, _width, available_height, _page_options_background_color);
    draw_string(5, page_item_height + 2 - offset + available_height - 18, _main_page[page_wb].label, _page_item_value_color, _page_item_value_color,
            _FreeSans9pt7b, align_left, 0);

    //draw circle as rotary knob indicator
    //TODO calculate center instead of hardcoded values
    fillCircle(55, 100, 35, _page_item_value_color);
    fillCircle(55, 100, 32, _page_options_background_color);

    fillCircle(55, 100, 11, _page_item_value_color);
    fillCircle(55, 100, 8, _page_options_background_color);


    //draw option items
    uint8_t j;
    for (j = 0; j < _wb_menu.menu_items_count; j++) {
        draw_wb_option_item(150, page_item_height + 2 - offset + available_height - 18 - 30 - j * 30, j, (_wb_menu_selection_index == j));
    }

}

void init_wb_page() {
    //colors
    _page_item_label_color = color565(255, 255, 255);
    _page_item_value_color = color565(0, 0, 0);
    _page_item_label_background_color = color565(0, 0, 0);
    _page_item_value_background_color = color565(255, 255, 255);
    _page_item_highlight_color = color565(255, 128, 0);

    _page_background_color = color565(200, 200, 200);
    _page_options_background_color = color565(255, 255, 255);


    // dimensions
    _padding_side = 8;
    _padding_elements = 11;
    _page_item_width = 94;
    //_page_item_height = 75;
    _page_item_label_height = 25;
    _page_item_value_height = 40;

    uint8_t j = 0;
    strcpy(_main_page[page_wb].label, "White Balance");
    _main_page[page_wb].page_id = page_wb;

    _main_page[page_wb].page_item[j].disabled = false;
    strcpy(_main_page[page_wb].page_item[j].label, "Preset 1");
    _main_page[page_wb].page_item[j].value = 0;
    _main_page[page_wb].page_item[j].highlighted = false;
    _main_page[page_wb].page_item[j].label_only = false;
    j++;
    _main_page[page_wb].page_item[j].disabled = false;
    strcpy(_main_page[page_wb].page_item[j].label, "Up");
    _main_page[page_wb].page_item[j].value = 1;
    _main_page[page_wb].page_item[j].highlighted = false;
    _main_page[page_wb].page_item[j].label_only = true;
    j++;
    _main_page[page_wb].page_item[j].disabled = false;
    strcpy(_main_page[page_wb].page_item[j].label, "Preset 2");
    _main_page[page_wb].page_item[j].value = 0;
    _main_page[page_wb].page_item[j].highlighted = false;
    _main_page[page_wb].page_item[j].label_only = false;
    j++;
    _main_page[page_wb].page_item[j].disabled = false;
    strcpy(_main_page[page_wb].page_item[j].label, "Cancel");
    _main_page[page_wb].page_item[j].highlighted = false;
    _main_page[page_wb].page_item[j].label_only = true;
    j++;
    _main_page[page_wb].page_item[j].disabled = false;
    strcpy(_main_page[page_wb].page_item[j].label, "Down");
    _main_page[page_wb].page_item[j].value = 15;
    _main_page[page_wb].page_item[j].highlighted = false;
    _main_page[page_wb].page_item[j].label_only = true;
    j++;
    _main_page[page_wb].page_item[j].disabled = false;
    strcpy(_main_page[page_wb].page_item[j].label, "OK");
    _main_page[page_wb].page_item[j].value = 200;
    _main_page[page_wb].page_item[j].highlighted = false;
    _main_page[page_wb].page_item[j].label_only = true;

    _main_page[page_wb].page_items_count = j + 1;

    uint8_t i = 0;
    _wb_menu.menu_item[i].disabled = false;
    _wb_menu.menu_item[i].hidden = false;
    strcpy(_wb_menu.menu_item[i].label, "Daylight");
    i++;
    _wb_menu.menu_item[i].disabled = false;
    _wb_menu.menu_item[i].hidden = false;
    strcpy(_wb_menu.menu_item[i].label, "Tungsten");
    i++;
    _wb_menu.menu_item[i].disabled = false;
    _wb_menu.menu_item[i].hidden = false;
    strcpy(_wb_menu.menu_item[i].label, "Fluorescent");
    _wb_menu.menu_items_count = i + 1;

    _wb_menu_selection_index = 0;
}

void wb_page_button_press_handler(ButtonID button_index) {
    if (button_index == P1) {
        _main_page[page_wb].page_item[0].highlighted = true;
        draw_lcd();
    }
    if (button_index == P2) {
        _main_page[page_wb].page_item[1].highlighted = true;
        draw_lcd();
    }
    if (button_index == P3) {
        _main_page[page_wb].page_item[2].highlighted = true;
        draw_lcd();
    }
    if (button_index == P4) {
        _main_page[page_wb].page_item[3].highlighted = true;
        draw_lcd();
    }
    if (button_index == P5) {
        _main_page[page_wb].page_item[4].highlighted = true;
        draw_lcd();
    }
    if (button_index == P6) {

        _main_page[page_wb].page_item[5].highlighted = true;
        draw_lcd();
    }
}

void wb_page_button_release_handler(ButtonID button_index) {
    if (button_index == P1) {
        _main_page[page_wb].page_item[0].highlighted = false;
        draw_lcd();
    }
    if (button_index == P2) {
        _main_page[page_wb].page_item[1].highlighted = false;
        _wb_menu_selection_index--;
        _wb_menu_selection_index = limit_range(_wb_menu_selection_index, 0, _wb_menu.menu_items_count - 1);
        draw_lcd();
    }
    if (button_index == P3) {
        _main_page[page_wb].page_item[2].highlighted = false;
        draw_lcd();
    }
    if (button_index == P4) {
        _main_page[page_wb].page_item[3].highlighted = false;
        _current_page = page_home;
        draw_lcd();
    }
    if (button_index == P5) {
        _main_page[page_wb].page_item[4].highlighted = false;
        _wb_menu_selection_index++;
        _wb_menu_selection_index = limit_range(_wb_menu_selection_index, 0, _wb_menu.menu_items_count - 1);
        draw_lcd();
    }
    if (button_index == P6) {
        _main_page[page_wb].page_item[5].highlighted = false;
        //_current_page = page_home;
        draw_lcd();
    }
    if (button_index == P7) {
        _current_page = page_home;
        draw_lcd();
    }
}

#endif /* PAGE_WB_C */
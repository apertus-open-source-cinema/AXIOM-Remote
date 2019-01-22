
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

#ifndef MENU_C
#define MENU_C

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "definitions.h"
#include "utility.c"

// Menu related stuff
//uint8_t _menu_selection_index = 0; // index of the currently selected item in the menu
uint8_t _menu_offset = 0; // when scrolling the menu this is the offset for the items
uint8_t _parameter_menu_active; // is a parameter menu currently visible (0 = no)
uint8_t _parameter_selection_index; // index of the item currently selected in a parameter menu
menu_t _main_menu[5];
uint8_t _main_menu_count;
char menu_breadcrumbs[64];

//page_t _side_icons;
bool home_icon_highlighted;
bool back_icon_highlighted;


// Color Definitions
uint16_t _menu_item_color;
uint16_t _menu_dimmed_item_color;
uint16_t _menu_disabled_item_color;
uint16_t _menu_selected_item_color;
uint16_t _menu_hightlighted_item_color;
uint16_t _menu_background_color;
uint16_t _menu_text_color;
uint16_t _menu_disabled_text_color;
uint16_t _menu_selected_text_color;

uint8_t get_current_menu_item_count() {
    uint8_t a;
    for (a = 0; a < _main_menu_count; a++) {
        if (_main_menu[a].menu_id == _current_menu) {
            return _main_menu[a].menu_items_count;
        }
    }
    return 0;
}

uint8_t get_current_item_choice_count() {
    uint8_t a;
    for (a = 0; a < _main_menu_count; a++) {
        if (_main_menu[a].menu_id == _current_menu) {
            return _main_menu[a].menu_item[_main_menu[a].menu_selection_index].choice_count;
        }
    }
    return 0;
}

char * menu_item_test_get_current_value(uint8_t menu, uint8_t menu_selection_index) {
    return _main_menu[menu].menu_item[menu_selection_index].choices[_main_menu[menu].menu_item[menu_selection_index].value].label;
}

void draw_menu_item(uint16_t x, uint16_t y, uint8_t menu_index, uint8_t menu_main_item_index) {
    //void draw_menu_item(uint16_t x, uint16_t y, menu_item_t menu_main_item, bool selected, bool highlighted) {
    uint16_t yoffset_label_from_base = 7;


    // don't draw empty items
    if (_main_menu[menu_index].menu_item[menu_main_item_index].label == NULL) {
        return;
    }

    char value[16];

    if (_main_menu[menu_index].menu_item[menu_main_item_index].type == SUBMENU) {
        // SUBMENU items are indicated by a ">" sign
        sprintf(value, ">");
    } else if (_main_menu[menu_index].menu_item[menu_main_item_index].type == PAGELINK) {
        // page links have no value icon displayed 
        sprintf(value, "");
    } else if (_main_menu[menu_index].menu_item[menu_main_item_index].type == BACKLINK) {
        // BACKLINKs point to a menu higher in the menu hierarchy
        sprintf(value, "");
    } else {
        if (_main_menu[menu_index].menu_item[menu_main_item_index].current_value_ptr != NULL) {
            strcpy(value, (*_main_menu[menu_index].menu_item[menu_main_item_index].current_value_ptr)(menu_index, menu_main_item_index));
            //sprintf(value, "%s", (*_main_menu[menu_index].menu_item[menu_main_item_index].current_value_ptr)(menu_index, menu_main_item_index));
        } else {
            sprintf(value, "%d", _main_menu[menu_index].menu_item[menu_main_item_index].value);
        }
    }

    // is the current line highlighted and not disabled?
    if (_main_menu[menu_index].menu_item[menu_main_item_index].highlighted && !_main_menu[menu_index].menu_item[menu_main_item_index].disabled) {
        fill_rect(x, y, _width - x, 29, _menu_hightlighted_item_color);

        //label
        draw_string(x + 5, y + yoffset_label_from_base, _main_menu[menu_index].menu_item[menu_main_item_index].label,
                _menu_selected_text_color, _menu_selected_text_color, _FreeSans9pt7b, ALIGN_LEFT, 0);

        //value
        draw_string(x + 180, y + yoffset_label_from_base, value, _menu_selected_text_color,
                _menu_selected_text_color, _FreeSans9pt7b, ALIGN_RIGHT, 80);
        return;
    }

    // is a parameter menu active currently and the item is disabled?
    if (_parameter_menu_active && _main_menu[menu_index].menu_item[menu_main_item_index].disabled) {
        fill_rect(x, y, _width - x, 29, _menu_dimmed_item_color);

        //label
        draw_string(x + 5, y + yoffset_label_from_base, _main_menu[menu_index].menu_item[menu_main_item_index].label,
                _menu_disabled_text_color, _menu_disabled_text_color, _FreeSans9pt7b, ALIGN_LEFT, 0);

        //value
        draw_string(x + 180, y + yoffset_label_from_base, value, _menu_disabled_text_color, _menu_disabled_text_color,
                _FreeSans9pt7b, ALIGN_RIGHT, 80);
        return;
    }

    // is a parameter menu active currently?
    if (_parameter_menu_active) {
        fill_rect(x, y, _width - x, 29, _menu_dimmed_item_color);

        //label
        draw_string(x + 5, y + yoffset_label_from_base, _main_menu[menu_index].menu_item[menu_main_item_index].label,
                _menu_text_color, _menu_text_color, _FreeSans9pt7b, ALIGN_LEFT, 0);

        //value
        draw_string(x + 180, y + yoffset_label_from_base, value, _menu_text_color, _menu_text_color,
                _FreeSans9pt7b, ALIGN_RIGHT, 80);
        return;
    }

    // is the current line selected and disabled?
    if (_main_menu[menu_index].menu_item[menu_main_item_index].selected && _main_menu[menu_index].menu_item[menu_main_item_index].disabled) {
        fill_rect(x, y, _width - x, 29, _menu_disabled_item_color);

        //disabled indicators on the side
        fill_rect(30, y, 4, 29, _menu_selected_item_color);
        fill_rect(_width - 16 - 4, y, 4, 29, _menu_selected_item_color);

        //label
        draw_string(x + 5, y + yoffset_label_from_base, _main_menu[menu_index].menu_item[menu_main_item_index].label,
                _menu_disabled_text_color, _menu_disabled_text_color, _FreeSans9pt7b, ALIGN_LEFT, 0);

        //value
        draw_string(x + 180, y + yoffset_label_from_base, value, _menu_disabled_text_color, _menu_disabled_text_color,
                _FreeSans9pt7b, ALIGN_RIGHT, 80);
        return;
    }

    // is the current line disabled?
    if (_main_menu[menu_index].menu_item[menu_main_item_index].disabled) {
        fill_rect(x, y, _width - x, 29, _menu_disabled_item_color);

        //label
        draw_string(x + 5, y + yoffset_label_from_base, _main_menu[menu_index].menu_item[menu_main_item_index].label,
                _menu_disabled_text_color, _menu_disabled_text_color, _FreeSans9pt7b, ALIGN_LEFT, 0);

        //value
        draw_string(x + 180, y + yoffset_label_from_base, value, _menu_disabled_text_color, _menu_disabled_text_color,
                _FreeSans9pt7b, ALIGN_RIGHT, 80);
        return;
    }

    // is the current line selected (cursor)?
    if (_main_menu[menu_index].menu_item[menu_main_item_index].selected) {
        fill_rect(x, y, _width - x, 29, _menu_selected_item_color);

        //value
        draw_string(x + 5, y + yoffset_label_from_base, _main_menu[menu_index].menu_item[menu_main_item_index].label,
                _menu_selected_text_color, _menu_selected_text_color, _FreeSans9pt7b, ALIGN_LEFT, 0);

        //label
        draw_string(x + 180, y + yoffset_label_from_base, value, _menu_selected_text_color, _menu_selected_text_color,
                _FreeSans9pt7b, ALIGN_RIGHT, 80);
        return;
    }

    //if nothing of the above applies simply draw the line item normally
    fill_rect(x, y, _width - x, 29, _menu_item_color);

    //label
    draw_string(x + 5, y + yoffset_label_from_base, _main_menu[menu_index].menu_item[menu_main_item_index].label,
            _menu_text_color, _menu_text_color, _FreeSans9pt7b, ALIGN_LEFT, 0);

    //value
    draw_string(x + 180, y + yoffset_label_from_base, value, _menu_text_color, _menu_text_color,
            _FreeSans9pt7b, ALIGN_RIGHT, 80);
}

void draw_scroll_indicator(uint8_t current_menu_item_screen_count, uint8_t current_menu_item_count) {
    // maximum height is the screen without header area
    uint8_t srollbar_max_height = _height - 30;

    // height of the scroll indicator is defined by the ratio of number of items on screen vs total number of item. 
    // if there are 7 items on screen of total 14 items the scroll indicator shall be 50% of the scrollbar height
    uint8_t scrollbarheight = srollbar_max_height * (float) ((float) current_menu_item_screen_count / (float) (current_menu_item_count));

    //
    uint8_t scrollbaroffset = ((current_menu_item_count - current_menu_item_screen_count) - _menu_offset) * ((srollbar_max_height - scrollbarheight) / (current_menu_item_count - current_menu_item_screen_count));

    //Background
    fill_rect(_width - 16, 0, 16, _height - 30, _menu_item_color);

    //Thin Line
    fill_rect(_width - 9, 0, 4, _height - 30, _menu_text_color);

    //Thick Line
    fill_rect(_width - 13, scrollbaroffset, 12, scrollbarheight, _menu_text_color);
}

/**************************************************************************/
/*!
   @brief  initialize our menu structure with default values and definitions
 * 
 */

/**************************************************************************/
void init_menus() {
    //Color Theme definitions
    _menu_item_color = color565(255, 255, 255);
    _menu_dimmed_item_color = color565(247, 251, 247);
    _menu_disabled_item_color = color565(255, 255, 255);
    _menu_selected_item_color = color565(255, 128, 0);
    _menu_background_color = color565(180, 180, 180);
    _menu_text_color = color565(0, 0, 0);
    _menu_selected_text_color = color565(255, 255, 255);
    _menu_hightlighted_item_color = color565(0, 128, 255);
    _menu_disabled_text_color = color565(230, 230, 230);

    uint8_t j = 0;
    strcpy(_main_menu[menu_main].label, "Main Menu");
    _main_menu[menu_main].menu_id = menu_main;

    _main_menu[menu_main].menu_item[j].disabled = false;
    _main_menu[menu_main].menu_item[j].hidden = false;
    _main_menu[menu_main].menu_item[j].selected = false;
    _main_menu[menu_main].menu_item[j].highlighted = false;
    strcpy(_main_menu[menu_main].menu_item[j].label, "Exit Menu");
    _main_menu[menu_main].menu_item[j].type = PAGELINK;
    _main_menu[menu_main].menu_item[j].link_to_page = PAGE_HOME;
    //_main_menu[menu_main].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_menu[menu_main].menu_item[j].disabled = false;
    _main_menu[menu_main].menu_item[j].hidden = false;
    _main_menu[menu_main].menu_item[j].selected = false;
    _main_menu[menu_main].menu_item[j].highlighted = false;
    strcpy(_main_menu[menu_main].menu_item[j].label, "SUBMENU 1");
    _main_menu[menu_main].menu_item[j].type = SUBMENU;
    _main_menu[menu_main].menu_item[j].link_to_SUBMENU = MENU_SUBMENU1;
    //_main_menu[menu_main].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_menu[menu_main].menu_item[j].disabled = false;
    _main_menu[menu_main].menu_item[j].hidden = false;
    _main_menu[menu_main].menu_item[j].selected = false;
    _main_menu[menu_main].menu_item[j].highlighted = false;
    strcpy(_main_menu[menu_main].menu_item[j].label, "SUBMENU 2");
    _main_menu[menu_main].menu_item[j].type = SUBMENU;
    _main_menu[menu_main].menu_item[j].link_to_SUBMENU = MENU_SUBMENU2;
    //_main_menu[menu_main].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_menu[menu_main].menu_item[j].disabled = true;
    _main_menu[menu_main].menu_item[j].hidden = false;
    _main_menu[menu_main].menu_item[j].selected = false;
    _main_menu[menu_main].menu_item[j].highlighted = false;
    strcpy(_main_menu[menu_main].menu_item[j].label, "Disabled Item");
    _main_menu[menu_main].menu_item[j].type = SUBMENU;
    //_main_menu[menu_main].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_menu[menu_main].menu_item[j].disabled = false;
    _main_menu[menu_main].menu_item[j].hidden = false;
    _main_menu[menu_main].menu_item[j].selected = false;
    _main_menu[menu_main].menu_item[j].highlighted = false;
    strcpy(_main_menu[menu_main].menu_item[j].label, "Fun");
    _main_menu[menu_main].menu_item[j].type = DROPDOWN;
    _main_menu[menu_main].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    _main_menu[menu_main].menu_item[j].choices[0].value = 0;
    strcpy(_main_menu[menu_main].menu_item[j].choices[0].label, "Off");
    _main_menu[menu_main].menu_item[j].choices[1].value = 1;
    strcpy(_main_menu[menu_main].menu_item[j].choices[1].label, "On");
    _main_menu[menu_main].menu_item[j].choice_count = 2;
    j++;
    _main_menu[menu_main].menu_item[j].disabled = false;
    _main_menu[menu_main].menu_item[j].hidden = false;
    _main_menu[menu_main].menu_item[j].selected = false;
    _main_menu[menu_main].menu_item[j].highlighted = false;
    strcpy(_main_menu[menu_main].menu_item[j].label, "Fun Level");
    _main_menu[menu_main].menu_item[j].type = DROPDOWN;
    _main_menu[menu_main].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    _main_menu[menu_main].menu_item[j].choices[0].value = 0;
    strcpy(_main_menu[menu_main].menu_item[j].choices[0].label, "Low");
    _main_menu[menu_main].menu_item[j].choices[1].value = 1;
    strcpy(_main_menu[menu_main].menu_item[j].choices[1].label, "Medium");
    _main_menu[menu_main].menu_item[j].choices[2].value = 2;
    strcpy(_main_menu[menu_main].menu_item[j].choices[2].label, "High");
    _main_menu[menu_main].menu_item[j].choices[3].value = 3;
    strcpy(_main_menu[menu_main].menu_item[j].choices[3].label, "Crazy");
    _main_menu[menu_main].menu_item[j].choice_count = 4;
    j++;
    _main_menu[menu_main].menu_item[j].disabled = false;
    _main_menu[menu_main].menu_item[j].hidden = false;
    _main_menu[menu_main].menu_item[j].selected = false;
    _main_menu[menu_main].menu_item[j].highlighted = false;
    strcpy(_main_menu[menu_main].menu_item[j].label, "READONLY Item");
    _main_menu[menu_main].menu_item[j].type = READONLY;
    //_main_menu[menu_main].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_menu[menu_main].menu_item[j].disabled = false;
    _main_menu[menu_main].menu_item[j].hidden = false;
    _main_menu[menu_main].menu_item[j].selected = false;
    _main_menu[menu_main].menu_item[j].highlighted = false;
    strcpy(_main_menu[menu_main].menu_item[j].label, "READONLY Item");
    _main_menu[menu_main].menu_item[j].type = READONLY;
    //_main_menu[menu_main].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_menu[menu_main].menu_item[j].disabled = false;
    _main_menu[menu_main].menu_item[j].hidden = false;
    _main_menu[menu_main].menu_item[j].selected = false;
    _main_menu[menu_main].menu_item[j].highlighted = false;
    strcpy(_main_menu[menu_main].menu_item[j].label, "READONLY Item");
    _main_menu[menu_main].menu_item[j].type = READONLY;
    //_main_menu[menu_main].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_menu[menu_main].menu_item[j].disabled = false;
    _main_menu[menu_main].menu_item[j].hidden = false;
    _main_menu[menu_main].menu_item[j].selected = false;
    _main_menu[menu_main].menu_item[j].highlighted = false;
    strcpy(_main_menu[menu_main].menu_item[j].label, "READONLY Item");
    _main_menu[menu_main].menu_item[j].type = READONLY;
    //_main_menu[menu_main].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;

    _main_menu[menu_main].menu_items_count = j + 1;
    _main_menu[menu_main].menu_selection_index = 0;

    j = 0;
    strcpy(_main_menu[MENU_SUBMENU1].label, "SUBMENU 1");
    _main_menu[MENU_SUBMENU1].menu_id = MENU_SUBMENU1;

    _main_menu[MENU_SUBMENU1].menu_item[j].disabled = false;
    _main_menu[MENU_SUBMENU1].menu_item[j].hidden = false;
    _main_menu[MENU_SUBMENU1].menu_item[j].selected = false;
    _main_menu[MENU_SUBMENU1].menu_item[j].highlighted = false;
    strcpy(_main_menu[MENU_SUBMENU1].menu_item[j].label, "< Up");
    _main_menu[MENU_SUBMENU1].menu_item[j].type = BACKLINK;
    _main_menu[MENU_SUBMENU1].menu_item[j].link_to_SUBMENU = menu_main;
    //_main_menu[MENU_SUBMENU1].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_menu[MENU_SUBMENU1].menu_item[j].disabled = false;
    _main_menu[MENU_SUBMENU1].menu_item[j].hidden = false;
    _main_menu[MENU_SUBMENU1].menu_item[j].selected = false;
    _main_menu[MENU_SUBMENU1].menu_item[j].highlighted = false;
    strcpy(_main_menu[MENU_SUBMENU1].menu_item[j].label, "SUBMENU 1 Item 1");
    _main_menu[MENU_SUBMENU1].menu_item[j].type = READONLY;
    //_main_menu[MENU_SUBMENU1].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_menu[MENU_SUBMENU1].menu_item[j].disabled = false;
    _main_menu[MENU_SUBMENU1].menu_item[j].hidden = false;
    _main_menu[MENU_SUBMENU1].menu_item[j].selected = false;
    _main_menu[MENU_SUBMENU1].menu_item[j].highlighted = false;
    strcpy(_main_menu[MENU_SUBMENU1].menu_item[j].label, "SUBMENU 1 Item 2");
    _main_menu[MENU_SUBMENU1].menu_item[j].type = READONLY;
    //_main_menu[MENU_SUBMENU1].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;

    _main_menu[MENU_SUBMENU1].menu_items_count = j + 1;
    _main_menu[MENU_SUBMENU1].menu_selection_index = 0;

    j = 0;
    strcpy(_main_menu[MENU_SUBMENU2].label, "SUBMENU 2");
    _main_menu[MENU_SUBMENU2].menu_id = MENU_SUBMENU2;

    _main_menu[MENU_SUBMENU2].menu_item[j].disabled = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].hidden = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].selected = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].highlighted = false;
    strcpy(_main_menu[MENU_SUBMENU2].menu_item[j].label, "< Up");
    _main_menu[MENU_SUBMENU2].menu_item[j].type = BACKLINK;
    _main_menu[MENU_SUBMENU2].menu_item[j].link_to_SUBMENU = menu_main;
    //_main_menu[MENU_SUBMENU1].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    _main_menu[MENU_SUBMENU2].menu_item[j].disabled = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].hidden = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].selected = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].highlighted = false;
    strcpy(_main_menu[MENU_SUBMENU2].menu_item[j].label, "SUBMENU 2 Item 1");
    _main_menu[MENU_SUBMENU2].menu_item[j].type = READONLY;
    j++;
    _main_menu[MENU_SUBMENU2].menu_item[j].disabled = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].hidden = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].selected = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].highlighted = false;
    strcpy(_main_menu[MENU_SUBMENU2].menu_item[j].label, "SUBMENU 2 Item 2");
    _main_menu[MENU_SUBMENU2].menu_item[j].type = READONLY;
    j++;
    _main_menu[MENU_SUBMENU2].menu_item[j].disabled = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].hidden = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].selected = false;
    _main_menu[MENU_SUBMENU2].menu_item[j].highlighted = false;
    strcpy(_main_menu[MENU_SUBMENU2].menu_item[j].label, "SUBMENU 2 Item 3");
    _main_menu[MENU_SUBMENU2].menu_item[j].type = READONLY;

    _main_menu[MENU_SUBMENU2].menu_items_count = j + 1;
    _main_menu[MENU_SUBMENU2].menu_selection_index = 0;

    _main_menu_count = 4; // "none" menu is first so always add 1 to number of menus


    //left side icons
    home_icon_highlighted = false;
    back_icon_highlighted = false;


    // init menu selection indexes
    _parameter_menu_active = 0;
    _parameter_selection_index = 0;
    _menu_offset = 0;

    strcpy(menu_breadcrumbs, "Menu");

    unselect_all_menu_items(_current_menu);
    _main_menu[menu_main].menu_selection_index = 0;
    _main_menu[menu_main].menu_item[_main_menu[menu_main].menu_selection_index].selected = true;
}

/**************************************************************************/
/*!
   @brief  Draw a menu where the parameter of a certain option is displayed.
 * This typically looks like a kind of drop-down menu with several options to choose from
 * 

    @param    x  location (attention: top right corner of menu_item that triggers the parameter menu)
    @param    y  location (attention: top right corner of menu_item that triggers the parameter menu)
    @param    items[] the struct containing the different options to display
    @param    currentvalueindex which option (index) from the struct is the current setting
 */

/**************************************************************************/
void draw_parameter_menu(uint16_t x, uint16_t y, uint8_t main_menu_index, uint8_t main_menu_item_index) {
    uint16_t yoffset_label_from_base = 5;

    //debug
    //fill_circle(x, y, 4, 0x03ff);

    // calculate the parameter menu height based on the number of choices plus a bit of border margin
    uint16_t height = 2 + 2 + _main_menu[main_menu_index].menu_item[main_menu_item_index].choice_count * 30;

    //calculate the maximum width of the provided text options so we can define the menu width
    uint16_t max_width = 0;
    uint8_t i;
    for (i = 0; i < _main_menu[main_menu_index].menu_item[main_menu_item_index].choice_count; i++) {
        uint16_t x1, y1, w1, h1;
        get_text_bounds(_main_menu[main_menu_index].menu_item[main_menu_item_index].choices[i].label, x, y, &x1, &y1, &w1, &h1, _FreeSans9pt7b);
        if (w1 > max_width) {
            max_width = w1;
        }
    }
    // add 25 pixels to the widest label as margin
    uint16_t width = max_width + 25;


    // x and y provided to draw_parameter_menu originally define the top right corner
    // now that we know the width of the menu shift the x coordinate towards the left edge
    x -= width;


    //don't draw over the LCD edge -> move the parameter menu
    int16_t edge = (y - 29 + height) - _height + 29;

    //if the menu would clip entire lines in the available height just shift it up as a whole
    if (edge > 4) {
        y -= edge - 3;
    }

    // todo:
    // handle the case that there are more parameter options  than fit in the height of the LCD (7)

    //debug
    //fill_circle(x, y, 4, 0xff30);

    // draw menu borders
    fill_rect(x, y - 28 - 1, width, height + 1, _menu_background_color);
    draw_rect(x + 1, y - 28 + 1, width - 2, height - 3, _menu_text_color);

    // draw DROPDOWN options
    for (i = 0; i < _main_menu[main_menu_index].menu_item[main_menu_item_index].choice_count; i++) {
        char draw_label[32];
        strcpy(draw_label, _main_menu[main_menu_index].menu_item[main_menu_item_index].choices[i].label);

        if (i == _parameter_selection_index) {
            if (btn_E1_pressed) {
                //highlight
                fill_rect(x + 2, y + height - 29 - 1 - (i + 1) * 30, width - 4, 29, _menu_hightlighted_item_color);
                draw_string(x + 12, y + height - 29 + yoffset_label_from_base - (i + 1) * 30, draw_label, _menu_selected_text_color, _menu_selected_text_color, _FreeSans9pt7b, ALIGN_LEFT, 0);
            } else {
                //selected
                fill_rect(x + 2, y + height - 29 - 1 - (i + 1) * 30, width - 4, 29, _menu_selected_item_color);
                draw_string(x + 12, y + height - 29 + yoffset_label_from_base - (i + 1) * 30, draw_label, _menu_selected_text_color, _menu_selected_text_color, _FreeSans9pt7b, ALIGN_LEFT, 0);
            }
        } else {
            //normal
            fill_rect(x + 2, y + height - 29 - 1 - (i + 1) * 30, width - 4, 29, _menu_item_color);
            draw_string(x + 12, y + height - 29 + yoffset_label_from_base - (i + 1) * 30, draw_label, _menu_text_color, _menu_text_color, _FreeSans9pt7b, ALIGN_LEFT, 0);
        }

        // add a circle icon at beginning of the line of the currently set option
        if (i == _main_menu[main_menu_index].menu_item[main_menu_item_index].value) {
            if (i == _parameter_selection_index) {
                fill_circle(x + 6, y + height - 29 + 6 + yoffset_label_from_base - (i + 1) * 30, 3, _menu_selected_text_color);
            } else {
                fill_circle(x + 6, y + height - 29 + 6 + yoffset_label_from_base - (i + 1) * 30, 3, _menu_text_color);
            }
        }
    }
}

void draw_menu() {
    //clear the screen
    fill_rect(0, 0, _width, _height, _menu_background_color);

    //draw header background
    fill_rect(0, _height - 28, _width, 28, _menu_item_color);

    //draw header bread crumbs 
    draw_string(5, _height - 22, menu_breadcrumbs, _menu_text_color, _menu_text_color, _FreeSans9pt7b, ALIGN_LEFT, 0);

    //two header separation lines
    draw_line(0, _height - 29, _width - 1, _height - 29, _menu_selected_item_color);
    draw_line(0, _height - 30, _width - 1, _height - 30, _menu_background_color);


    //Side Icons

    //Side Icons Background
    fill_rect(0, 0, 30, _height - 30, _menu_item_color);

    //Home Icon
    if (home_icon_highlighted) {
        drawRGBBitmap(0, 158, (uint16_t*) (home_icon_highlight.pixel_data), home_icon_highlight.width, home_icon_highlight.height);
    } else {
        drawRGBBitmap(0, 158, (uint16_t*) (home_icon.pixel_data), home_icon.width, home_icon.height);
    }

    // Back Icon
    if (back_icon_highlighted) {
        drawRGBBitmap(0, 108, (uint16_t*) (back_icon_highlight.pixel_data), back_icon_highlight.width, back_icon_highlight.height);
    } else {
        drawRGBBitmap(0, 108, (uint16_t*) (back_icon.pixel_data), back_icon.width, back_icon.height);
    }


    // draw menu items
    uint8_t a;
    for (a = 0; a < _main_menu_count; a++) {
        if (_main_menu[a].menu_id == _current_menu) {
            uint8_t i;

            // this is the index of the 7 menu items drawn on screen currently
            int8_t display_selection_index = _main_menu[a].menu_selection_index - _menu_offset;


            // the _menu_offset is added to the item index and defines which item is the first one shown on screen

            //scrolling up from the first item 
            if (display_selection_index < 0) {
                _menu_offset -= 1;
            }

            //scrolling down from the last item
            if (display_selection_index >= 7) {
                _menu_offset += 1;
            }

            // only up to 7 menu items fit on screen at once
            int menu_items_count = _main_menu[a].menu_items_count;
            menu_items_count = limit_range(menu_items_count, 0, 7);

            // draw up to 7 menu items
            for (i = 0; i < menu_items_count; i++) {
                draw_menu_item(30, (_height - 29 - 30) - i * 30, a, i + _menu_offset);
            }

            // draw scroll bar indicator only if there are 7 or more menu items 
            if (menu_items_count == 7) {
                draw_scroll_indicator(menu_items_count, _main_menu[a].menu_items_count);
            }

            // draw parameter menu
            if (_parameter_menu_active != 0) {
                //the drawing coordinates in this case are provided as top right corner of the menu item
                //that triggers the parameter menu
                //the width depends on the text length of the options
                //the y coordinate may be shifted up or down if the choices would end up off screen
                uint16_t offset = (_top - 30)-(_parameter_menu_active - _menu_offset)*30 - 2;

                if (menu_items_count == 7) {
                    // if there is a scrollbar
                    draw_parameter_menu(_right - 16, offset, a, _parameter_menu_active);
                } else {
                    //if there is no scrollbar 
                    draw_parameter_menu(_right, offset, a, _parameter_menu_active);
                }
            }
        }
    }
}

void unselect_all_menu_items(enum _menu_id_t current_menu) {
    uint8_t b;
    for (b = 0; b < _main_menu[current_menu].menu_items_count; b++) {
        _main_menu[current_menu].menu_item[b].selected = false;
    }
    //_main_menu[current_menu].menu_selection_index = 0;
}

void unhighlight_all_menu_items(enum _menu_id_t current_menu) {
    uint8_t b;
    for (b = 0; b < _main_menu[current_menu].menu_items_count; b++) {
        _main_menu[current_menu].menu_item[b].highlighted = false;
    }
}

void navigate_to_menu(enum _menu_id_t target_menu) {

    // menu transition animation
    if (target_menu == menu_main) {
        // main menu is perceived as top/root level on the left so we need to push current menu towards right
        start_framebuffer_transition(PUSH_RIGHT, 60);
    } else {
        // sub menus are perceived as lower hierarchy right of the main menu so we need to push current menu towards left
        start_framebuffer_transition(PUSH_LEFT, 60);
    }

    //navigate into SUBMENU
    _current_menu = target_menu;

    //highlight the first item in the menu
    unselect_all_menu_items(_current_menu);
    _main_menu[target_menu].menu_item[_main_menu[target_menu].menu_selection_index].selected = true;

    //update bread crumbs
    if (target_menu != menu_main) {
        strcpy(menu_breadcrumbs, "Menu > ");
        strcat(menu_breadcrumbs, _main_menu[target_menu].label);
    } else {
        strcpy(menu_breadcrumbs, "Menu");
    }

    if (_current_page != PAGE_NONE) {
        _current_page = PAGE_NONE;
    }
}

void main_menu_button_release_handler(ButtonID button_index) {
    if (button_index == P7) {
        home_icon_highlighted = false;

        _parameter_menu_active = false;

        //go back to home page
        navigate_to_page(PAGE_HOME, PUSH_LEFT);
    }
    if (button_index == P8) {
        back_icon_highlighted = false;
        if (_parameter_menu_active) {
            // if a parameter menu is shown close it
            _parameter_menu_active = false;
        } else if (_current_menu == menu_main) {
            // if we are in the main menu back means we go to the home page
            navigate_to_page(PAGE_HOME, PUSH_LEFT);
        } else {
            // if we are in any SUBMENU back means we go to the main menu
            navigate_to_menu(menu_main);
        }
    }
    if (button_index == E1) {
        unhighlight_all_menu_items(_current_menu);

        uint8_t a;
        for (a = 0; a < _main_menu_count; a++) {
            if (_main_menu[a].menu_id == _current_menu) {

                // if this menu item is disabled don't do anything
                if (_main_menu[a].menu_item[_main_menu[a].menu_selection_index].disabled) {
                    return;
                }

                // is the current item linking into a SUBMENU?
                if ((_main_menu[a].menu_item[_main_menu[a].menu_selection_index].type == SUBMENU) || (_main_menu[a].menu_item[_main_menu[a].menu_selection_index].type == BACKLINK)) {
                    // navigate into SUBMENU
                    navigate_to_menu(_main_menu[a].menu_item[_main_menu[a].menu_selection_index].link_to_SUBMENU);
                    return;
                }

                // is the current item linking to a page
                if (_main_menu[a].menu_item[_main_menu[a].menu_selection_index].type == PAGELINK) {
                    // navigate to page
                    navigate_to_page(_main_menu[a].menu_item[_main_menu[a].menu_selection_index].link_to_page, PUSH_LEFT);
                    return;
                }

                // is the current item supposed to show a drop-down menu?
                if ((_main_menu[a].menu_item[_main_menu[a].menu_selection_index].type == DROPDOWN) && (_parameter_menu_active == 0)) {
                    //open parameter menu
                    _parameter_menu_active = _main_menu[a].menu_selection_index;
                    return;
                }

                // are we in a drop-down menu currently?
                if ((_main_menu[a].menu_item[_main_menu[a].menu_selection_index].type == DROPDOWN) && (_parameter_menu_active != 0)) {
                    // set new value
                    _main_menu[a].menu_item[_main_menu[a].menu_selection_index].value = _parameter_selection_index;

                    //close parameter menu
                    _parameter_menu_active = 0;
                }
            }
        }
    }
}

void main_menu_button_press_handler(ButtonID button_index) {
    if (button_index == P7) {
        home_icon_highlighted = true;
    }
    if (button_index == P8) {
        back_icon_highlighted = true;
    }
    if (button_index == E1) {
        if (_parameter_menu_active) {
            // a parameter menu is currently active

        } else {
            // we are in a normal menu structure
            unhighlight_all_menu_items(_current_menu);
            uint8_t a;
            for (a = 0; a < _main_menu_count; a++) {
                if (_main_menu[a].menu_id == _current_menu) {
                    _main_menu[a].menu_item[_main_menu[a].menu_selection_index].highlighted = true;
                }
            }
        }
    }
}

void main_menu_knob_handler(ButtonID button_index, int8_t diff) {
    if (button_index == E1_ROT) {
        // are we inside a parameter menu?
        if (_parameter_menu_active) {
            _parameter_selection_index += diff;
            _parameter_selection_index = limit_range(_parameter_selection_index, 0, get_current_item_choice_count() - 1);

        } else {
            // we are inside the normal menu structure
            uint8_t a;
            for (a = 0; a < _main_menu_count; a++) {
                if (_main_menu[a].menu_id == _current_menu) {
                    _main_menu[a].menu_selection_index += diff;

                    _main_menu[a].menu_selection_index = limit_range(_main_menu[a].menu_selection_index, 0,
                            _main_menu[a].menu_items_count - 1);

                    unhighlight_all_menu_items(_current_menu);
                    unselect_all_menu_items(_current_menu);
                    _main_menu[a].menu_item[_main_menu[a].menu_selection_index].selected = true;
                }
            }
        }
    }
}

#endif /* MENU_C */
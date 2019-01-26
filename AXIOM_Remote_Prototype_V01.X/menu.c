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

#include "menu.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "draw.h"
#include "globals.h"
#include "draw.h"
#include "utility.h"

#include "media/media.h"

//static uint8_t menu_offset = 0; // when scrolling the menu this is the offset for the items
//static uint8_t parameter_menu_active; // is a parameter menu currently visible (0 = no)
//static uint8_t parameter_selection_index; // index of the item currently selected in a parameter menu
static menu_t main_menu[5];
//uint8_t main_menu_count;
static char menu_breadcrumbs[64];

//page_t _side_icons;
bool home_icon_highlighted;
bool back_icon_highlighted;


// Color Definitions

static uint16_t menu_item_color;
static uint16_t menu_dimmed_item_color;
static uint16_t menu_disabled_item_color;
static uint16_t menu_selected_item_color;
static uint16_t menu_hightlighted_item_color;
static uint16_t menu_background_color;
static uint16_t menu_text_color;
static uint16_t menu_disabled_text_color;
static uint16_t menu_selected_text_color;

uint8_t get_current_menu_item_count() {
    uint8_t a;
    for (a = 0; a < main_menu_count; a++) {
        if (main_menu[a].menu_id == current_menu) {
            return main_menu[a].menu_items_count;
        }
    }
    return 0;
}

uint8_t get_current_item_choice_count() {
    uint8_t a;
    for (a = 0; a < main_menu_count; a++) {
        if (main_menu[a].menu_id == current_menu) {
            return main_menu[a].menu_item[main_menu[a].menu_selection_index].choice_count;
        }
    }
    return 0;
}

char * menu_item_test_get_current_value(uint8_t menu, uint8_t menu_selection_index) {
    return main_menu[menu].menu_item[menu_selection_index].choices[main_menu[menu].menu_item[menu_selection_index].value].label;
}

void draw_menu_item(uint16_t x, uint16_t y, uint8_t menu_index, uint8_t menu_main_item_index) {
    //void draw_menu_item(uint16_t x, uint16_t y, MENU_ITEM_TYPE_t MENU_MAIN_item, bool selected, bool highlighted) {
    uint16_t yoffset_label_from_base = 7;


    // don't draw empty items
    if (main_menu[menu_index].menu_item[menu_main_item_index].label == NULL) {
        return;
    }

    char value[16];

    if (main_menu[menu_index].menu_item[menu_main_item_index].type == MENU_ITEM_TYPE_SUBMENU) {
        // submenu items are indicated by a ">" sign
        sprintf(value, ">");
    } else if (main_menu[menu_index].menu_item[menu_main_item_index].type == MENU_ITEM_TYPE_PAGELINK) {
        // page links have no value icon displayed 
        sprintf(value, "");
    } else if (main_menu[menu_index].menu_item[menu_main_item_index].type == MENU_ITEM_TYPE_BACKLINK) {
        // backlinks point to a menu higher in the menu hierarchy
        sprintf(value, "");
    } else {
        if (main_menu[menu_index].menu_item[menu_main_item_index].current_value_ptr != NULL) {
            //strcpy(value, (*main_menu[menu_index].menu_item[menu_main_item_index].current_value_ptr)(menu_index, menu_main_item_index));


            //sprintf(value, "%s", (*main_menu[menu_index].menu_item[menu_main_item_index].current_value_ptr)(menu_index, menu_main_item_index));
        } else {
            sprintf(value, "%d", main_menu[menu_index].menu_item[menu_main_item_index].value);
        }
    }

    // is the current line highlighted and not disabled?
    if (main_menu[menu_index].menu_item[menu_main_item_index].highlighted && !main_menu[menu_index].menu_item[menu_main_item_index].disabled) {
        fill_rect(x, y, FRAMEBUFFER_WIDTH - x, 29, menu_hightlighted_item_color);

        //label
        draw_string(x + 5, y + yoffset_label_from_base, main_menu[menu_index].menu_item[menu_main_item_index].label,
                menu_selected_text_color, menu_selected_text_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);

        //value
        draw_string(x + 180, y + yoffset_label_from_base, value, menu_selected_text_color,
                menu_selected_text_color, _FreeSans9pt7b, TEXT_ALIGN_RIGHT, 80);
        return;
    }

    // is a parameter menu active currently and the item is disabled?
    if (parameter_menu_active && main_menu[menu_index].menu_item[menu_main_item_index].disabled) {
        fill_rect(x, y, FRAMEBUFFER_WIDTH - x, 29, menu_dimmed_item_color);

        //label
        draw_string(x + 5, y + yoffset_label_from_base, main_menu[menu_index].menu_item[menu_main_item_index].label,
                menu_disabled_text_color, menu_disabled_text_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);

        //value
        draw_string(x + 180, y + yoffset_label_from_base, value, menu_disabled_text_color, menu_disabled_text_color,
                _FreeSans9pt7b, TEXT_ALIGN_RIGHT, 80);
        return;
    }

    // is a parameter menu active currently?
    if (parameter_menu_active) {
        fill_rect(x, y, FRAMEBUFFER_WIDTH - x, 29, menu_dimmed_item_color);

        //label
        draw_string(x + 5, y + yoffset_label_from_base, main_menu[menu_index].menu_item[menu_main_item_index].label,
                menu_text_color, menu_text_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);

        //value
        draw_string(x + 180, y + yoffset_label_from_base, value, menu_text_color, menu_text_color,
                _FreeSans9pt7b, TEXT_ALIGN_RIGHT, 80);
        return;
    }

    // is the current line selected and disabled?
    if (main_menu[menu_index].menu_item[menu_main_item_index].selected && main_menu[menu_index].menu_item[menu_main_item_index].disabled) {
        fill_rect(x, y, FRAMEBUFFER_WIDTH - x, 29, menu_disabled_item_color);

        //disabled indicators on the side
        fill_rect(30, y, 4, 29, menu_selected_text_color);
        fill_rect(FRAMEBUFFER_WIDTH - 16 - 4, y, 4, 29, menu_selected_text_color);

        //label
        draw_string(x + 5, y + yoffset_label_from_base, main_menu[menu_index].menu_item[menu_main_item_index].label,
                menu_disabled_text_color, menu_disabled_text_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);

        //value
        draw_string(x + 180, y + yoffset_label_from_base, value, menu_disabled_text_color, menu_disabled_text_color,
                _FreeSans9pt7b, TEXT_ALIGN_RIGHT, 80);
        return;
    }

    // is the current line disabled?
    if (main_menu[menu_index].menu_item[menu_main_item_index].disabled) {
        fill_rect(x, y, FRAMEBUFFER_WIDTH - x, 29, menu_disabled_text_color);

        //label
        draw_string(x + 5, y + yoffset_label_from_base, main_menu[menu_index].menu_item[menu_main_item_index].label,
                menu_disabled_text_color, menu_disabled_text_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);

        //value
        draw_string(x + 180, y + yoffset_label_from_base, value, menu_disabled_text_color, menu_disabled_text_color,
                _FreeSans9pt7b, TEXT_ALIGN_RIGHT, 80);
        return;
    }

    // is the current line selected (cursor)?
    if (main_menu[menu_index].menu_item[menu_main_item_index].selected) {
        fill_rect(x, y, FRAMEBUFFER_WIDTH - x, 29, menu_selected_text_color);

        //value
        draw_string(x + 5, y + yoffset_label_from_base, main_menu[menu_index].menu_item[menu_main_item_index].label,
                menu_selected_text_color, menu_selected_text_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);

        //label
        draw_string(x + 180, y + yoffset_label_from_base, value, menu_selected_text_color, menu_selected_text_color,
                _FreeSans9pt7b, TEXT_ALIGN_RIGHT, 80);
        return;
    }

    //if nothing of the above applies simply draw the line item normally
    fill_rect(x, y, FRAMEBUFFER_WIDTH - x, 29, menu_item_color);

    //label
    draw_string(x + 5, y + yoffset_label_from_base, main_menu[menu_index].menu_item[menu_main_item_index].label,
            menu_text_color, menu_text_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);

    //value
    draw_string(x + 180, y + yoffset_label_from_base, value, menu_text_color, menu_text_color,
            _FreeSans9pt7b, TEXT_ALIGN_RIGHT, 80);
}

void draw_scroll_indicator(uint8_t current_menu_item_screen_count, uint8_t current_menu_item_count) {
    // maximum height is the screen without header area
    uint8_t srollbar_max_framebuffer_height = FRAMEBUFFER_HEIGHT - 30;

    // height of the scroll indicator is defined by the ratio of number of items on screen vs total number of item. 
    // if there are 7 items on screen of total 14 items the scroll indicator shall be 50% of the scrollbar height
    uint8_t scrollbarheight = srollbar_max_framebuffer_height * (float) ((float) current_menu_item_screen_count /
            (float) (current_menu_item_count));

    //
    uint8_t scrollbaroffset = ((current_menu_item_count - current_menu_item_screen_count) - menu_offset) *
            ((srollbar_max_framebuffer_height - scrollbarheight) / (current_menu_item_count - current_menu_item_screen_count));

    //Background
    fill_rect(FRAMEBUFFER_WIDTH - 16, 0, 16, FRAMEBUFFER_HEIGHT - 30, menu_item_color);

    //Thin Line
    fill_rect(FRAMEBUFFER_WIDTH - 9, 0, 4, FRAMEBUFFER_HEIGHT - 30, menu_text_color);

    //Thick Line
    fill_rect(FRAMEBUFFER_WIDTH - 13, scrollbaroffset, 12, scrollbarheight, menu_text_color);
}

/**************************************************************************/
/*!
   @brief  initialize our menu structure with default values and definitions
 * 
 */

/**************************************************************************/
void init_menus() {
    //Color Theme definitions
    menu_item_color = color565(255, 255, 255);
    menu_dimmed_item_color = color565(247, 251, 247);
    menu_disabled_text_color = color565(255, 255, 255);
    menu_selected_text_color = color565(255, 128, 0);
    menu_background_color = color565(180, 180, 180);
    menu_text_color = color565(0, 0, 0);
    menu_selected_text_color = color565(255, 255, 255);
    menu_hightlighted_item_color = color565(0, 128, 255);
    menu_disabled_text_color = color565(230, 230, 230);

    uint8_t j = 0;
    strcpy(main_menu[MENU_MAIN].label, "Main Menu");
    main_menu[MENU_MAIN].menu_id = MENU_MAIN;

    main_menu[MENU_MAIN].menu_item[j].disabled = false;
    main_menu[MENU_MAIN].menu_item[j].hidden = false;
    main_menu[MENU_MAIN].menu_item[j].selected = false;
    main_menu[MENU_MAIN].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_MAIN].menu_item[j].label, "Exit Menu");
    main_menu[MENU_MAIN].menu_item[j].type = MENU_ITEM_TYPE_PAGELINK;
    main_menu[MENU_MAIN].menu_item[j].link_to_page = PAGE_HOME;
    //main_menu[MENU_MAIN].menu_item[j].current_value_ptr = &MENU_ITEM_TYPE_test_get_current_value;
    j++;
    main_menu[MENU_MAIN].menu_item[j].disabled = false;
    main_menu[MENU_MAIN].menu_item[j].hidden = false;
    main_menu[MENU_MAIN].menu_item[j].selected = false;
    main_menu[MENU_MAIN].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_MAIN].menu_item[j].label, "Submenu 1");
    main_menu[MENU_MAIN].menu_item[j].type = MENU_ITEM_TYPE_SUBMENU;
    main_menu[MENU_MAIN].menu_item[j].link_to_submenu = MENU_SUBMENU1;
    //main_menu[MENU_MAIN].menu_item[j].current_value_ptr = &MENU_ITEM_TYPE_test_get_current_value;
    j++;
    main_menu[MENU_MAIN].menu_item[j].disabled = false;
    main_menu[MENU_MAIN].menu_item[j].hidden = false;
    main_menu[MENU_MAIN].menu_item[j].selected = false;
    main_menu[MENU_MAIN].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_MAIN].menu_item[j].label, "Submenu 2");
    main_menu[MENU_MAIN].menu_item[j].type = MENU_ITEM_TYPE_SUBMENU;
    main_menu[MENU_MAIN].menu_item[j].link_to_submenu = MENU_SUBMENU2;
    //main_menu[MENU_MAIN].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    main_menu[MENU_MAIN].menu_item[j].disabled = true;
    main_menu[MENU_MAIN].menu_item[j].hidden = false;
    main_menu[MENU_MAIN].menu_item[j].selected = false;
    main_menu[MENU_MAIN].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_MAIN].menu_item[j].label, "Disabled Item");
    main_menu[MENU_MAIN].menu_item[j].type = MENU_ITEM_TYPE_SUBMENU;
    //main_menu[MENU_MAIN].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    main_menu[MENU_MAIN].menu_item[j].disabled = false;
    main_menu[MENU_MAIN].menu_item[j].hidden = false;
    main_menu[MENU_MAIN].menu_item[j].selected = false;
    main_menu[MENU_MAIN].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_MAIN].menu_item[j].label, "Fun");
    main_menu[MENU_MAIN].menu_item[j].type = MENU_ITEM_TYPE_DROPDOWN;
    main_menu[MENU_MAIN].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    main_menu[MENU_MAIN].menu_item[j].choices[0].value = 0;
    strcpy(main_menu[MENU_MAIN].menu_item[j].choices[0].label, "Off");
    main_menu[MENU_MAIN].menu_item[j].choices[1].value = 1;
    strcpy(main_menu[MENU_MAIN].menu_item[j].choices[1].label, "On");
    main_menu[MENU_MAIN].menu_item[j].choice_count = 2;
    j++;
    main_menu[MENU_MAIN].menu_item[j].disabled = false;
    main_menu[MENU_MAIN].menu_item[j].hidden = false;
    main_menu[MENU_MAIN].menu_item[j].selected = false;
    main_menu[MENU_MAIN].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_MAIN].menu_item[j].label, "Fun Level");
    main_menu[MENU_MAIN].menu_item[j].type = MENU_ITEM_TYPE_DROPDOWN;
    main_menu[MENU_MAIN].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    main_menu[MENU_MAIN].menu_item[j].choices[0].value = 0;
    strcpy(main_menu[MENU_MAIN].menu_item[j].choices[0].label, "Low");
    main_menu[MENU_MAIN].menu_item[j].choices[1].value = 1;
    strcpy(main_menu[MENU_MAIN].menu_item[j].choices[1].label, "Medium");
    main_menu[MENU_MAIN].menu_item[j].choices[2].value = 2;
    strcpy(main_menu[MENU_MAIN].menu_item[j].choices[2].label, "High");
    main_menu[MENU_MAIN].menu_item[j].choices[3].value = 3;
    strcpy(main_menu[MENU_MAIN].menu_item[j].choices[3].label, "Crazy");
    main_menu[MENU_MAIN].menu_item[j].choice_count = 4;
    j++;
    main_menu[MENU_MAIN].menu_item[j].disabled = false;
    main_menu[MENU_MAIN].menu_item[j].hidden = false;
    main_menu[MENU_MAIN].menu_item[j].selected = false;
    main_menu[MENU_MAIN].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_MAIN].menu_item[j].label, "Readonly Item");
    main_menu[MENU_MAIN].menu_item[j].type = MENU_ITEM_TYPE_READONLY;
    //main_menu[MENU_MAIN].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    main_menu[MENU_MAIN].menu_item[j].disabled = false;
    main_menu[MENU_MAIN].menu_item[j].hidden = false;
    main_menu[MENU_MAIN].menu_item[j].selected = false;
    main_menu[MENU_MAIN].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_MAIN].menu_item[j].label, "Readonly Item");
    main_menu[MENU_MAIN].menu_item[j].type = MENU_ITEM_TYPE_READONLY;
    //main_menu[MENU_MAIN].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    main_menu[MENU_MAIN].menu_item[j].disabled = false;
    main_menu[MENU_MAIN].menu_item[j].hidden = false;
    main_menu[MENU_MAIN].menu_item[j].selected = false;
    main_menu[MENU_MAIN].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_MAIN].menu_item[j].label, "Readonly Item");
    main_menu[MENU_MAIN].menu_item[j].type = MENU_ITEM_TYPE_READONLY;
    //main_menu[MENU_MAIN].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    j++;
    main_menu[MENU_MAIN].menu_item[j].disabled = false;
    main_menu[MENU_MAIN].menu_item[j].hidden = false;
    main_menu[MENU_MAIN].menu_item[j].selected = false;
    main_menu[MENU_MAIN].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_MAIN].menu_item[j].label, "Readonly Item");
    main_menu[MENU_MAIN].menu_item[j].type = MENU_ITEM_TYPE_READONLY;
    //main_menu[MENU_MAIN].menu_item[j].current_value_ptr = &menu_item_test_get_current_value;
    main_menu[MENU_MAIN].menu_items_count = j + 1;
    main_menu[MENU_MAIN].menu_selection_index = 0;

    j = 0;
    strcpy(main_menu[MENU_SUBMENU1].label, "Submenu 1");
    main_menu[MENU_SUBMENU1].menu_id = MENU_SUBMENU1;
    main_menu[MENU_SUBMENU1].menu_item[j].disabled = false;
    main_menu[MENU_SUBMENU1].menu_item[j].hidden = false;
    main_menu[MENU_SUBMENU1].menu_item[j].selected = false;
    main_menu[MENU_SUBMENU1].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_SUBMENU1].menu_item[j].label, "< Up");
    main_menu[MENU_SUBMENU1].menu_item[j].type = MENU_ITEM_TYPE_BACKLINK;
    main_menu[MENU_SUBMENU1].menu_item[j].link_to_submenu = MENU_MAIN;
    //main_menu[MENU_SUBMENU1].menu_item[j].current_value_ptr = &MENU_ITEM_TYPE_test_get_current_value;
    j++;
    main_menu[MENU_SUBMENU1].menu_item[j].disabled = false;
    main_menu[MENU_SUBMENU1].menu_item[j].hidden = false;
    main_menu[MENU_SUBMENU1].menu_item[j].selected = false;
    main_menu[MENU_SUBMENU1].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_SUBMENU1].menu_item[j].label, "Submenu 1 Item 1");
    main_menu[MENU_SUBMENU1].menu_item[j].type = MENU_ITEM_TYPE_READONLY;
    //main_menu[MENU_SUBMENU1].menu_item[j].current_value_ptr = &MENU_ITEM_TYPE_test_get_current_value;
    j++;
    main_menu[MENU_SUBMENU1].menu_item[j].disabled = false;
    main_menu[MENU_SUBMENU1].menu_item[j].hidden = false;
    main_menu[MENU_SUBMENU1].menu_item[j].selected = false;
    main_menu[MENU_SUBMENU1].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_SUBMENU1].menu_item[j].label, "Submenu 1 Item 2");
    main_menu[MENU_SUBMENU1].menu_item[j].type = MENU_ITEM_TYPE_READONLY;
    //main_menu[MENU_SUBMENU1].menu_item[j].current_value_ptr = &MENU_ITEM_TYPE_test_get_current_value;

    main_menu[MENU_SUBMENU1].menu_items_count = j + 1;
    main_menu[MENU_SUBMENU1].menu_selection_index = 0;

    j = 0;
    strcpy(main_menu[MENU_SUBMENU2].label, "Submenu 2");
    main_menu[MENU_SUBMENU2].menu_id = MENU_SUBMENU2;
    main_menu[MENU_SUBMENU2].menu_item[j].disabled = false;
    main_menu[MENU_SUBMENU2].menu_item[j].hidden = false;
    main_menu[MENU_SUBMENU2].menu_item[j].selected = false;
    main_menu[MENU_SUBMENU2].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_SUBMENU2].menu_item[j].label, "< Up");
    main_menu[MENU_SUBMENU2].menu_item[j].type = MENU_ITEM_TYPE_BACKLINK;
    main_menu[MENU_SUBMENU2].menu_item[j].link_to_submenu = MENU_MAIN;
    //main_menu[MENU_SUBMENU1].menu_item[j].current_value_ptr = &MENU_ITEM_TYPE_test_get_current_value;
    j++;
    main_menu[MENU_SUBMENU2].menu_item[j].disabled = false;
    main_menu[MENU_SUBMENU2].menu_item[j].hidden = false;
    main_menu[MENU_SUBMENU2].menu_item[j].selected = false;
    main_menu[MENU_SUBMENU2].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_SUBMENU2].menu_item[j].label, "Submenu 2 Item 1");
    main_menu[MENU_SUBMENU2].menu_item[j].type = MENU_ITEM_TYPE_READONLY;
    j++;
    main_menu[MENU_SUBMENU2].menu_item[j].disabled = false;
    main_menu[MENU_SUBMENU2].menu_item[j].hidden = false;
    main_menu[MENU_SUBMENU2].menu_item[j].selected = false;
    main_menu[MENU_SUBMENU2].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_SUBMENU2].menu_item[j].label, "Submenu 2 Item 2");
    main_menu[MENU_SUBMENU2].menu_item[j].type = MENU_ITEM_TYPE_READONLY;
    j++;
    main_menu[MENU_SUBMENU2].menu_item[j].disabled = false;
    main_menu[MENU_SUBMENU2].menu_item[j].hidden = false;
    main_menu[MENU_SUBMENU2].menu_item[j].selected = false;
    main_menu[MENU_SUBMENU2].menu_item[j].highlighted = false;
    strcpy(main_menu[MENU_SUBMENU2].menu_item[j].label, "Submenu 2 Item 3");
    main_menu[MENU_SUBMENU2].menu_item[j].type = MENU_ITEM_TYPE_READONLY;

    main_menu[MENU_SUBMENU2].menu_items_count = j + 1;
    main_menu[MENU_SUBMENU2].menu_selection_index = 0;

    main_menu_count = 4; // "none" menu is first so always add 1 to number of menus


    //left side icons
    home_icon_highlighted = false;
    back_icon_highlighted = false;


    // init menu selection indexes
    parameter_menu_active = 0;
    parameter_selection_index = 0;
    menu_offset = 0;

    strcpy(menu_breadcrumbs, "Menu");

    unselect_all_menu_items(current_menu);
    main_menu[MENU_MAIN].menu_selection_index = 0;
    main_menu[MENU_MAIN].menu_item[main_menu[MENU_MAIN].menu_selection_index].selected = true;
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
    uint16_t height = 2 + 2 + main_menu[main_menu_index].menu_item[main_menu_item_index].choice_count * 30;

    //calculate the maximum width of the provided text options so we can define the menu width
    uint16_t max_width = 0;
    uint8_t i;
    for (i = 0; i < main_menu[main_menu_index].menu_item[main_menu_item_index].choice_count; i++) {
        uint16_t x1, y1, w1, h1;
        get_text_bounds(main_menu[main_menu_index].menu_item[main_menu_item_index].choices[i].label, x, y, &x1, &y1, &w1, &h1, _FreeSans9pt7b);
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
    int16_t edge = (y - 29 + height) - FRAMEBUFFER_HEIGHT + 29;

    //if the menu would clip entire lines in the available height just shift it up as a whole
    if (edge > 4) {
        y -= edge - 3;
    }

    // todo:
    // handle the case that there are more parameter options  than fit in the height of the LCD (7)

    //debug
    //fill_circle(x, y, 4, 0xff30);

    // draw menu borders
    fill_rect(x, y - 28 - 1, width, height + 1, menu_background_color);
    draw_rect(x + 1, y - 28 + 1, width - 2, height - 3, menu_text_color);

    // draw DROPDOWN options
    for (i = 0; i < main_menu[main_menu_index].menu_item[main_menu_item_index].choice_count; i++) {
        char draw_label[32];
        strcpy(draw_label, main_menu[main_menu_index].menu_item[main_menu_item_index].choices[i].label);

        if (i == parameter_selection_index) {
            if (btn_E1_pressed) {
                //highlight
                fill_rect(x + 2, y + height - 29 - 1 - (i + 1) * 30, width - 4, 29, menu_hightlighted_item_color);
                draw_string(x + 12, y + height - 29 + yoffset_label_from_base - (i + 1) * 30, draw_label, menu_selected_text_color, menu_selected_text_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);
            } else {
                //selected
                fill_rect(x + 2, y + height - 29 - 1 - (i + 1) * 30, width - 4, 29, menu_selected_item_color);
                draw_string(x + 12, y + height - 29 + yoffset_label_from_base - (i + 1) * 30, draw_label, menu_selected_text_color, menu_selected_text_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);
            }
        } else {
            //normal
            fill_rect(x + 2, y + height - 29 - 1 - (i + 1) * 30, width - 4, 29, menu_item_color);
            draw_string(x + 12, y + height - 29 + yoffset_label_from_base - (i + 1) * 30, draw_label, menu_text_color, menu_text_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);
        }

        // add a circle icon at beginning of the line of the currently set option
        if (i == main_menu[main_menu_index].menu_item[main_menu_item_index].value) {
            if (i == parameter_selection_index) {
                fill_circle(x + 6, y + height - 29 + 6 + yoffset_label_from_base - (i + 1) * 30, 3, menu_selected_text_color);
            } else {
                fill_circle(x + 6, y + height - 29 + 6 + yoffset_label_from_base - (i + 1) * 30, 3, menu_text_color);
            }
        }
    }
}

void draw_menu() {
    //clear the screen
    fill_rect(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, menu_background_color);

    //draw header background
    fill_rect(0, FRAMEBUFFER_HEIGHT - 28, FRAMEBUFFER_WIDTH, 28, menu_item_color);

    //draw header bread crumbs 
    draw_string(5, FRAMEBUFFER_HEIGHT - 22, menu_breadcrumbs, menu_text_color, menu_text_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);

    //two header separation lines
    draw_line(0, FRAMEBUFFER_HEIGHT - 29, FRAMEBUFFER_WIDTH - 1, FRAMEBUFFER_HEIGHT - 29, menu_selected_text_color);

    draw_line(0, FRAMEBUFFER_HEIGHT - 30, FRAMEBUFFER_WIDTH - 1, FRAMEBUFFER_HEIGHT - 30, menu_background_color);


    //Side Icons

    //Side Icons Background
    fill_rect(0, 0, 30, FRAMEBUFFER_HEIGHT - 30, menu_item_color);

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
    for (a = 0; a < main_menu_count; a++) {
        if (main_menu[a].menu_id == current_menu) {
            uint8_t i;

            // this is the index of the 7 menu items drawn on screen currently
            int8_t display_selection_index = main_menu[a].menu_selection_index - menu_offset;


            // the _menu_offset is added to the item index and defines which item is the first one shown on screen

            //scrolling up from the first item 
            if (display_selection_index < 0) {
                menu_offset -= 1;
            }

            //scrolling down from the last item
            if (display_selection_index >= 7) {
                menu_offset += 1;
            }

            // only up to 7 menu items fit on screen at once
            int menu_items_count = main_menu[a].menu_items_count;
            menu_items_count = limit_range(menu_items_count, 0, 7);

            // draw up to 7 menu items
            for (i = 0; i < menu_items_count; i++) {
                draw_menu_item(30, (FRAMEBUFFER_HEIGHT - 29 - 30) - i * 30, a, i + menu_offset);
            }

            // draw scroll bar indicator only if there are 7 or more menu items 
            if (menu_items_count == 7) {
                draw_scroll_indicator(menu_items_count, main_menu[a].menu_items_count);
            }

            // draw parameter menu
            if (parameter_menu_active != 0) {
                //the drawing coordinates in this case are provided as top right corner of the menu item
                //that triggers the parameter menu
                //the width depends on the text length of the options
                //the y coordinate may be shifted up or down if the choices would end up off screen
                uint16_t offset = (FRAMEBUFFER_TOP - 30)-(parameter_menu_active - menu_offset)*30 - 2;

                if (menu_items_count == 7) {
                    // if there is a scrollbar
                    draw_parameter_menu(FRAMEBUFFER_RIGHT - 16, offset, a, parameter_menu_active);
                } else {
                    //if there is no scrollbar 
                    draw_parameter_menu(FRAMEBUFFER_RIGHT, offset, a, parameter_menu_active);
                }
            }
        }
    }
}

void unselect_all_menu_items(enum menu_id_t current_menu) {
    uint8_t b;
    for (b = 0; b < main_menu[current_menu].menu_items_count; b++) {
        main_menu[current_menu].menu_item[b].selected = false;
    }
    //_main_menu[current_menu].menu_selection_index = 0;
}

void unhighlight_all_menu_items(enum menu_id_t current_menu) {
    uint8_t b;
    for (b = 0; b < main_menu[current_menu].menu_items_count; b++) {
        main_menu[current_menu].menu_item[b].highlighted = false;
    }
}

void navigate_to_menu(enum menu_id_t target_menu) {

    // menu transition animation
    if (target_menu == MENU_MAIN) {
        // main menu is perceived as top/root level on the left so we need to push current menu towards right
        start_framebuffer_transition(TRANSITION_PUSH_RIGHT, 60);
    } else {
        // sub menus are perceived as lower hierarchy right of the main menu so we need to push current menu towards left
        start_framebuffer_transition(TRANSITION_PUSH_LEFT, 60);
    }

    //navigate into submenu
    current_menu = target_menu;

    //highlight the first item in the menu
    unselect_all_menu_items(current_menu);
    main_menu[target_menu].menu_item[main_menu[target_menu].menu_selection_index].selected = true;

    //update bread crumbs
    if (target_menu != MENU_MAIN) {
        strcpy(menu_breadcrumbs, "Menu > ");
        strcat(menu_breadcrumbs, main_menu[target_menu].label);
    } else {
        strcpy(menu_breadcrumbs, "Menu");
    }

    if (current_page != PAGE_NONE) {
        current_page = PAGE_NONE;
    }
}

void main_menu_button_release_handler(ButtonID button_index) {
    if (button_index == P7) {
        home_icon_highlighted = false;

        parameter_menu_active = false;

        //go back to home page
        navigate_to_page(PAGE_HOME, TRANSITION_PUSH_LEFT);
    }
    if (button_index == P8) {
        back_icon_highlighted = false;
        if (parameter_menu_active) {
            // if a parameter menu is shown close it
            parameter_menu_active = false;
        } else if (current_menu == MENU_MAIN) {
            // if we are in the main menu back means we go to the home page
            navigate_to_page(PAGE_HOME, TRANSITION_PUSH_LEFT);
        } else {
            // if we are in any submenu back means we go to the main menu
            navigate_to_menu(MENU_MAIN);
        }
    }
    if (button_index == E1) {
        unhighlight_all_menu_items(current_menu);

        uint8_t a;
        for (a = 0; a < main_menu_count; a++) {
            if (main_menu[a].menu_id == current_menu) {

                // if this menu item is disabled don't do anything
                if (main_menu[a].menu_item[main_menu[a].menu_selection_index].disabled) {
                    return;
                }

                // is the current item linking into a submenu?
                if ((main_menu[a].menu_item[main_menu[a].menu_selection_index].type == MENU_ITEM_TYPE_SUBMENU) || (main_menu[a].menu_item[main_menu[a].menu_selection_index].type == MENU_ITEM_TYPE_BACKLINK)) {
                    // navigate into submenu
                    navigate_to_menu(main_menu[a].menu_item[main_menu[a].menu_selection_index].link_to_submenu);
                    return;
                }

                // is the current item linking to a page
                if (main_menu[a].menu_item[main_menu[a].menu_selection_index].type == MENU_ITEM_TYPE_PAGELINK) {
                    // navigate to page
                    navigate_to_page(main_menu[a].menu_item[main_menu[a].menu_selection_index].link_to_page, TRANSITION_PUSH_LEFT);
                    return;
                }

                // is the current item supposed to show a drop-down menu?
                if ((main_menu[a].menu_item[main_menu[a].menu_selection_index].type == MENU_ITEM_TYPE_DROPDOWN) && (parameter_menu_active == 0)) {
                    //open parameter menu
                    parameter_menu_active = main_menu[a].menu_selection_index;
                    return;
                }

                // are we in a drop-down menu currently?
                if ((main_menu[a].menu_item[main_menu[a].menu_selection_index].type == MENU_ITEM_TYPE_DROPDOWN) && (parameter_menu_active != 0)) {
                    // set new value
                    main_menu[a].menu_item[main_menu[a].menu_selection_index].value = parameter_selection_index;

                    //close parameter menu
                    parameter_menu_active = 0;
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
        if (parameter_menu_active) {
            // a parameter menu is currently active

        } else {
            // we are in a normal menu structure
            unhighlight_all_menu_items(current_menu);
            uint8_t a;
            for (a = 0; a < main_menu_count; a++) {
                if (main_menu[a].menu_id == current_menu) {
                    main_menu[a].menu_item[main_menu[a].menu_selection_index].highlighted = true;
                }
            }
        }
    }
}

void main_menu_knob_handler(ButtonID button_index, int8_t diff) {
    if (button_index == E1_pos) {
        // are we inside a parameter menu?
        if (parameter_menu_active) {
            parameter_selection_index += diff;
            parameter_selection_index = limit_range(parameter_selection_index, 0, get_current_item_choice_count() - 1);

        } else {
            // we are inside the normal menu structure
            uint8_t a;
            for (a = 0; a < main_menu_count; a++) {
                if (main_menu[a].menu_id == current_menu) {
                    main_menu[a].menu_selection_index += diff;

                    main_menu[a].menu_selection_index = limit_range(main_menu[a].menu_selection_index, 0,
                            main_menu[a].menu_items_count - 1);

                    unhighlight_all_menu_items(current_menu);
                    unselect_all_menu_items(current_menu);
                    main_menu[a].menu_item[main_menu[a].menu_selection_index].selected = true;
                }
            }
        }
    }
}


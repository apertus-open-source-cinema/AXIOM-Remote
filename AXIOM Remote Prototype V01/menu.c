
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

#ifndef MENU_C
#define MENU_C

uint8_t get_current_menu_item_count() {
    if (_current_menu == Main) {
        return 5; //sizeof (_menu_main_item) / sizeof *(_menu_main_item);
    }
    /* if (_current_menu == Submenu1) {
         return sizeof (submenu1_item_labels) / sizeof *(submenu1_item_labels);
     }
     if (_current_menu == Submenu2) {
         return array_len(submenu2_item_labels);
     }*/
}

uint8_t getCurrentParameterItemCount() {
    if (_parameter_menu_active == 3)
        return 2;
    if (_parameter_menu_active == 4)
        return 4;
}

bool menu_item_test_action() {
    //do something!
    return true;
}

char * menu_item_test_get_current_value(menu_item_t this) {
    char return_value[32];
    strcpy(return_value, this.choices[this.value].label);
    return return_value;
}

void draw_menu_item(uint16_t x, uint16_t y, menu_item_t menu_main_item, bool selected, bool highlighted) {
    // don't draw empty items
    if (menu_main_item.label == NULL) {
        return;
    }

    char value[16];
    if (menu_main_item.type == submenu) {
        sprintf(value, ">");
    } else {
        //sprintf(value, "%d", menu_main_item.value);
        //sprintf(value, "%d", (*menu_main_item.current_value_ptr)(menu_main_item));
        strcpy(value, (*menu_main_item.current_value_ptr)(menu_main_item));
    }

    // is the current line highlighted?
    if (highlighted && !menu_main_item.disabled) {
        fillRect(x, y, _width, 29, _menu_hightlighted_item_color);
        drawString(x + 5, y + 19, menu_main_item.label, _menu_selected_text_color, _menu_selected_text_color, 1, left, 0);
        drawString(x + 210, y + 19, value, _menu_selected_text_color, _menu_selected_text_color, 1, right, 80);
        return;
    }

    // is a parameter menu active currently and the item is disabled?
    if (_parameter_menu_active && menu_main_item.disabled) {
        fillRect(x, y, _width, 29, menu_dimmed_item_color);
        drawString(x + 5, y + 19, menu_main_item.label, _menu_disabled_text_color, _menu_disabled_text_color, 1, left, 0);
        drawString(x + 210, y + 19, value, _menu_disabled_text_color, _menu_disabled_text_color, 1, right, 80);
        return;
    }

    // is a parameter menu active currently?
    if (_parameter_menu_active) {
        fillRect(x, y, _width, 29, menu_dimmed_item_color);
        drawString(x + 5, y + 19, menu_main_item.label, menu_text_color, menu_text_color, 1, left, 0);
        drawString(x + 210, y + 19, value, menu_text_color, menu_text_color, 1, right, 80);
        return;
    }

    // is the current line selected and disabled?
    if (selected && menu_main_item.disabled) {
        fillRect(x, y, _width, 29, _menu_disabled_item_color);
        fillRect(0, y, 3, 29, menu_selected_item_color);
        fillRect(_width - 16 - 3, y, 3, 29, menu_selected_item_color);
        drawString(x + 5, y + 19, menu_main_item.label, _menu_disabled_text_color, _menu_disabled_text_color, 1, left, 0);
        drawString(x + 210, y + 19, value, _menu_disabled_text_color, _menu_disabled_text_color, 1, right, 80);
        return;
    }

    // is the current line disabled?
    if (menu_main_item.disabled) {
        fillRect(x, y, _width, 29, _menu_disabled_item_color);
        drawString(x + 5, y + 19, menu_main_item.label, _menu_disabled_text_color, _menu_disabled_text_color, 1, left, 0);
        drawString(x + 210, y + 19, value, _menu_disabled_text_color, _menu_disabled_text_color, 1, right, 80);
        return;
    }

    // is the current line selected (cursor)?
    if (selected) {
        fillRect(x, y, _width, 29, menu_selected_item_color);
        drawString(x + 5, y + 19, menu_main_item.label, _menu_selected_text_color, _menu_selected_text_color, 1, left, 0);
        drawString(x + 210, y + 19, value, _menu_selected_text_color, _menu_selected_text_color, 1, right, 80);
        return;
    }

    //if nothing of the above applies simply draw the line item normally
    fillRect(x, y, _width, 29, menu_item_color);
    drawString(x + 5, y + 19, menu_main_item.label, menu_text_color, menu_text_color, 1, left, 0);
    drawString(x + 210, y + 19, value, menu_text_color, menu_text_color, 1, right, 80);
}

void draw_scroll_indicator(uint8_t number, uint8_t CurrentMenuItemCount) {
    uint8_t scrollbarheight = (_height - 31) * (float) ((float) number / (float) (CurrentMenuItemCount));
    uint8_t scrollbaroffset = 31 + _menu_offset * (((_height - 31) - scrollbarheight) / (CurrentMenuItemCount - number));

    //Background
    fillRect(_width - 16, 31, 16, _height - 31, menu_item_color);

    //Thin Line
    fillRect(_width - 10, 31, 4, _height - 31, menu_text_color);

    //Thick Line
    fillRect(_width - 14, scrollbaroffset, 12, scrollbarheight, menu_text_color);
    //
}

/**************************************************************************/
/*!
   @brief  initialize our menu structure with default values and definitions
 * 
 */

/**************************************************************************/
void initMenu() {
    //Color Theme definitions
    menu_item_color = color565(255, 255, 255);
    menu_dimmed_item_color = color565(247, 251, 247);
    _menu_disabled_item_color = color565(255, 255, 255);
    menu_selected_item_color = color565(255, 128, 0);
    menu_background_color = color565(180, 180, 180);
    menu_text_color = color565(0, 0, 0);
    _menu_selected_text_color = color565(255, 255, 255);
    _menu_hightlighted_item_color = color565(0, 128, 255);
    _menu_disabled_text_color = color565(230, 230, 230);


    /* strcpy(submenu1_item_labels[0], "<- back");
     strcpy(submenu1_item_labels[1], "Submenu1 Item 1");
     strcpy(submenu1_item_labels[2], "Submenu1 Item 2");
     strcpy(submenu1_item_labels[3], "Submenu1 Item 3");
     strcpy(submenu1_item_labels[4], "Submenu1 Item 4");


     strcpy(submenu2_item_labels[0], "<- back");
     strcpy(submenu2_item_labels[1], "Submenu2 Item 1");
     strcpy(submenu2_item_labels[2], "Submenu2 Item 2");
     strcpy(submenu2_item_labels[3], "Submenu2 Item 3");*/

    mainMenuItem2Choices[0].value = 0;
    strcpy(mainMenuItem2Choices[0].label, "Off");
    mainMenuItem2Choices[1].value = 1;
    strcpy(mainMenuItem2Choices[1].label, "On");
    //mainMenuItem2 = 0;

    mainMenuItem3Choices[0].value = 0;
    strcpy(mainMenuItem3Choices[0].label, "Low");
    mainMenuItem3Choices[1].value = 1;
    strcpy(mainMenuItem3Choices[1].label, "Medium");
    mainMenuItem3Choices[2].value = 2;
    strcpy(mainMenuItem3Choices[2].label, "High");
    mainMenuItem3Choices[3].value = 3;
    strcpy(mainMenuItem3Choices[3].label, "Crazy");
    //mainMenuItem3 = 0;


    // new menu structure definitions
    _menu_main_item[0].disabled = false;
    _menu_main_item[0].hidden = false;
    strcpy(_menu_main_item[0].label, "Submenu 1");
    _menu_main_item[0].type = submenu;
    _menu_main_item[0].link_to_submenu = Submenu1;
    _menu_main_item[0].current_value_ptr = &menu_item_test_get_current_value;

    _menu_main_item[1].disabled = false;
    _menu_main_item[1].hidden = false;
    strcpy(_menu_main_item[1].label, "Submenu 2");
    _menu_main_item[1].type = submenu;
    _menu_main_item[0].link_to_submenu = Submenu2;
    _menu_main_item[1].current_value_ptr = &menu_item_test_get_current_value;

    _menu_main_item[2].disabled = true;
    _menu_main_item[2].hidden = false;
    strcpy(_menu_main_item[2].label, "disabled sample");
    _menu_main_item[2].type = dropdown;
    _menu_main_item[2].current_value_ptr = &menu_item_test_get_current_value;

    _menu_main_item[3].disabled = false;
    _menu_main_item[3].hidden = false;
    strcpy(_menu_main_item[3].label, "Fun");
    _menu_main_item[3].type = dropdown;
    _menu_main_item[3].choices[0] = mainMenuItem2Choices[0];
    _menu_main_item[3].choices[1] = mainMenuItem2Choices[1];
    _menu_main_item[3].choice_count = array_len(mainMenuItem2Choices);
    _menu_main_item[3].current_value_ptr = &menu_item_test_get_current_value;

    _menu_main_item[4].disabled = false;
    _menu_main_item[4].hidden = false;
    strcpy(_menu_main_item[4].label, "Fun Level");
    _menu_main_item[4].type = dropdown;
    //_menu_main_item[4].action_ptr = &menu_item_test_action;
    _menu_main_item[4].current_value_ptr = &menu_item_test_get_current_value;
    _menu_main_item[4].choices[0] = mainMenuItem3Choices[0];
    _menu_main_item[4].choices[1] = mainMenuItem3Choices[1];
    _menu_main_item[4].choices[2] = mainMenuItem3Choices[2];
    _menu_main_item[4].choices[3] = mainMenuItem3Choices[3];
    _menu_main_item[4].choice_count = array_len(mainMenuItem3Choices);


    _menu_sub1_item[0].disabled = false;
    _menu_sub1_item[0].hidden = false;
    strcpy(_menu_sub1_item[0].label, "Submenu 1 Item 1");
    _menu_sub1_item[0].type = dropdown;

    _menu_sub1_item[1].disabled = false;
    _menu_sub1_item[1].hidden = false;
    strcpy(_menu_sub1_item[1].label, "Submenu 1 Item 2");
    _menu_sub1_item[1].type = dropdown;

    _menu_sub1_item[2].disabled = false;
    _menu_sub1_item[2].hidden = false;
    strcpy(_menu_sub1_item[2].label, "Submenu 1 Item 3");
    _menu_sub1_item[2].type = dropdown;

    //menu_item_t _menu_sub2_item[5];

    _current_menu = Main;
    _parameter_menu_active = 0;
    _parameter_selection_index = 0;
    _menu_offset = 0;

    strcpy(menu_breadcrumbs, "Menu");
}

/**************************************************************************/
/*!
   @brief  Draw a menu where the parameter of a certain option is displayed.
 * This typically looks like a kind of drop-down menu with several options to choose from
 * 

    @param    x  location (attention: top right corner)
    @param    y  location (attention: top right corner)
    @param    items[] the struct containing the different options to display
    @param    currentvalueindex which option (index) from the struct is the current setting
 */

/**************************************************************************/
void draw_parameter_menu(uint16_t x, uint16_t y, menu_item_t menuitem) {
    //uint8_t choice_number = 2;//choice_count; //getCurrentParameterItemCount(); // why doesn't this work?: sizeof (items) / sizeof (*items); //
    uint16_t height = 2 + 2 + menuitem.choice_count * 30;

    //calculate the maximum width of the provided text options so we can define the menu width
    uint16_t max_width = 0;
    uint8_t i;
    for (i = 0; i < menuitem.choice_count; i++) {
        uint16_t x1, y1, w1, h1;
        getTextBounds(menuitem.choices[i].label, x, y, &x1, &y1, &w1, &h1);
        if (w1 > max_width) {
            max_width = w1;
        }
    }
    // add 25 pixel space to the widest label
    uint16_t width = max_width + 25;

    // alter x now that we know the width of the menu
    // x and y define the top right corner here
    x -= width;

    //don't draw over the LCD edge
    int16_t edge = (y + height) - _height;
    if ((edge < 4) && (edge > 0)) {
        height = _height - y;
    }

    //if the menu would clip entire lines in the available height just shift it up as a whole
    if (edge > 4) {
        y -= edge - 3;
        height = _height - y;
    }

    // todo
    // handle the case that there are more parameter options than fit in the height of the LCD

    // draw menu borders
    fillRect(x, y, width, height, menu_background_color);
    drawRect(x + 1, y + 1, width - 2, height - 2, menu_text_color);

    for (i = 0; i < menuitem.choice_count; i++) {
        char draw_label[32];
        strcpy(draw_label, menuitem.choices[i].label);

        if (i == _parameter_selection_index) {
            if (btn_E1_pressed) {
                fillRect(x + 2, y + 2 + i * 30, width - 4, 29, _menu_hightlighted_item_color);
                drawString(x + 12, y + 21 + i * 30, draw_label, _menu_selected_text_color, _menu_selected_text_color, 1, left, 0);
            } else {
                fillRect(x + 2, y + 2 + i * 30, width - 4, 29, menu_selected_item_color);
                drawString(x + 12, y + 21 + i * 30, draw_label, _menu_selected_text_color, _menu_selected_text_color, 1, left, 0);
            }
        } else {
            fillRect(x + 2, y + 2 + i * 30, width - 4, 29, menu_item_color);
            drawString(x + 12, y + 21 + i * 30, draw_label, menu_text_color, menu_text_color, 1, left, 0);
        }

        // add a circle icon at beginning of the line and label of the current setting
        if (i == menuitem.value) {
            if (i == _parameter_selection_index) {
                fillCircle(x + 6, y + 15 + i * 30, 3, _menu_selected_text_color);
            } else {
                fillCircle(x + 6, y + 15 + i * 30, 3, menu_text_color);
            }
        }
    }
}
void drawMenu(bool firstime) {
    if (firstime) {
        //clear the screen
        fillRect(0, 0, _width, _height, menu_background_color);

        //draw header
        fillRect(0, 0, _width, 28, menu_item_color);
        drawString(5, 19, menu_breadcrumbs, menu_text_color, menu_text_color, 1, left, 0);
        drawLine(0, 29, _width, 29, menu_selected_item_color);
        drawLine(0, 30, _width, 30, menu_background_color);
    }

    if (_current_menu == Main) {
        uint8_t i;
        int8_t display_selection_index = _menu_selection_index - _menu_offset;
        if (display_selection_index >= 7) {
            _menu_offset += 1;
        }
        if (display_selection_index < 0) {
            _menu_offset -= 1;
        }

        int number = 5; //sizeof (menuItemLabels) / sizeof *(menuItemLabels);

        number = LimitRange(number, 0, 7);

        for (i = 0; i < number; i++) {
            draw_menu_item(0, 31 + i * 30, _menu_main_item[i + _menu_offset], i + _menu_offset == _menu_selection_index, ((i + _menu_offset == _menu_selection_index) && btn_E1_pressed));
            //draw_menu_item(0, 31+i*30, menuItemLabels[i+menu_offset], menuItemValuesText[i+menu_offset], i+menu_offset==menuSelectionIndex, ((i+menu_offset==menuSelectionIndex) && btn_E1_pressed));
        }
        if (number == 7) {
            draw_scroll_indicator(number, get_current_menu_item_count());
        }

        // draw parameter menu
        if (_parameter_menu_active != 0) {
            uint16_t offset = 31 + (_parameter_menu_active - _menu_offset)*30 - 2;
            draw_parameter_menu(304, offset, _menu_main_item[_parameter_menu_active]);
        }
    }

    if (_current_menu == Submenu1) {
        uint8_t i;
        //int number = sizeof (submenu1_item_labels) / sizeof *(submenu1_item_labels);
        int number = array_len(_menu_sub1_item); // (submenu1_item_labels) / sizeof *(submenu1_item_labels);
        for (i = 0; i < number; i++) {
            draw_menu_item(0, 31 + i * 30, _menu_sub1_item[i + _menu_offset], i == _menu_selection_index, ((i == _menu_selection_index) && btn_E1_pressed));
        }
    }

    if (_current_menu == Submenu2) {
        int i;
        //int number = sizeof (submenu2_item_labels) / sizeof *(submenu2_item_labels);
        // for (i = 0; i < number; i++) {
        //draw_menu_item(0, 31+i*30, submenu2_item_labels[i], "", i==menuSelectionIndex, ((i==menuSelectionIndex) && btn_E1_pressed));
        //}
    }

    //drawRGBBitmap(10, 10, (uint16_t*)(gradient.pixel_data), gradient.width, gradient.height);
    //fillCircle(150, 5, 3, menuSelectedItemColor);
    //drawRGBBitmap(155, 0, (uint16_t*)(axiom_logo.pixel_data), axiom_logo.width, axiom_logo.height);

    //setLCDBacklight(menuItemValues[0]);
}


#endif /* MENU_C */
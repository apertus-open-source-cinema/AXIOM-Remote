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

#include "page_wb.h"

#include <stdio.h>
#include <string.h>

#include "draw.h"
#include "globals.h"
#include "utility.h"
#include "media/media.h"

// Color Definitions
static uint16_t page_item_label_color;
static uint16_t page_item_value_color;
static uint16_t page_item_label_background_color;
static uint16_t page_item_value_background_color;
static uint16_t page_item_highlight_color;
static uint16_t page_background_color;
static uint16_t page_options_background_color;
static uint16_t button_primary_background_color;
static uint16_t button_background_color;
static uint16_t button_hightlight_background_color;
static uint16_t page_item_width;
static uint8_t padding_side;
static uint8_t padding_elements;
static uint8_t page_item_label_height;
static uint8_t page_item_value_height;
static uint8_t wb_menu_highlight_index;



/**************************************************************************/
/*!
    @brief    draw a standard page item that contains a value and label 
    @param    screen_index - position of the item on screen, there is space for 3 top and 3 bottom, total 6 items
    @param    page_item - the struct containing details about the page item
    @param    highlighted - is the item currently highlighted
 */

/**************************************************************************/
void draw_wb_page_item(uint8_t screen_index) {

    // don't draw empty or disabled items
    if ((main_page[PAGE_WB].page_item[screen_index].label == NULL) || (main_page[PAGE_WB].page_item[screen_index].disabled)) {
        return;
    }

    // the screen only provides space for 6 items
    screen_index = limit_range(screen_index, 0, 5);

    uint16_t page_item_height = page_item_label_height + page_item_value_height;

    // derive x,y coordinates from screen_index
    uint16_t x, y;
    if (screen_index == 0) {
        x = padding_side;
        y = FRAMEBUFFER_TOP - page_item_height;
    }
    if (screen_index == 1) {
        x = padding_side + padding_elements + page_item_width;
        y = FRAMEBUFFER_TOP - page_item_height;
    }
    if (screen_index == 2) {
        x = padding_side + 2 * padding_elements + 2 * page_item_width;
        y = FRAMEBUFFER_TOP - page_item_height;
    }
    if (screen_index == 3) {
        x = padding_side;
        y = 0;
    }
    if (screen_index == 4) {
        x = padding_side + padding_elements + page_item_width;
        y = 0;
    }
    if (screen_index == 5) {
        x = padding_side + 2 * padding_elements + 2 * page_item_width;
        y = 0;
    }

    // handle item highlighting
    if (main_page[PAGE_WB].page_item[screen_index].highlighted) {
        page_item_label_color = page_item_label_background_color;
        page_item_label_background_color = button_hightlight_background_color;
        page_item_value_color = page_item_label_color;
        page_item_value_background_color = page_item_label_background_color;
    } else {
        page_item_label_color = main_page[PAGE_WB].page_item[screen_index].text_color;
        page_item_label_background_color = main_page[PAGE_WB].page_item[screen_index].background_color; //_page_item_label_background_color;
        page_item_value_background_color = page_item_value_background_color;
        page_item_value_color = page_item_value_color;
    }

    if (screen_index < 3) {
        // 3 top items

        if (main_page[PAGE_WB].page_item[screen_index].item_type == BUTTON) {
            fill_round_rect(x, y + 2 + button_height, page_item_width, button_height, 3, page_item_label_background_color);
            draw_string(x, y + 9 + button_height, main_page[PAGE_WB].page_item[screen_index].label,
                    page_item_label_color, page_item_label_color,
                    FreeSans12pt7b, TEXT_ALIGN_CENTER, page_item_width);
        }
        if (main_page[PAGE_WB].page_item[screen_index].item_type == VALUE_AND_LABEL) {
            // draw label
            fill_round_rect(x, y + page_item_value_height, page_item_width, page_item_label_height, 3,
                    page_item_label_background_color);
            fill_rect(x, y + page_item_value_height, page_item_width, 3, page_item_label_background_color);
            draw_string(x, y + page_item_value_height + 7, main_page[PAGE_WB].page_item[screen_index].label,
                    page_item_label_color, page_item_value_color,
                    FreeSans9pt7b, TEXT_ALIGN_CENTER, page_item_width);

            // draw value
            fill_round_rect(x, y, page_item_width, page_item_value_height, 3, page_item_value_background_color);
            fill_rect(x, y + page_item_value_height - 3, page_item_width, 3, page_item_value_background_color);
            draw_string(x, y + 10, main_page[PAGE_WB].page_item[screen_index].value, page_item_value_color, page_item_value_color,
                    FreeSans12pt7b, TEXT_ALIGN_CENTER, page_item_width);
        }
    } else {
        // 3 bottom items

        if (main_page[PAGE_WB].page_item[screen_index].item_type == BUTTON) {
            fill_round_rect(x, y + 2, page_item_width, button_height, 3, page_item_label_background_color);
            draw_string(x, y + 9, main_page[PAGE_WB].page_item[screen_index].label, page_item_label_color, page_item_label_color,
                    FreeSans12pt7b, TEXT_ALIGN_CENTER, page_item_width);
        }
        if (main_page[PAGE_WB].page_item[screen_index].item_type == VALUE_AND_LABEL) {
            // draw label

            fill_round_rect(x, y + 1, page_item_width, page_item_label_height, 3, page_item_label_background_color);
            fill_rect(x, y + page_item_label_height - 3, page_item_width, 3, page_item_label_background_color);
            draw_string(x, y + 7, main_page[PAGE_WB].page_item[screen_index].label, page_item_label_color, page_item_label_color,
                    FreeSans9pt7b, TEXT_ALIGN_CENTER, page_item_width);

            // draw value
            fill_round_rect(x, y + page_item_label_height, page_item_width, page_item_value_height, 3, page_item_value_background_color);
            fill_rect(x, y + page_item_label_height, page_item_width, 3, page_item_value_background_color);
            draw_string(x, y + page_item_label_height + 6, main_page[PAGE_WB].page_item[screen_index].value, page_item_value_color, page_item_value_color,
                    FreeSans12pt7b, TEXT_ALIGN_CENTER, page_item_width);
        }
    }
}

void draw_wb_page_side_items() {
    //left side

    //Home Icon
    if (main_page[PAGE_WB].page_item[6].highlighted) {
        drawRGBBitmap(0, 158, (uint16_t*) (home_icon_highlight.pixel_data), home_icon_highlight.width, home_icon_highlight.height);
    } else {
        drawRGBBitmap(0, 158, (uint16_t*) (home_icon.pixel_data), home_icon.width, home_icon.height);
    }

    // Back Icon
    if (main_page[PAGE_WB].page_item[7].highlighted) {
        drawRGBBitmap(0, 108, (uint16_t*) (back_icon_highlight.pixel_data), back_icon_highlight.width, back_icon_highlight.height);
    } else {
        drawRGBBitmap(0, 108, (uint16_t*) (back_icon.pixel_data), back_icon.width, back_icon.height);
    }

    // Edit/Trash Icon
    if (main_page[PAGE_WB].page_item[8].highlighted) {
        //drawRGBBitmap(0, 58, (uint16_t*) (back_icon_highlight.pixel_data), back_icon_highlight.width, back_icon_highlight.height);
    } else {
        drawRGBBitmap(0, 58, (uint16_t*) (trash_icon.pixel_data), trash_icon.width, trash_icon.height);
    }

    //right side

    // Up Icon
    if (main_page[PAGE_WB].page_item[9].highlighted) {
        drawRGBBitmap(FRAMEBUFFER_WIDTH - 1 - up_icon_highlight.width, 158, (uint16_t*) (up_icon_highlight.pixel_data), up_icon_highlight.width, up_icon_highlight.height);
    } else {
        drawRGBBitmap(FRAMEBUFFER_WIDTH - 1 - up_icon.width, 158, (uint16_t*) (up_icon.pixel_data), up_icon.width, up_icon.height);
    }

    // Help Icon
    if (main_page[PAGE_WB].page_item[10].highlighted) {
        //drawRGBBitmap(_width - 1 - down_icon_highlight.width, 108, (uint16_t*) (down_icon_highlight.pixel_data), down_icon_highlight.width, up_icon_highlight.height);
    } else {
        drawRGBBitmap(FRAMEBUFFER_WIDTH - 1 - help_icon.width, 108, (uint16_t*) (help_icon.pixel_data), help_icon.width, help_icon.height);
    }

    // Down Icon
    if (main_page[PAGE_WB].page_item[11].highlighted) {
        drawRGBBitmap(FRAMEBUFFER_WIDTH - 1 - down_icon_highlight.width, 58, (uint16_t*) (down_icon_highlight.pixel_data), down_icon_highlight.width, up_icon_highlight.height);
    } else {
        drawRGBBitmap(FRAMEBUFFER_WIDTH - 1 - down_icon.width, 58, (uint16_t*) (down_icon.pixel_data), down_icon.width, down_icon.height);
    }
}

void draw_wb_option_item(uint16_t x, uint16_t y, uint8_t option_item_index, bool selected) {
    // don't draw empty items
    if (white_balance_parameter.white_balance_options[option_item_index].label == NULL) {

        return;
    }

    uint16_t yoffset_label_from_base = 7;

    // is the current line selected (cursor)?
    if (selected) {
        fill_rect(x, y, FRAMEBUFFER_WIDTH - x - 28, 29, page_item_highlight_color);

        draw_string(x + 5, y + yoffset_label_from_base, white_balance_parameter.white_balance_options[option_item_index].label,
                page_item_label_color, page_item_label_color, FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);

        char value_string[8];
        sprintf(value_string, "%dK", white_balance_parameter.white_balance_options[option_item_index].Kelvin);
        draw_string(x + 5 + 120, y + yoffset_label_from_base, value_string,
                page_item_label_color, page_item_label_color, FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);
        //draw_string(x + 210, y + yoffset_label_from_base, value, _menu_selected_text_color, _menu_selected_text_color, FreeSans9pt7b, ALIGN_RIGHT, 80);
        return;
    }

    //draw the option item normally
    //fill_rect(x, y, _width, 29, _page_item_value_background_color);
    draw_string(x + 5, y + yoffset_label_from_base, white_balance_parameter.white_balance_options[option_item_index].label,
            page_item_value_color, page_item_value_color, FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);
    char value_string[32];
    sprintf(value_string, "%dK", white_balance_parameter.white_balance_options[option_item_index].Kelvin);
    draw_string(x + 5 + 120, y + yoffset_label_from_base, value_string,
            page_item_value_color, page_item_value_color, FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);
    //draw_string(x + 210, y + yoffset_label_from_base, value, menu_text_color, menu_text_color, FreeSans9pt7b, ALIGN_RIGHT, 80);
}
/**************************************************************************/
/*!
    @brief    draw the page
 */
/**************************************************************************/
void draw_wb_page() {
    //clear the screen
    //clear_screen(page_item_highlight_color);
    //fill_rect(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, page_item_highlight_color);
    fill_rect(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, page_background_color);


    //draw page items
    uint8_t i;
    for (i = 0; i < main_page[PAGE_WB].page_items_count; i++) {
        draw_wb_page_item(i);
    }


    // draw options area
    uint16_t page_item_height = page_item_label_height + page_item_value_height;
    uint16_t available_height = FRAMEBUFFER_HEIGHT - 2 * button_height - 8;

    fill_rect(0, button_height + 4, FRAMEBUFFER_WIDTH, available_height, page_options_background_color);

    // Draw header
    draw_string(5, FRAMEBUFFER_HEIGHT - 30, main_page[PAGE_WB].label, page_item_label_color, page_item_label_color,
            FreeSans18pt7b, TEXT_ALIGN_LEFT, 0);

    // separation line
    draw_line(0, FRAMEBUFFER_HEIGHT - 34, FRAMEBUFFER_RIGHT, FRAMEBUFFER_TOP - 34, page_item_highlight_color);

    //draw rotary knob indicator/illustration
    drawRGBBitmap(37, 76, (uint16_t*) (wheel_illustration.pixel_data), wheel_illustration.width, wheel_illustration.height);

    //draw option items
    if (wb_menu_highlight_index > 1)
        draw_wb_option_item(110, button_height + 2 + available_height - 38, wb_menu_highlight_index - 2, false);

    if (wb_menu_highlight_index > 0)
        draw_wb_option_item(110, button_height + 2 + available_height - 38 - (1 * 30), wb_menu_highlight_index - 1, false);

    draw_wb_option_item(110, button_height + 2 + available_height - 38 - (2 * 30), wb_menu_highlight_index, true);

    if (wb_menu_highlight_index < white_balance_parameter.white_balance_options_count - 1)
        draw_wb_option_item(110, button_height + 2 + available_height - 38 - (3 * 30), wb_menu_highlight_index + 1, false);

    if (wb_menu_highlight_index < white_balance_parameter.white_balance_options_count - 2)
        draw_wb_option_item(110, button_height + 2 + available_height - 38 - (4 * 30), wb_menu_highlight_index + 2, false);

    // draw side icons
    draw_wb_page_side_items();
}

/*
char * wb_item_get_current_value(uint8_t menu_item_index) {
    return "Test"; //_wb_menu.menu_item[menu_item_index];
}*/

void init_wb_page() {

    //colors
    page_item_label_color = color565(255, 255, 255);
    page_item_value_color = color565(0, 0, 0);
    page_item_label_background_color = color565(0, 0, 0);
    page_item_value_background_color = color565(255, 255, 255);
    page_item_highlight_color = color565(255, 128, 0);

    page_background_color = color565(97, 92, 91);
    button_primary_background_color = color565(255, 128, 0);
    button_hightlight_background_color = color565(0, 128, 255);
    button_background_color = color565(220, 220, 220);
    page_options_background_color = color565(255, 255, 255);


    // dimensions
    button_height = 30;
    padding_side = 8;
    padding_elements = 11;
    page_item_width = 94;
    //_page_item_height = 75;
    page_item_label_height = 25;
    page_item_value_height = 40;

    uint8_t j = 0;
    strcpy(main_page[PAGE_WB].label, "White Balance");
    main_page[PAGE_WB].page_id = PAGE_WB;

    main_page[PAGE_WB].page_item[j].disabled = true;
    j++;
    main_page[PAGE_WB].page_item[j].disabled = true;
    j++;
    main_page[PAGE_WB].page_item[j].disabled = true;
    j++;
    main_page[PAGE_WB].page_item[j].disabled = false;
    strcpy(main_page[PAGE_WB].page_item[j].label, "Cancel");
    main_page[PAGE_WB].page_item[j].highlighted = false;
    main_page[PAGE_WB].page_item[j].item_type = BUTTON;
    main_page[PAGE_WB].page_item[j].text_color = page_item_value_color;
    main_page[PAGE_WB].page_item[j].background_color = button_background_color;
    j++;
    main_page[PAGE_WB].page_item[j].disabled = false;
    strcpy(main_page[PAGE_WB].page_item[j].label, "Add");
    main_page[PAGE_WB].page_item[j].highlighted = false;
    main_page[PAGE_WB].page_item[j].item_type = BUTTON;
    main_page[PAGE_WB].page_item[j].text_color = page_item_value_color;
    main_page[PAGE_WB].page_item[j].background_color = button_background_color;
    j++;
    main_page[PAGE_WB].page_item[j].disabled = false;
    strcpy(main_page[PAGE_WB].page_item[j].label, "Set");
    main_page[PAGE_WB].page_item[j].highlighted = false;
    main_page[PAGE_WB].page_item[j].item_type = BUTTON;
    main_page[PAGE_WB].page_item[j].text_color = page_item_value_color;
    main_page[PAGE_WB].page_item[j].background_color = button_primary_background_color;

    main_page[PAGE_WB].page_items_count = j + 1;

    j++;
    main_page[PAGE_WB].page_item[j].disabled = false;
    strcpy(main_page[PAGE_WB].page_item[j].label, "Home");
    main_page[PAGE_WB].page_item[j].highlighted = false;
    main_page[PAGE_WB].page_item[j].item_type = BUTTON;
    main_page[PAGE_WB].page_item[j].text_color = page_item_value_color;
    main_page[PAGE_WB].page_item[j].background_color = button_primary_background_color;

    wb_menu_highlight_index = 0;
}

void wb_page_button_press_handler(ButtonID button_index) {
    if (button_index == P1) {
        main_page[PAGE_WB].page_item[0].highlighted = true;
    }
    if (button_index == P2) {
        main_page[PAGE_WB].page_item[1].highlighted = true;
    }
    if (button_index == P3) {
        main_page[PAGE_WB].page_item[2].highlighted = true;
    }
    if (button_index == P4) {
        main_page[PAGE_WB].page_item[3].highlighted = true;
    }
    if (button_index == P5) {
        main_page[PAGE_WB].page_item[4].highlighted = true;
    }
    if (button_index == P6) {
        main_page[PAGE_WB].page_item[5].highlighted = true;
    }
    if (button_index == P7) {
        main_page[PAGE_WB].page_item[6].highlighted = true;
    }
    if (button_index == P8) {
        main_page[PAGE_WB].page_item[7].highlighted = true;
    }
    if (button_index == P9) {
        main_page[PAGE_WB].page_item[8].highlighted = true;
    }
    if (button_index == P10) {
        main_page[PAGE_WB].page_item[9].highlighted = true;
    }
    if (button_index == P11) {
        main_page[PAGE_WB].page_item[10].highlighted = true;
    }
    if (button_index == P12) {

        main_page[PAGE_WB].page_item[11].highlighted = true;
    }

}

void wb_page_button_release_handler(ButtonID button_index) {
    if (button_index == P1) {
        main_page[PAGE_WB].page_item[0].highlighted = false;
    }
    if (button_index == P2) {
        main_page[PAGE_WB].page_item[1].highlighted = false;
    }
    if (button_index == P3) {
        main_page[PAGE_WB].page_item[2].highlighted = false;
    }
    if (button_index == P4) {
        main_page[PAGE_WB].page_item[3].highlighted = false;

        navigate_to_page(PAGE_HOME, TRANSITION_PUSH_DOWN);
    }
    if (button_index == P5) {
        main_page[PAGE_WB].page_item[4].highlighted = false;
    }
    if (button_index == P6) {
        // turn off button highlight
        main_page[PAGE_WB].page_item[5].highlighted = false;

        // set the highlighted white balance
        white_balance_parameter.white_balance_selection_index = wb_menu_highlight_index;

        // update the new white balance value on the home page
        update_pages();

        // navigate to home page
        navigate_to_page(PAGE_HOME, TRANSITION_PUSH_DOWN);
    }
    if (button_index == P7) {

        main_page[PAGE_WB].page_item[6].highlighted = false;

        navigate_to_page(PAGE_HOME, TRANSITION_PUSH_DOWN);
    }
    if (button_index == P8) {

        main_page[PAGE_WB].page_item[7].highlighted = false;

        navigate_to_page(PAGE_HOME, TRANSITION_PUSH_DOWN);
    }
    if (button_index == P9) {
        main_page[PAGE_WB].page_item[8].highlighted = false;
    }
    if (button_index == P10) {

        wb_menu_highlight_index--;
        wb_menu_highlight_index = limit_range(wb_menu_highlight_index, 0, white_balance_parameter.white_balance_options_count - 1);

        main_page[PAGE_WB].page_item[9].highlighted = false;
    }
    if (button_index == P11) {
        main_page[PAGE_WB].page_item[10].highlighted = false;

        navigate_to_page(PAGE_WB_HELP, TRANSITION_PUSH_LEFT);
    }
    if (button_index == P12) {

        wb_menu_highlight_index++;
        wb_menu_highlight_index = limit_range(wb_menu_highlight_index, 0, white_balance_parameter.white_balance_options_count - 1);
        main_page[PAGE_WB].page_item[11].highlighted = false;
    }
    if (button_index == E1) {
        white_balance_parameter.white_balance_selection_index = wb_menu_highlight_index;

        update_pages();
        navigate_to_page(PAGE_HOME, TRANSITION_PUSH_DOWN);
    }
}

void wb_page_knob_handler(ButtonID button_index, int8_t diff) {
    if (button_index == E1_ROT) {
        wb_menu_highlight_index += diff;
        wb_menu_highlight_index = limit_range(wb_menu_highlight_index, 0, white_balance_parameter.white_balance_options_count - 1);
    }
}

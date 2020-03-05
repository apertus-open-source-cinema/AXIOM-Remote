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

#include "page_wb_help.h"

//#include <stdint.h>
//#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "utility.h"
#include "media/media.h"

#include "media/media.h"

uint16_t help_page_text_color;
uint16_t help_page_background_color;
uint8_t help_page_padding_side;
uint16_t help_page_scroll_index;
char wb_help_text[512];

int8_t wrap_tester;

void draw_wb_help_scroll_indicator(uint8_t line, uint8_t total_lines) {
    // maximum height is the screen without header area
    uint8_t srollbar_max_height = FRAMEBUFFER_HEIGHT - 30;

    // height of the scroll indicator is defined by the ratio of number of items on screen vs total number of item. 
    // if there are 7 items on screen of total 14 items the scroll indicator shall be 50% of the scrollbar height
    uint8_t scrollbarheight = srollbar_max_height * (float) ((float) line / (float) (total_lines));

    //
    uint8_t scrollbaroffset = ((total_lines - line) - menu_offset) * ((srollbar_max_height - scrollbarheight) / (total_lines - line));

    //Background
    fill_rect(FRAMEBUFFER_WIDTH - 16, 0, 16, FRAMEBUFFER_HEIGHT - 30, help_page_background_color);

    //Thin Line
    fill_rect(FRAMEBUFFER_WIDTH - 9, 0, 4, FRAMEBUFFER_HEIGHT - 30, help_page_text_color);

    //Thick Line
    fill_rect(FRAMEBUFFER_WIDTH - 13, scrollbaroffset, 12, scrollbarheight, help_page_text_color);

}

void draw_wb_help_page_side_items() {
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
}


/**************************************************************************/
/*!
    @brief    draw the page
 */

/**************************************************************************/
void draw_wb_help_page() {
    //clear the screen
    fill_rect(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, help_page_background_color);

    //draw header background
    //fill_rect(0, FRAMEBUFFER_TOP - 28, FRAMEBUFFER_WIDTH, 28,help_page_text_color);

    // Draw header
    draw_string(5, FRAMEBUFFER_HEIGHT - 30, "White Balance Help", help_page_text_color, help_page_text_color,
            FreeSans12pt7b, TEXT_ALIGN_LEFT, 0);

    //separation line
    draw_line(0, FRAMEBUFFER_TOP - 34, FRAMEBUFFER_RIGHT, FRAMEBUFFER_TOP - 34, color565(255, 128, 0));

    // draw side icons
    draw_wb_help_page_side_items();

    // draw scroll indicator
    draw_wb_help_scroll_indicator(help_page_scroll_index, 10);

    //debug
    draw_rect(30 + 6, 5, FRAMEBUFFER_WIDTH - 36 - 20 - wrap_tester, 200, color565(255, 0, 0));

    //draw content
    draw_string(30 + 6, FRAMEBUFFER_TOP - 30 - 25 + help_page_scroll_index, wb_help_text, help_page_text_color, help_page_text_color,
            FreeSans9pt7b, TEXT_ALIGN_LEFT, FRAMEBUFFER_WIDTH - 36 - 20 - wrap_tester);

    /*uint8_t length = string_len("This is a very long text with many letters ahahaha - its so incredible  lonmg.asdasd.");
    char debug[32];
    sprintf(debug, "length = %d", length);
    //debug_uart(_wb_help_text);
    debug_uart(debug);*/
}

void init_wb_help_page() {
    help_page_text_color = color565(0, 0, 0);
    help_page_background_color = color565(255, 255, 255);

    help_page_padding_side = 10;
    help_page_scroll_index = 0;

    strcpy(wb_help_text, "Headline\nThis is a very long text with many letters - "
            "it's so incredibly long that it does not all fit into one line on the screen.");

    wrap_tester = 0;
}

void wb_help_page_button_press_handler(ButtonID button_index) {
    if (button_index == P7) {
        main_page[PAGE_WB].page_item[6].highlighted = true;
    }
    if (button_index == P8) {
        main_page[PAGE_WB].page_item[7].highlighted = true;
    }
}

void wb_help_page_button_release_handler(ButtonID button_index) {
    if (button_index == P7) {
        main_page[PAGE_WB].page_item[6].highlighted = false;

        navigate_to_page(PAGE_HOME, TRANSITION_PUSH_RIGHT);
    }
    if (button_index == P8) {
        main_page[PAGE_WB].page_item[7].highlighted = false;

        navigate_to_page(PAGE_WB, TRANSITION_PUSH_RIGHT);
    }
}

void wb_help_page_knob_handler(ButtonID button_index, int8_t diff) {
    if (button_index == E1_ROT) {
        wrap_tester += diff;
        //wrap_tester = limit_range(wrap_tester, 0, 200);

        //_help_page_scroll_index += diff;
        //_help_page_scroll_index = limit_range(_help_page_scroll_index, 0, 100);
    }
}

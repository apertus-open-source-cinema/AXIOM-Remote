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

#ifndef DRAW_H
#define DRAW_H

#include <inttypes.h>
#include "globals.h"

//void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, GFXfont gfxFont);
//uint16_t get_string_width(const char* str, GFXfont gfxFont);
//void fill_round_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void drawRGBBitmap(int16_t x, int16_t y, const uint16_t *pcolors, int16_t w, int16_t h);
void draw_string(int16_t x, int16_t y, char* text, uint16_t color, uint16_t bg, GFXfont gfxFont, text_align align, uint16_t textblockwidth);

void fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void fill_round_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);

void fill_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

void clear_screen(uint16_t fill_color);
void fill_rect2(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

#endif //DRAW_H


#ifndef DRAW_H
#define DRAW_H

#include <inttypes.h>

#include "globals.h"
#include "gfxfont.h"

//void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, GFXfont gfxFont);
//uint16_t get_string_width(const char* str, GFXfont gfxFont);
//void fill_round_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void drawRGBBitmap(int16_t x, int16_t y, const uint16_t *pcolors, int16_t w, int16_t h);
void draw_string(int16_t x, int16_t y, char* text, uint16_t color, uint16_t bg, GFXfont gfxFont, text_align align, uint16_t textblockwidth);
void fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void start_framebuffer_transition(enum transition_animation transition_animation_type, uint8_t speed);

#endif //DRAW_H

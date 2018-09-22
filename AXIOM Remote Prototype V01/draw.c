/*	AXIOM Remote
 **
 **	Copyright (C) 2018 Herbert Poetzl, Sebastian Pichelhofer
 * 
 * code partly based on Adafruit ILI9341 TFT Displays written by Limor "ladyada" Fried for Adafruit Industries.
 **
 **	This program is free software; you can redistribute it and/or modify
 **    	it under the terms of the GNU General Public License 2 as published 
 **	by the Free Software Foundation.
 **
 **	Compile with -O6 for best experience
 */

#include <stdint.h>
#include <stdbool.h>
#include "glcdfont.c"
#include "gfxfont.h"
#include <string.h>
#include "definitions.h"
#include "main.h"

#ifndef DRAW_C
#define DRAW_C

/**************************************************************************/
/*!
   @brief    Draw a pixel to the canvas framebuffer
    @param   x   x coordinate
    @param   y   y coordinate
   @param    color 16-bit 5-6-5 Color to fill with
 */

/**************************************************************************/
void drawPixel(int16_t x, int16_t y, uint16_t color) {
    //origin shall be at the lower left corner
    _framebuffer[x][_height - y] = color;
}

/**************************************************************************/
/*!
   @brief    Write a line.  Bresenham's algorithm - thx wikpedia
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
 */

/**************************************************************************/
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            drawPixel(y0, x0, color);
        } else {
            drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly vertical line (this is often optimized in a subclass!)
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
 */

/**************************************************************************/
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    drawLine(x, y, x, (int16_t) y + h - 1, color);
}

/**************************************************************************/
/*!
   @brief  Fill a rectangle, includes code for SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    w  Width of rectangle
    @param    h  Height of rectangle
    @param    color 16-bit 5-6-5 Color to fill with
 */

/**************************************************************************/
void fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    /*int16_t x2 = x + w - 1, y2 = y + h - 1;
    if((x2 < 0) || (y2 < 0)) return;

    // Clip left/top
    if(x < 0) {
        x = 0;
        w = x2 + 1;
    }
    if(y < 0) {
        y = 0;
        h = y2 + 1;
    }

    // Clip right/bottom
    if(x2 >= _width)  w = _width  - x;
    if(y2 >= _height) h = _height - y;

    int32_t len = (int32_t)w * h;
    setAddrWindow(x, y, w, h);
    writeColor(color, len);*/
    int16_t i;
    for (i = x; i < x + w; i++) {
        drawFastVLine(i, y, h, color);
    }
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly horizontal line (this is often optimized in a subclass!)
    @param    x   Left-most x coordinate
    @param    y   Left-most y coordinate
    @param    w   Width in pixels
   @param    color 16-bit 5-6-5 Color to fill with
 */

/**************************************************************************/
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    drawLine(x, y, x + w - 1, y, color);
}

/**************************************************************************/
/*!
   @brief    Draw a circle outline
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to draw with
 */

/**************************************************************************/
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

/**************************************************************************/
/*!
   @brief    Quarter-circle drawer, used to do circles and roundrects
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing
    @param    color 16-bit 5-6-5 Color to draw with
 */

/**************************************************************************/
void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

/**************************************************************************/
/*!
   @brief    Quarter-circle drawer with fill, used to do circles and roundrects
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing
    @param    delta  Offset from center-point, used for round-rects
    @param    color 16-bit 5-6-5 Color to fill with
 */

/**************************************************************************/
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1) {
            drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x2) {
            drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }
}

/**************************************************************************/
/*!
   @brief    Draw a circle with filled color
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to fill with
 */

/**************************************************************************/
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    drawFastVLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

/**************************************************************************/
/*!
   @brief   Draw a rectangle with no fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    color 16-bit 5-6-5 Color to draw with
 */

/**************************************************************************/
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y + h - 1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x + w - 1, y, h, color);
}

/**************************************************************************/
/*!
   @brief   Draw a rounded rectangle with no fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    r   Radius of corner rounding
    @param    color 16-bit 5-6-5 Color to draw with
 */

/**************************************************************************/
void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    drawFastHLine(x + r, y, w - 2 * r, color); // Top
    drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    drawFastVLine(x, y + r, h - 2 * r, color); // Left
    drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right

    // draw four corners
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

/**************************************************************************/
/*!
   @brief   Draw a rounded rectangle with fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    r   Radius of corner rounding
    @param    color 16-bit 5-6-5 Color to draw/fill with
 */

/**************************************************************************/
void fill_round_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    fill_rect(x + r, y, w - 2 * r, h, color);

    // draw four corners
    fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color, no background)
    @param    size  Font magnification level, 1 is 'original' size
 */

/**************************************************************************/
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, GFXfont gfxFont) {

    c -= (uint8_t) pgm_read_byte(&gfxFont.first);
    GFXglyph *glyph = &(((GFXglyph *) pgm_read_pointer(&gfxFont.glyph))[c]);
    uint8_t *bitmap = (uint8_t *) pgm_read_pointer(&gfxFont.bitmap);

    uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
    uint8_t w = pgm_read_byte(&glyph->width);
    uint8_t h = pgm_read_byte(&glyph->height);
    int8_t xo = pgm_read_byte(&glyph->xOffset);
    int8_t yo = pgm_read_byte(&glyph->yOffset);
    uint8_t xx, yy, bits = 0, bit = 0;
    int16_t xo16 = 0, yo16 = 0;

    // Todo: Add character clipping here

    // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
    // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
    // has typically been used with the 'classic' font to overwrite old
    // screen contents with new data.  This ONLY works because the
    // characters are a uniform size; it's not a sensible thing to do with
    // proportionally-spaced fonts with glyphs of varying sizes (and that
    // may overlap).  To replace previously-drawn text when using a custom
    // font, use the getTextBounds() function to determine the smallest
    // rectangle encompassing a string, erase the area with fillRect(),
    // then draw new text.  This WILL infortunately 'blink' the text, but
    // is unavoidable.  Drawing 'background' pixels will NOT fix this,
    // only creates a new set of problems.  Have an idea to work around
    // this (a canvas object type for MCUs that can afford the RAM and
    // displays supporting setAddrWindow() and pushColors()), but haven't
    // implemented this yet.

    for (yy = 0; yy < h; yy++) {
        for (xx = 0; xx < w; xx++) {
            if (!(bit++ & 7)) {
                bits = pgm_read_byte(&bitmap[bo++]);
            }
            if (bits & 0x80) {
                drawPixel(x + xo + xx, y - yo - yy, color);
            }
            bits <<= 1;
        }
    }
}

/**************************************************************************/
/*!
   @brief  Draw RGB rectangle of data from RAM to a location on screen
   Adapted from https://github.com/PaulStoffregen/ILI9341_t3
   by Marc MERLIN. See examples/pictureEmbed to use this.
   5/6/2017: function name and arguments have changed for compatibility
   with current GFX library and to avoid naming problems in prior
   implementation.  Formerly drawBitmap() with arguments in different order.

    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    pcolors Pointer to 16-bit color data
    @param    w  Width of pcolors rectangle
    @param    h  Height of pcolors rectangle
 */
/**************************************************************************/
// Options for converting images to the format used here include:
//   http://www.rinkydinkelectronics.com/t_imageconverter565.php
// or
//  GIMP (https://www.gimp.org/) as follows:
//    1. File -> Export As
//    2. In Export Image dialog, use 'C source code (*.c)' as filetype.
//    3. Press export to get the export options dialog.
//    4. Type the desired variable name into the 'prefixed name' box.
//    5. Uncheck 'GLIB types (guint8*)'
//    6. Check 'Save as RGB565 (16-bit)'
//    7. Press export to save your image.
//  Assuming 'image_name' was typed in the 'prefixed name' box of step 4,
//  you can have to include the c file, then using the image can be done with:
//    tft.drawRGBBitmap(0, 0, image_name.pixel_data, image_name.width, image_name.height);
//  See also https://forum.pjrc.com/threads/35575-Export-for-ILI9341_t3-with-GIMP

void drawRGBBitmap(int16_t x, int16_t y, const uint16_t *pcolors, int16_t w, int16_t h) {
    //rotation fix
    int16_t temp = w;
    w = h;
    h = temp;

    int16_t x2, y2; // Lower-right coord
    if ((x >= _width) || // Off-edge right
            (y >= _height) || // " top
            ((x2 = (x + w - 1)) < 0) || // " left
            ((y2 = (y + h - 1)) < 0)) return; // " bottom

    int16_t bx1 = 0, by1 = 0, // Clipped top-left within bitmap
            saveW = w; // Save original bitmap width value
    if (x < 0) { // Clip left
        w += x;
        bx1 = -x;
        x = 0;
    }
    if (y < 0) { // Clip top
        h += y;
        by1 = -y;
        y = 0;
    }
    if (x2 >= _width) w = _width - x; // Clip right
    if (y2 >= _height) h = _height - y; // Clip bottom

    uint16_t draw_x;
    uint16_t draw_y;

    for (draw_x = 0; draw_x < w; draw_x++) {
        for (draw_y = 0; draw_y < h; draw_y++) {
            //drawPixel(draw_y+y, _height-x+w+draw_x, *pcolors++);
            drawPixel(draw_y + x, y + draw_x, *pcolors++);
            //drawPixel(draw_y+x, _height-y+w+draw_x, *pcolors++);
        }
    }
    /*pcolors += by1 * saveW + bx1; // Offset bitmap ptr to clipped top-left
    setAddrWindow(x, y, w, h); // Clipped area
    while(h--) { // For each (clipped) scanline...
      drawPixel(pcolors, w); // Push one (clipped) row
      pcolors += saveW; // Advance pointer by one full (unclipped) line
    }*/
}

void setCursor(int16_t x, int16_t y) {
    _cursor_x = x;
    _cursor_y = y;
}

/**************************************************************************/
/*!
    @brief    Helper to determine size of a character with current font/size.
       Broke this out as it's used by both the PROGMEM- and RAM-resident getTextBounds() functions.
    @param    c     The ascii character in question
    @param    x     Pointer to x location of character
    @param    y     Pointer to y location of character
    @param    minx  Minimum clipping value for X
    @param    miny  Minimum clipping value for Y
    @param    maxx  Maximum clipping value for X
    @param    maxy  Maximum clipping value for Y
 */

/**************************************************************************/
void getCharBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy, uint8_t textsize, GFXfont gfxFont) {
    bool wrap = false;
    if (c == '\n') { // Newline?
        *x = 0; // Reset x to zero, advance y by one line
        *y += textsize * (uint8_t) pgm_read_byte(&gfxFont.yAdvance);
    } else if (c != '\r') { // Not a carriage return; is normal char
        uint8_t first = pgm_read_byte(&gfxFont.first),
                last = pgm_read_byte(&gfxFont.last);
        if ((c >= first) && (c <= last)) { // Char present in this font?
            GFXglyph *glyph = &(((GFXglyph *) pgm_read_pointer(
                    &gfxFont.glyph))[c - first]);
            uint8_t gw = pgm_read_byte(&glyph->width),
                    gh = pgm_read_byte(&glyph->height),
                    xa = pgm_read_byte(&glyph->xAdvance);
            int8_t xo = pgm_read_byte(&glyph->xOffset),
                    yo = pgm_read_byte(&glyph->yOffset);
            if (wrap && ((*x + (((int16_t) xo + gw) * textsize)) > _width)) {
                *x = 0; // Reset x to zero, advance y by one line
                *y += textsize * (uint8_t) pgm_read_byte(&gfxFont.yAdvance);
            }
            int16_t ts = (int16_t) textsize,
                    x1 = *x + xo * ts,
                    y1 = *y + yo * ts,
                    x2 = x1 + gw * ts - 1,
                    y2 = y1 + gh * ts - 1;
            if (x1 < *minx) *minx = x1;
            if (y1 < *miny) *miny = y1;
            if (x2 > *maxx) *maxx = x2;
            if (y2 > *maxy) *maxy = y2;
            *x += xa * ts;
        }
    }
}
/**************************************************************************/
/*!
    @brief    Helper to calculate the x-offset (distance from cursor to left border of the character) in pixels to be drawn
    @param    c     The ascii character to measure
    @param    font    The font to be used
 */

/**************************************************************************/
uint16_t get_char_xoffset(const char c, GFXfont gfxFont) {

    uint8_t first = pgm_read_byte(&gfxFont.first);
    uint8_t last = pgm_read_byte(&gfxFont.last);

    if ((c >= first) && (c <= last)) { // Char present in this font?
        GFXglyph *glyph = &(((GFXglyph *) pgm_read_pointer(&gfxFont.glyph))[c - first]);
        uint8_t gw = pgm_read_byte(&glyph->width);
        uint8_t xa = pgm_read_byte(&glyph->xAdvance);
        int8_t xo = pgm_read_byte(&glyph->xOffset);

        return xo;
    }
    return 0;
}
/**************************************************************************/
/*!
    @brief    Helper to calculate the width in pixels of a string to be drawn
    @param    str     The ascii string to measure
    @param    font    The font to be used
 */

/**************************************************************************/
uint16_t get_string_width(const char* str, GFXfont gfxFont) {
    uint16_t width = 0;
    uint8_t gap;
    uint8_t xo1;
    bool first_letter = true;

    while (*str != 0) {
        uint8_t first = pgm_read_byte(&gfxFont.first);
        uint8_t last = pgm_read_byte(&gfxFont.last);

        if ((*str >= first) && (*str <= last)) { // Char present in this font?
            GFXglyph *glyph = &(((GFXglyph *) pgm_read_pointer(&gfxFont.glyph))[*str - first]);
            uint8_t gw = pgm_read_byte(&glyph->width);
            uint8_t xa = pgm_read_byte(&glyph->xAdvance);
            int8_t xo = pgm_read_byte(&glyph->xOffset);
            gap = xa - gw - xo;
            width += xa;

            if (first_letter) {
                xo1 = xo;
                first_letter = false;
            }

            *str++;
        }
    }

    // remove the gap after the last letter
    if (width > 0) {
        width -= gap;
    }
    // remove the xoffset before the first letter
    if (width > 0) {
        width -= xo1;
    }
    return width;
}
/**************************************************************************/
/*!
    @brief    Helper to determine size of a string with current font/size. Pass string and a cursor position, returns UL corner and W,H.
    @param    str     The ascii string to measure
    @param    x       The current cursor X
    @param    y       The current cursor Y
    @param    x1      The boundary X coordinate, set by function
    @param    y1      The boundary Y coordinate, set by function
    @param    w       The boundary width, set by function
    @param    h       The boundary height, set by function
 */

/**************************************************************************/
void get_text_bounds(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h, GFXfont gfxfont) {
    uint8_t c; // Current character

    *x1 = x;
    *y1 = y;
    *w = *h = 0;

    int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;

    while ((c = *str++))
        getCharBounds(c, &x, &y, &minx, &miny, &maxx, &maxy, 1, gfxfont);

    if (maxx >= minx) {
        *x1 = minx;
        *w = maxx - minx + 1;
    }
    if (maxy >= miny) {
        *y1 = miny;
        *h = maxy - miny + 1;
    }
}

void draw_string(int16_t x, int16_t y, char* text, uint16_t color, uint16_t bg, GFXfont gfxFont, textAlign align, uint16_t textblockwidth) {
    uint8_t first = pgm_read_byte(&gfxFont.first);
    uint8_t last = pgm_read_byte(&gfxFont.last);
    uint8_t length = strlen(text);

    _cursor_x = x;
    _cursor_y = y;

    uint16_t text_width = get_string_width(text, gfxFont);

    uint8_t i;
    for (i = 0; i < length; i++) {
        char c = text[i];

        if (c == 32) { // space " " character 
            _cursor_x += 10;
        }

        if ((c >= first) && (c <= last)) {
            GFXglyph *glyph = &(((GFXglyph *) pgm_read_pointer(&gfxFont.glyph))[c - first]);
            uint8_t w = pgm_read_byte(&glyph->width),
                    h = pgm_read_byte(&glyph->height);
            if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
                int16_t xo = (int8_t) pgm_read_byte(&glyph->xOffset); // sic
                
                if (i == 0) {
                    //first character doesn't need x offset
                    _cursor_x -= xo;
                }
                
                if (align == align_left) {

                    drawChar(_cursor_x + xo, _cursor_y, c, color, bg, gfxFont);
                    _cursor_x += (uint8_t) pgm_read_byte(&glyph->xAdvance);
                }
                if ((align == align_center) && (textblockwidth > 0)) {
                    drawChar(_cursor_x + xo - text_width / 2 + textblockwidth / 2, _cursor_y, c, color, bg, gfxFont);
                    _cursor_x += (uint8_t) pgm_read_byte(&glyph->xAdvance);
                }
                if ((align == align_right) && (textblockwidth > 0)) {
                    drawChar(_cursor_x + xo + textblockwidth - text_width, _cursor_y, c, color, bg, gfxFont);
                    _cursor_x += (uint8_t) pgm_read_byte(&glyph->xAdvance);
                }
            }
        }
    }

    /*if(text == '\n') {                        // Newline?
        cursor_x  = 0;                     // Reset x to zero,
        cursor_y += size * 8;          // advance y one line
    } else if(c != '\r') {                 // Ignore carriage returns
        if(wrap && ((cursor_x + size * 6) > _width)) { // Off right?
            cursor_x  = 0;                 // Reset x to zero,
            cursor_y += size * 8;      // advance y one line
        }
        drawChar(cursor_x, cursor_y, c, color, bg, size);
        cursor_x += size * 6;          // Advance x one char
    }*/
}

#endif /* DRAW_C */
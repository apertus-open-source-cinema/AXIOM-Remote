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

#ifndef UTILITY_H
#define UTILITY_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

static inline int8_t limit_range(int8_t in, int8_t min, int8_t max)
{
    if (in >= max) {
        return max;
    }
    if (in <= min) {
        return min;
    }
    return in;
}

/**************************************************************************/
/*!
    @brief  Pass 8-bit (each) R,G,B, get back 16-bit packed color
            This function converts 8-8-8 RGB data to 16-bit 5-6-5
    @param    red   Red 8 bit color
    @param    green Green 8 bit color
    @param    blue  Blue 8 bit color
    @return   Unsigned 16-bit down-sampled color in 5-6-5 format
 */

/**************************************************************************/
static inline uint16_t color565(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}

#endif /* UTILITY_C */

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

#ifndef PAGE_WB_HELP_H
#define PAGE_WB_HELP_H

#include "globals.h"

void draw_wb_help_page();
void init_wb_help_page();

void wb_help_page_knob_handler(ButtonID button_index, int8_t diff);

#endif //PAGE_WB_HELP_H


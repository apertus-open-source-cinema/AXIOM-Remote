
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

#ifndef PAGE_H
#define PAGE_H

#include "globals.h"

void init_pages();
void draw_page();

void main_page_button_press_handler(ButtonID button_index);
void main_page_button_release_handler(ButtonID button_index);

#endif //PAGE_H


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

#ifndef MENU_H
#define MENU_H

#include "globals.h"

void init_menus();
void draw_menu();

void main_menu_button_press_handler(ButtonID button_index);
void main_menu_button_release_handler(ButtonID button_index);

void main_menu_knob_handler(ButtonID button_index, int8_t diff);

void unselect_all_menu_items(enum menu_id_t current_menu);

void navigate_to_menu(enum menu_id_t target_menu);

#endif //MENU_H

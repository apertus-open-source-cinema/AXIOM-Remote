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

void init_menus();
void draw_menu();

void main_menu_button_press_handler(ButtonID button_index);
void main_menu_button_release_handler(ButtonID button_index);

#endif //MENU_H


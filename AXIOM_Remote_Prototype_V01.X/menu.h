#ifndef MENU_H
#define MENU_H

#include "globals.h"

void init_menus();
void draw_menu();

void main_menu_button_press_handler(ButtonID button_index);
void main_menu_button_release_handler(ButtonID button_index);

#endif //MENU_H

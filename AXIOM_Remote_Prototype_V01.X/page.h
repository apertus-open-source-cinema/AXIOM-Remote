#ifndef PAGE_H
#define PAGE_H

#include "globals.h"

void init_pages();
void draw_page();

void main_page_button_press_handler(ButtonID button_index);
void main_page_button_release_handler(ButtonID button_index);

#endif //PAGE_H

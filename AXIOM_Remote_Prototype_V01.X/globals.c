#include "globals.h"

//#include "FreeSans9pt7b.h"
//#include "FreeSans12pt7b.h"
//#include "FreeSans18pt7b.h"
//#include "FreeSans24pt7b.h"

//const uint16_t FRAMEBUFFER_WIDTH = 320;
//const uint16_t FRAMEBUFFER_HEIGHT = 240;

//uint16_t framebuffer[FRAMEBUFFER_WIDTH][FRAMEBUFFER_WIDTH];

GFXfont _FreeSans9pt7b;
GFXfont _FreeSans12pt7b;
GFXfont _FreeSans18pt7b;
GFXfont _FreeSans24pt7b;

//GFXfont _FreeSans9pt7b = FreeSans9pt7b;
//GFXfont _FreeSans12pt7b = FreeSans12pt7b;
//GFXfont _FreeSans18pt7b = FreeSans18pt7b;
//GFXfont _FreeSans24pt7b = FreeSans24pt7b;

enum menu_id_t current_menu = MENU_MAIN;
enum page_id_t current_page = PAGE_HOME;

page_t main_page[3];

bool btn_E1_pressed = false;
bool btn_P7_pressed = false;
bool btn_P8_pressed = false;

uint8_t menu_offset = 0;

uint16_t _cursor_x = 0;
uint16_t _cursor_y = 0;

white_balance white_balance_parameter;

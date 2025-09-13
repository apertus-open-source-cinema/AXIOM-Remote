/*******************************************************************************
 * Size: 10 px
 * Bpp: 2
 * Opts: --bpp 2 --size 10 --no-compress --font TitilliumWeb-SemiBold.ttf --range 32-127 --format lvgl -o titillium_10.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef TITILLIUM_10
#define TITILLIUM_10 1
#endif

#if TITILLIUM_10

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0x33, 0x33, 0x11, 0x70,

    /* U+0022 "\"" */
    0x69, 0x69, 0x0,

    /* U+0023 "#" */
    0x32, 0x43, 0x24, 0xbf, 0xd3, 0x24, 0xbf, 0xd3,
    0x24, 0x32, 0x40,

    /* U+0024 "$" */
    0x0, 0x0, 0x20, 0x2f, 0x87, 0x50, 0x79, 0x1,
    0xf4, 0x9, 0xc0, 0x8c, 0x3f, 0x40, 0x80, 0x0,
    0x0,

    /* U+0025 "%" */
    0x0, 0xb, 0x60, 0xd6, 0x6, 0x50, 0x8, 0x0,
    0x98, 0xb, 0x51, 0x2c, 0x0, 0x0,

    /* U+0026 "&" */
    0x1f, 0x80, 0x93, 0x1, 0xa8, 0xf, 0xc8, 0xa2,
    0xf2, 0x83, 0xc2, 0xf5, 0x80,

    /* U+0027 "'" */
    0x66, 0x0,

    /* U+0028 "(" */
    0x14, 0xc3, 0x1c, 0xa2, 0x87, 0xc, 0x24,

    /* U+0029 ")" */
    0x51, 0xc3, 0xd, 0x34, 0xd3, 0xc, 0x60,

    /* U+002A "*" */
    0x0, 0x18, 0x3e, 0x58, 0x0,

    /* U+002B "+" */
    0x9, 0x0, 0x90, 0x7f, 0xc0, 0x90, 0x9, 0x0,

    /* U+002C "," */
    0x16, 0x90,

    /* U+002D "-" */
    0x7e,

    /* U+002E "." */
    0x17,

    /* U+002F "/" */
    0x3, 0x1, 0x80, 0xc0, 0x70, 0x24, 0xc, 0xa,
    0x0, 0x0,

    /* U+0030 "0" */
    0x2f, 0x47, 0x1c, 0xa0, 0xc9, 0xd, 0xa0, 0xc7,
    0x1c, 0x2f, 0x40,

    /* U+0031 "1" */
    0x1, 0x7, 0xc2, 0x30, 0xc, 0x3, 0x0, 0xc0,
    0x30,

    /* U+0032 "2" */
    0x3f, 0x0, 0xa0, 0x18, 0xd, 0xd, 0xd, 0x7,
    0xfc,

    /* U+0033 "3" */
    0x3f, 0x40, 0x70, 0x28, 0xbd, 0x1, 0xc0, 0x77,
    0xf4,

    /* U+0034 "4" */
    0x4, 0x0, 0xc0, 0x28, 0x3, 0x24, 0x62, 0x4b,
    0xfc, 0x2, 0x40,

    /* U+0035 "5" */
    0x3f, 0xc2, 0x0, 0x20, 0x7, 0xf4, 0x1, 0xc0,
    0x1c, 0x7f, 0x40,

    /* U+0036 "6" */
    0x1f, 0xc3, 0x0, 0x60, 0xb, 0xf8, 0x60, 0xc3,
    0xc, 0x2f, 0x40,

    /* U+0037 "7" */
    0x7f, 0xc0, 0x70, 0x34, 0x1c, 0x9, 0x3, 0x0,
    0x40,

    /* U+0038 "8" */
    0x2f, 0x4b, 0xc, 0x70, 0xc3, 0xf8, 0xa0, 0xca,
    0xc, 0x3f, 0x80,

    /* U+0039 "9" */
    0x2f, 0x7, 0x1c, 0xa0, 0xca, 0xc, 0x2f, 0xc0,
    0x1c, 0x7f, 0x0,

    /* U+003A ":" */
    0x7, 0x1, 0x70,

    /* U+003B ";" */
    0x0, 0xc1, 0x4, 0x62, 0x40,

    /* U+003C "<" */
    0x0, 0x46, 0xd7, 0x40, 0x79, 0x1, 0x80,

    /* U+003D "=" */
    0x7f, 0xc0, 0x7, 0xfc,

    /* U+003E ">" */
    0x10, 0xb, 0x80, 0x2c, 0x78, 0x20, 0x0,

    /* U+003F "?" */
    0x7e, 0x0, 0xc0, 0x70, 0x70, 0x24, 0x4, 0x2,
    0x40,

    /* U+0040 "@" */
    0x7, 0xfd, 0x2, 0xc0, 0x28, 0x31, 0xfc, 0xc6,
    0x30, 0xcd, 0xa3, 0xc, 0x96, 0x30, 0xcc, 0x72,
    0xfb, 0x83, 0x80, 0x0, 0xb, 0xf4, 0x0,

    /* U+0041 "A" */
    0xf, 0x1, 0xa4, 0x25, 0x83, 0xc, 0x30, 0xc7,
    0xfd, 0x90, 0x60,

    /* U+0042 "B" */
    0x3f, 0xc3, 0xd, 0x30, 0xd3, 0xfc, 0x30, 0xa3,
    0xa, 0x3f, 0xc0,

    /* U+0043 "C" */
    0x2f, 0xc3, 0x0, 0x60, 0x6, 0x0, 0x60, 0x3,
    0x0, 0x2f, 0xc0,

    /* U+0044 "D" */
    0x3f, 0x83, 0xa, 0x30, 0x73, 0x7, 0x30, 0x73,
    0xa, 0x3f, 0x80,

    /* U+0045 "E" */
    0x3f, 0xc3, 0x0, 0x30, 0x3, 0xf4, 0x30, 0x3,
    0x0, 0x3f, 0xc0,

    /* U+0046 "F" */
    0x3f, 0xc3, 0x0, 0x30, 0x3, 0xf4, 0x30, 0x3,
    0x0, 0x30, 0x0,

    /* U+0047 "G" */
    0x1f, 0xd3, 0x0, 0x60, 0x6, 0x1e, 0x60, 0xa3,
    0xa, 0x1f, 0xd0,

    /* U+0048 "H" */
    0x30, 0x33, 0x3, 0x30, 0x33, 0xff, 0x30, 0x33,
    0x3, 0x30, 0x30,

    /* U+0049 "I" */
    0x33, 0x33, 0x33, 0x30,

    /* U+004A "J" */
    0x30, 0xc3, 0xc, 0x30, 0xce, 0x0,

    /* U+004B "K" */
    0x30, 0xd3, 0x1c, 0x33, 0x43, 0xf0, 0x33, 0x43,
    0x1c, 0x30, 0xd0,

    /* U+004C "L" */
    0x30, 0xc, 0x3, 0x0, 0xc0, 0x30, 0xc, 0x3,
    0xf8,

    /* U+004D "M" */
    0x3c, 0x1e, 0x3c, 0x2e, 0x39, 0x36, 0x36, 0x36,
    0x33, 0x96, 0x32, 0xc6, 0x31, 0x86,

    /* U+004E "N" */
    0x3c, 0x30, 0xf0, 0xc3, 0x63, 0xc, 0xcc, 0x32,
    0x70, 0xc6, 0xc3, 0xf, 0x0,

    /* U+004F "O" */
    0x1f, 0x80, 0xc2, 0x86, 0x3, 0x18, 0xc, 0x60,
    0x30, 0xc2, 0x81, 0xfc, 0x0,

    /* U+0050 "P" */
    0x3f, 0x83, 0x9, 0x30, 0xa3, 0xd, 0x3f, 0x83,
    0x0, 0x30, 0x0,

    /* U+0051 "Q" */
    0x1f, 0x80, 0xc2, 0x86, 0x3, 0x18, 0xc, 0x60,
    0x30, 0xc2, 0x81, 0xfc, 0x0, 0x38, 0x0, 0x0,

    /* U+0052 "R" */
    0x3f, 0x83, 0xa, 0x30, 0x63, 0xa, 0x3f, 0xc3,
    0xc, 0x30, 0x90,

    /* U+0053 "S" */
    0x2f, 0x8a, 0x0, 0x70, 0x2, 0xf4, 0x1, 0xc0,
    0x1c, 0x7f, 0x40,

    /* U+0054 "T" */
    0xff, 0xc0, 0xc0, 0xc, 0x0, 0xc0, 0xc, 0x0,
    0xc0, 0xc, 0x0,

    /* U+0055 "U" */
    0x70, 0x77, 0x7, 0x70, 0x77, 0x7, 0x70, 0x73,
    0xa, 0x1f, 0xc0,

    /* U+0056 "V" */
    0x90, 0xa6, 0xd, 0x30, 0xc3, 0xc, 0x25, 0x81,
    0xa4, 0xf, 0x0,

    /* U+0057 "W" */
    0x90, 0xd1, 0xd8, 0x78, 0xa7, 0x2a, 0x24, 0xcc,
    0xcc, 0x33, 0x33, 0xa, 0x8a, 0xc2, 0xd1, 0xe0,

    /* U+0058 "X" */
    0xa0, 0xc3, 0x18, 0x1b, 0x0, 0xe0, 0x1f, 0x3,
    0x28, 0xa0, 0xc0,

    /* U+0059 "Y" */
    0xa0, 0xc3, 0x28, 0x2b, 0x0, 0xe0, 0xd, 0x0,
    0xd0, 0xd, 0x0,

    /* U+005A "Z" */
    0xbf, 0xc0, 0xa0, 0x70, 0x34, 0x28, 0x1c, 0xb,
    0xfc,

    /* U+005B "[" */
    0x7c, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70,
    0x7c,

    /* U+005C "\\" */
    0x50, 0xc, 0x2, 0x40, 0x70, 0xd, 0x1, 0xc0,
    0x30, 0x0,

    /* U+005D "]" */
    0xb8, 0x61, 0x86, 0x18, 0x61, 0x86, 0xb8,

    /* U+005E "^" */
    0x4, 0x1, 0xf0, 0x37, 0x47, 0x1c,

    /* U+005F "_" */
    0x3f, 0xc0,

    /* U+0060 "`" */
    0x0, 0x3c, 0x0,

    /* U+0061 "a" */
    0x3f, 0x0, 0xd7, 0xb6, 0x89, 0x7e, 0x80,

    /* U+0062 "b" */
    0x60, 0x18, 0x7, 0xf5, 0xc7, 0x60, 0xd8, 0x77,
    0xf4,

    /* U+0063 "c" */
    0x3f, 0x18, 0xa, 0x1, 0xc0, 0x3f, 0x0,

    /* U+0064 "d" */
    0x1, 0x80, 0x62, 0xf9, 0xc6, 0xa1, 0x98, 0x63,
    0xf8,

    /* U+0065 "e" */
    0x2f, 0x18, 0x6b, 0xf9, 0x80, 0x3f, 0x40,

    /* U+0066 "f" */
    0x3d, 0x30, 0xbd, 0x30, 0x30, 0x30, 0x30,

    /* U+0067 "g" */
    0x3f, 0xc6, 0x24, 0x3f, 0x3, 0x0, 0x3f, 0x86,
    0xc, 0x3f, 0x40,

    /* U+0068 "h" */
    0x70, 0x1c, 0x7, 0xf5, 0xc7, 0x70, 0xdc, 0x37,
    0xc,

    /* U+0069 "i" */
    0x70, 0x77, 0x77, 0x70,

    /* U+006A "j" */
    0x1c, 0x1, 0xc7, 0x1c, 0x71, 0x8a, 0x30, 0x0,

    /* U+006B "k" */
    0x70, 0x1c, 0x7, 0x35, 0xdc, 0x7d, 0x1c, 0xc7,
    0x28,

    /* U+006C "l" */
    0x77, 0x77, 0x77, 0x70,

    /* U+006D "m" */
    0x7f, 0xbd, 0x71, 0x86, 0x71, 0xc7, 0x71, 0xc7,
    0x71, 0xc7,

    /* U+006E "n" */
    0x7f, 0x5c, 0x77, 0xd, 0xc3, 0x70, 0xc0,

    /* U+006F "o" */
    0x2f, 0x18, 0x6a, 0xd, 0x86, 0x2f, 0x0,

    /* U+0070 "p" */
    0x7f, 0x5c, 0x77, 0xd, 0xc7, 0x7f, 0x5c, 0x7,
    0x0,

    /* U+0071 "q" */
    0x2f, 0x98, 0x6a, 0x19, 0x86, 0x3f, 0x80, 0x60,
    0x18,

    /* U+0072 "r" */
    0x0, 0x7d, 0x74, 0x70, 0x70, 0x70,

    /* U+0073 "s" */
    0x3f, 0x28, 0x3, 0xe0, 0xd, 0x7f, 0x0,

    /* U+0074 "t" */
    0x10, 0x30, 0xbd, 0x30, 0x30, 0x30, 0x2d,

    /* U+0075 "u" */
    0x61, 0xd8, 0x76, 0x1d, 0xc7, 0x3f, 0xc0,

    /* U+0076 "v" */
    0x92, 0x98, 0xd3, 0x30, 0xdc, 0x2e, 0x0,

    /* U+0077 "w" */
    0x93, 0x8d, 0x63, 0x8c, 0x36, 0xcc, 0x39, 0xa8,
    0x3c, 0x74,

    /* U+0078 "x" */
    0xa3, 0xe, 0x82, 0xc0, 0xe8, 0xa3, 0x0,

    /* U+0079 "y" */
    0x91, 0x98, 0x93, 0x30, 0xdc, 0x2e, 0x3, 0x0,
    0xc0,

    /* U+007A "z" */
    0xbf, 0x2, 0x81, 0xc0, 0xc0, 0xbf, 0x0,

    /* U+007B "{" */
    0x4, 0x28, 0x34, 0x34, 0xb0, 0x30, 0x24, 0x24,
    0x1c,

    /* U+007C "|" */
    0x77, 0x77, 0x77, 0x77, 0x70,

    /* U+007D "}" */
    0x50, 0x30, 0x24, 0x34, 0x1d, 0x28, 0x34, 0x34,
    0xa0,

    /* U+007E "~" */
    0x38, 0x41, 0xe0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 35, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 43, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 63, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 7, .adv_w = 90, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 18, .adv_w = 90, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 35, .adv_w = 90, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 49, .adv_w = 111, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 36, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 64, .adv_w = 46, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 71, .adv_w = 46, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 78, .adv_w = 68, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 83, .adv_w = 90, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 39, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 93, .adv_w = 67, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 94, .adv_w = 38, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 71, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 105, .adv_w = 90, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 116, .adv_w = 90, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 90, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 134, .adv_w = 90, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 143, .adv_w = 90, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 154, .adv_w = 90, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 165, .adv_w = 90, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 176, .adv_w = 90, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 185, .adv_w = 90, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 196, .adv_w = 90, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 38, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 210, .adv_w = 43, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 215, .adv_w = 90, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 90, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 226, .adv_w = 90, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 71, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 242, .adv_w = 156, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 265, .adv_w = 96, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 276, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 287, .adv_w = 87, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 298, .adv_w = 103, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 309, .adv_w = 88, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 320, .adv_w = 85, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 342, .adv_w = 108, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 353, .adv_w = 43, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 357, .adv_w = 47, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 363, .adv_w = 93, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 374, .adv_w = 76, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 383, .adv_w = 136, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 397, .adv_w = 110, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 410, .adv_w = 105, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 423, .adv_w = 95, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 105, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 450, .adv_w = 99, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 461, .adv_w = 87, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 472, .adv_w = 84, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 483, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 494, .adv_w = 95, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 505, .adv_w = 146, .box_w = 9, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 521, .adv_w = 90, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 532, .adv_w = 88, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 543, .adv_w = 85, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 552, .adv_w = 55, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 561, .adv_w = 75, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 571, .adv_w = 55, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 578, .adv_w = 90, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 584, .adv_w = 100, .box_w = 6, .box_h = 1, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 586, .adv_w = 41, .box_w = 4, .box_h = 3, .ofs_x = -1, .ofs_y = 5},
    {.bitmap_index = 589, .adv_w = 81, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 596, .adv_w = 86, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 605, .adv_w = 71, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 612, .adv_w = 86, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 621, .adv_w = 81, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 628, .adv_w = 55, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 635, .adv_w = 84, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 646, .adv_w = 87, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 655, .adv_w = 39, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 659, .adv_w = 39, .box_w = 3, .box_h = 10, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 667, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 676, .adv_w = 40, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 680, .adv_w = 134, .box_w = 8, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 690, .adv_w = 87, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 697, .adv_w = 85, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 704, .adv_w = 86, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 713, .adv_w = 86, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 722, .adv_w = 58, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 728, .adv_w = 75, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 735, .adv_w = 57, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 742, .adv_w = 87, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 749, .adv_w = 79, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 756, .adv_w = 123, .box_w = 8, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 766, .adv_w = 74, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 773, .adv_w = 79, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 782, .adv_w = 73, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 789, .adv_w = 57, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 798, .adv_w = 40, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 803, .adv_w = 57, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 812, .adv_w = 90, .box_w = 5, .box_h = 2, .ofs_x = 0, .ofs_y = 2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 1, 0, 0, 0, 2,
    1, 3, 4, 5, 0, 6, 7, 6,
    8, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 9, 9, 0, 0, 0,
    0, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 18, 19, 20, 21, 18, 18,
    22, 23, 22, 24, 25, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 0, 0,
    0, 0, 35, 36, 37, 38, 39, 40,
    41, 42, 38, 38, 43, 38, 42, 42,
    44, 36, 45, 46, 47, 48, 45, 49,
    50, 51, 52, 53, 54, 0, 55, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 0, 1, 0, 0, 0, 2,
    1, 3, 4, 5, 0, 6, 7, 6,
    8, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 9, 9, 0, 0, 0,
    10, 11, 12, 13, 14, 13, 13, 13,
    15, 13, 13, 16, 13, 13, 13, 13,
    15, 13, 15, 13, 17, 18, 19, 20,
    21, 22, 23, 24, 0, 25, 26, 0,
    0, 0, 27, 28, 29, 30, 29, 31,
    32, 0, 0, 33, 0, 0, 34, 34,
    29, 34, 30, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 0, 44, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, -4, 0, 0, 0, -22, -11, -10,
    0, 0, -2, -6, 0, 0, 0, -3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, -3, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -7, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -6, 0, -4,
    -3, 0, -8, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, -1, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -3, -3, -1, 0,
    3, -1, -1, 0, -2, -1, -2, 0,
    -1, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -5, 0, 0, 0, -2,
    0, 1, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, -3, -3, 0, -2,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -22, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, -1, -1, 0, 0, -10, -2, -7,
    -5, 0, -12, 0, 0, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 0, -3,
    0, -4, -4, 0, -5, 0, 0, 0,
    -11, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, -4,
    -4, -10, 0, -4, -3, -6, -11, -4,
    0, 0, -1, 0, 0, 0, -2, 0,
    0, 0, 0, -2, 0, -2, 0, -4,
    -2, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -47, 0, 0, 0, -6,
    0, -1, -2, -3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, 0,
    -6, -6, 0, -5, 0, -3, -4, 0,
    -3, -2, -1, 0, -2, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, -2, 0, 0, -6, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, -4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -6, 0, 0, 0, -5, 0, -1, 0,
    0, -3, 0, 0, 0, -2, -2, 0,
    -1, -7, -1, -4, -3, 0, -8, 0,
    -7, -2, 0, 0, -1, -1, -1, -1,
    0, 0, 0, -2, 0, -3, -2, 0,
    -3, 0, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, -1, 0, -1, 0, -2,
    0, -2, -4, 0, -2, -3, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, -4, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, -1, -1, -2,
    0, 0, 0, 0, -1, -2, -2, 0,
    -1, 0, 0, 0, 0, 0, 0, -2,
    0, -1, 0, -2, 0, -1, 0, -2,
    0, 0, 0, -2, 0, -2, 0, -2,
    0, -3, -4, 0, -2, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    0, 0, 0, 0, 0, 0, -3, 0,
    0, 0, 0, 0, 0, -1, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, -1, 0, -2,
    0, 0, 0, 0, 0, -1, -1, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, -9, -1, -6, 0, 0, 0, -4,
    0, -1, -1, -2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, 0,
    -3, -3, -1, -3, 0, -3, -3, 0,
    -2, -1, -1, -2, -1, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, -1, 0, 0, -3, 0,
    -1, 0, 0, 0, 0, 0, -1, -1,
    0, 0, 0, 0, 0, -1, -1, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, -1, -1, -1, 0, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, -1, -1, -1,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -5, 0, 0, 0, 0, 0,
    0, -1, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    -3, -2, -1, -2, 0, -1, 0, -2,
    -2, -3, -4, 0, -3, 0, 0, 0,
    -13, 0, 0, 0, -13, 0, -9, 0,
    0, -1, 0, 0, 0, -1, -2, 0,
    0, -13, -1, -9, -7, 0, -14, 0,
    -11, -1, 0, 0, 0, 0, -1, -1,
    0, 0, 0, -2, 0, -6, -4, 0,
    -6, 0, 0, -1, 0, 0, 0, -2,
    0, -1, 0, -2, 0, 0, 0, -2,
    0, 0, 0, -2, 0, -2, 0, -2,
    0, -3, -4, 0, -2, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    0, 0, 0, -1, 0, -11, -1, -6,
    0, 0, 0, -4, 0, 0, 0, -3,
    0, 0, 0, -1, 0, -3, -3, 0,
    -1, -3, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, -1, 0, -2,
    0, -1, -4, 0, -2, -1, -1, 0,
    -1, -1, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, -1,
    0, 0, 0, -1, 0, -1, -2, 0,
    0, 0, 0, 0, 0, 0, -1, -1,
    0, 0, 0, -1, 0, -1, -1, -1,
    -1, 0, 0, 0, 0, -2, 0, 0,
    0, -9, -10, -8, -8, 0, -3, -7,
    0, -1, -2, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -13, 0,
    -13, -12, -2, -14, 0, -11, -12, -3,
    -11, -9, -9, -9, -9, -12, 0, 0,
    0, 0, 0, 0, 0, -2, 0, -2,
    0, 0, 0, -1, 0, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -1, -1, -1, -2,
    0, -1, -1, 0, -1, 0, 0, 0,
    0, -1, 0, 0, 0, -3, 0, 0,
    0, -7, -4, -7, -2, 0, -2, -4,
    0, -2, -2, -3, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, 0,
    -5, -5, -1, -5, 0, -4, -4, 0,
    -3, -1, -1, -1, -1, -2, 0, 0,
    0, 0, 0, 0, 0, -5, -3, -4,
    0, 0, 0, -3, 0, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -2, 0, -3, -3, 0, -4,
    0, -2, -2, 0, -2, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, 0, -6, 0, 0, 0, 0, 0,
    0, -3, -3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    -4, -3, -1, -3, 0, -1, 0, -2,
    -3, -4, -4, 0, -4, 0, 0, 0,
    0, -5, 0, 0, 1, -12, -11, -10,
    -6, 0, -6, -8, 0, -4, -5, -4,
    -3, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -10, 0, -11, -11, -3, -12,
    0, -10, -11, -3, -9, -6, -7, -6,
    -6, -8, 0, 0, 0, 0, 0, 0,
    0, 0, -4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, -1, -1, -2, 0, -1, 0, 0,
    -1, -1, -2, 0, -1, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, -3, -4, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -4, 0, -5, -5, -2, 0,
    1, -2, -2, -3, -4, -4, -4, -1,
    -4, -1, -3, 0, -11, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, -2, 0, -1, -9, -3, -7,
    -5, 0, -11, 0, 0, 0, 0, 0,
    -1, 0, -1, 0, 0, 0, 0, -2,
    0, -4, -3, 0, -4, 0, 0, 0,
    -1, 0, 0, 0, -1, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, -12, -1, -4, -3, 0, -10, 0,
    -6, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    -1, 0, 0, -1, -2, 0, 0, -3,
    -1, 0, 0, 0, 0, -3, 0, -1,
    -1, 0, 0, -3, -1, -12, -1, -5,
    -3, -3, -11, -1, -6, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, 0, -2, -1, 0, 0, -4,
    0, 0, 0, 0, 0, 0, -4, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, -14, 0, -2, -1, 0, -8, 0,
    -3, -1, 0, 0, -1, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, -1, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, -1,
    0, -13, -1, -4, -3, -1, -13, -1,
    -6, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    -1, 0, 0, -3, 0, -1, 0, 0,
    0, -6, -6, -4, 0, 0, 0, -4,
    0, 0, 0, -2, 0, -2, 0, 0,
    0, -1, 0, -1, 0, 0, 0, 0,
    -1, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -10, 0, -1, 0, 0, -5, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, -1,
    -1, 0, 0, 0, 0, -3, 0, 0,
    0, 0, 0, -1, -1, -13, -1, -4,
    -3, 0, -11, -1, -6, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, -1, 0, -1, 0, 0, -2,
    0, 0, 0, 0, 0, 0, -5, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -10, -1, -1, 0, 0, -6, 0,
    -2, -1, 0, 0, -2, -1, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, -2, 0, 0, -3,
    -1, 0, 0, 0, 0, -3, 0, -1,
    -1, 0, 0, -3, -1, -13, -1, -5,
    -3, -4, -11, -1, -6, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, 0, -2, -1, 0, 0, -4,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, -1,
    0, -11, -1, -4, -2, -1, -10, -1,
    -4, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, -1, 0, -1,
    0, -8, -6, -5, 0, 0, 0, -4,
    0, 0, 0, -3, 0, -9, 0, 0,
    0, -4, -4, -2, -1, -4, 0, 0,
    -1, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    0, 0, 0, -2, 0, 0, -1, 0,
    0, -1, 0, 0, 0, 0, 0, -1,
    0, -12, 0, -3, -2, -1, -8, 0,
    -4, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    -1, 0, 0, -3, 0, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -9, 0, -1,
    0, 0, -5, 0, -2, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, -1, 0, -4, -2, -3,
    0, -1, 0, -3, 0, 0, 0, -3,
    0, -9, 0, -1, 0, -4, -6, -2,
    -2, -4, -1, 0, -1, -1, 0, -1,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, -3, 0, 0, 0, -2,
    0, -4, 0, -2, 0, -1, 0, -2,
    -1, 0, 0, -3, 0, -9, 0, -1,
    0, -4, -7, -2, -2, -4, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    0, 0, 0, 0, 0, 0, -4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -9, 0, -1, 0, 0, -6, 0,
    -2, -1, 0, 0, -2, -2, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, -5, -2, -3, 0, -1, 0, -3,
    0, 0, 0, -3, 0, -9, 0, -1,
    0, -4, -6, -2, -2, -4, -1, 0,
    -1, -1, 0, -1, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    0, 0, 0, 0, 0, 0, -4, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, -12, -1, -2, -1, 0, -8, 0,
    -3, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, -3, -3, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -3, 0,
    -4, -4, -1, 0, 3, -2, -2, -1,
    -4, -3, -3, -1, -3, -1, -2, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 55,
    .right_class_cnt     = 44,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 2,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

extern const lv_font_t lv_font_montserrat_10;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t titillium_10 = {
#else
lv_font_t titillium_10 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 12,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &lv_font_montserrat_10,
#endif
    .user_data = NULL,
};



#endif /*#if TITILLIUM_10*/


/*******************************************************************************
 * Size: 12 px
 * Bpp: 2
 * Opts: --bpp 2 --size 12 --no-compress --font TitilliumWeb-SemiBold.ttf --range 32-127 --format lvgl -o titillium_12.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef TITILLIUM_12
#define TITILLIUM_12 1
#endif

#if TITILLIUM_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0x34, 0xd3, 0xc, 0x30, 0x2, 0xd,

    /* U+0022 "\"" */
    0x73, 0x1c, 0xc2, 0x20,

    /* U+0023 "#" */
    0x28, 0xd0, 0xa3, 0x4f, 0xff, 0x4a, 0x34, 0x28,
    0xd2, 0xff, 0xd2, 0x8d, 0xa, 0x34,

    /* U+0024 "$" */
    0x0, 0x40, 0x5, 0x1, 0xfe, 0xd, 0x90, 0x33,
    0x0, 0xfd, 0x0, 0x7e, 0x0, 0x9c, 0x16, 0xb0,
    0xff, 0x0, 0x80, 0x0, 0x0,

    /* U+0025 "%" */
    0x0, 0x1, 0xe3, 0x9, 0xd8, 0x1e, 0x90, 0x3,
    0x0, 0x8, 0x0, 0x9b, 0x3, 0x69, 0x8, 0xf0,
    0x0, 0x0,

    /* U+0026 "&" */
    0xb, 0xd0, 0x7, 0x2c, 0x1, 0xc7, 0x0, 0x3f,
    0x10, 0x76, 0xcc, 0x2c, 0x2f, 0x7, 0x47, 0xd0,
    0xbf, 0x48,

    /* U+0027 "'" */
    0x77, 0x20,

    /* U+0028 "(" */
    0x4, 0x1c, 0x38, 0x34, 0x70, 0x70, 0x70, 0x70,
    0x34, 0x38, 0x1c,

    /* U+0029 ")" */
    0x1, 0xc3, 0x4e, 0x2c, 0xb2, 0xcb, 0x38, 0xd7,
    0x0,

    /* U+002A "*" */
    0x0, 0xa, 0x42, 0xf4, 0xa4, 0x0, 0x0,

    /* U+002B "+" */
    0x3, 0x0, 0xc, 0x0, 0x30, 0x1f, 0xfc, 0x7,
    0x0, 0xc, 0x0,

    /* U+002C "," */
    0x31, 0xca, 0x0,

    /* U+002D "-" */
    0x7f, 0x40, 0x0,

    /* U+002E "." */
    0x21, 0xc0,

    /* U+002F "/" */
    0x0, 0x0, 0xb0, 0x34, 0x1c, 0xe, 0x3, 0x2,
    0x80, 0xd0, 0x70, 0x0, 0x0,

    /* U+0030 "0" */
    0x1f, 0xc0, 0xd2, 0xc7, 0x3, 0x2c, 0xd, 0xb0,
    0x36, 0xc0, 0xc3, 0x4b, 0x7, 0xf0,

    /* U+0031 "1" */
    0xa, 0x7f, 0x97, 0x7, 0x7, 0x7, 0x7, 0x7,

    /* U+0032 "2" */
    0x3f, 0xc1, 0x1e, 0x0, 0xa0, 0xd, 0x3, 0x80,
    0xe0, 0x3c, 0x3, 0xff,

    /* U+0033 "3" */
    0x3f, 0xc1, 0xe, 0x0, 0xe1, 0xfc, 0x0, 0xf0,
    0x7, 0x10, 0xe3, 0xfc,

    /* U+0034 "4" */
    0xb, 0x0, 0x34, 0x1, 0xc0, 0xe, 0x34, 0x30,
    0xd2, 0xff, 0xc0, 0xd, 0x0, 0x34,

    /* U+0035 "5" */
    0x3f, 0xf0, 0xd0, 0x3, 0x0, 0xf, 0xf4, 0x14,
    0xb0, 0x1, 0xc1, 0xb, 0xf, 0xf0,

    /* U+0036 "6" */
    0xf, 0xf0, 0xe0, 0x7, 0x0, 0x1f, 0xf4, 0x74,
    0xb1, 0xc0, 0xc3, 0x8b, 0x7, 0xf4,

    /* U+0037 "7" */
    0x3f, 0xf0, 0xb, 0x0, 0xd0, 0x1c, 0x3, 0x80,
    0x70, 0xe, 0x0, 0xc0,

    /* U+0038 "8" */
    0x2f, 0xd1, 0xd2, 0xc7, 0x7, 0xf, 0xf8, 0x74,
    0xb2, 0xc0, 0xd7, 0x4b, 0xb, 0xf4,

    /* U+0039 "9" */
    0x1f, 0x81, 0xd3, 0x8b, 0x7, 0x1c, 0x1c, 0x3f,
    0xf0, 0x1, 0xc0, 0xe, 0xf, 0xe0,

    /* U+003A ":" */
    0x70, 0x80, 0x8, 0x70,

    /* U+003B ";" */
    0x20, 0xd0, 0x0, 0x35, 0xc6, 0x0,

    /* U+003C "<" */
    0x0, 0x10, 0x6d, 0x3d, 0x3, 0xd0, 0x7, 0xd0,
    0x5,

    /* U+003D "=" */
    0x7f, 0xf0, 0x0, 0x7f, 0xf0, 0x0,

    /* U+003E ">" */
    0x40, 0x2e, 0x0, 0xb8, 0x2f, 0xb9, 0x20, 0x0,

    /* U+003F "?" */
    0x7f, 0x40, 0xb0, 0x2c, 0x2d, 0x1c, 0x1, 0x1,
    0x80, 0x70,

    /* U+0040 "@" */
    0x2, 0xff, 0x80, 0xe, 0x41, 0xb4, 0x38, 0xbf,
    0x6c, 0x31, 0xd3, 0x4c, 0x72, 0xc3, 0x4c, 0x72,
    0xc3, 0x4c, 0x71, 0xc7, 0x5c, 0x70, 0xbe, 0xf4,
    0x38, 0x0, 0x0, 0x1e, 0x40, 0x0, 0x2, 0xfe,
    0x0,

    /* U+0041 "A" */
    0xf, 0xc0, 0x37, 0x41, 0xca, 0xa, 0x1c, 0x34,
    0x30, 0xff, 0xd7, 0x3, 0xa8, 0xb,

    /* U+0042 "B" */
    0x3f, 0xe0, 0xd0, 0xe3, 0x43, 0x4f, 0xfc, 0x34,
    0x38, 0xd0, 0xa3, 0x43, 0x8f, 0xf8,

    /* U+0043 "C" */
    0x1f, 0xf0, 0xe0, 0x47, 0x0, 0x1c, 0x0, 0x70,
    0x1, 0xc0, 0x3, 0x81, 0x7, 0xfc,

    /* U+0044 "D" */
    0x3f, 0xe0, 0x34, 0x78, 0x34, 0x1c, 0x34, 0x1c,
    0x34, 0x1c, 0x34, 0x1c, 0x34, 0x78, 0x3f, 0xe0,

    /* U+0045 "E" */
    0x3f, 0xf0, 0xd0, 0x3, 0x40, 0xf, 0xf4, 0x34,
    0x0, 0xd0, 0x3, 0x40, 0xf, 0xfc,

    /* U+0046 "F" */
    0x3f, 0xf0, 0xd0, 0x3, 0x40, 0xd, 0x0, 0x3f,
    0xd0, 0xd0, 0x3, 0x40, 0xd, 0x0,

    /* U+0047 "G" */
    0xf, 0xf4, 0xe0, 0x47, 0x0, 0x1c, 0x0, 0x70,
    0xb9, 0xc0, 0xa3, 0x83, 0x83, 0xfe,

    /* U+0048 "H" */
    0x34, 0xc, 0x34, 0xc, 0x34, 0xc, 0x3f, 0xfc,
    0x34, 0x1c, 0x34, 0xc, 0x34, 0xc, 0x34, 0xc,

    /* U+0049 "I" */
    0x34, 0xd3, 0x4d, 0x34, 0xd3, 0x4d,

    /* U+004A "J" */
    0x28, 0xa2, 0x8a, 0x28, 0xa2, 0x8e, 0xf4, 0x0,

    /* U+004B "K" */
    0x34, 0x34, 0xd2, 0xc3, 0x4d, 0xe, 0xb0, 0x3f,
    0xc0, 0xd3, 0x43, 0x4b, 0xd, 0xe,

    /* U+004C "L" */
    0x34, 0x3, 0x40, 0x34, 0x3, 0x40, 0x34, 0x3,
    0x40, 0x34, 0x3, 0xfe,

    /* U+004D "M" */
    0x3d, 0x3, 0xd3, 0xe0, 0xbd, 0x37, 0xd, 0xd3,
    0x74, 0xcd, 0x36, 0x9c, 0xd3, 0x5e, 0x4d, 0x34,
    0xf0, 0xd3, 0x46, 0xd,

    /* U+004E "N" */
    0x3d, 0xd, 0x3b, 0xd, 0x37, 0xd, 0x36, 0x8d,
    0x35, 0xcd, 0x34, 0xdd, 0x34, 0xad, 0x34, 0x3d,

    /* U+004F "O" */
    0xf, 0xe0, 0x38, 0x7c, 0x70, 0x1c, 0x70, 0xc,
    0x70, 0xc, 0x70, 0x1c, 0x38, 0x3c, 0xf, 0xe0,

    /* U+0050 "P" */
    0x3f, 0xe0, 0xd0, 0xe3, 0x42, 0x8d, 0xe, 0x3f,
    0xf0, 0xd0, 0x3, 0x40, 0xd, 0x0,

    /* U+0051 "Q" */
    0xf, 0xe0, 0x38, 0x7c, 0x70, 0x1c, 0x70, 0xc,
    0x70, 0xc, 0x70, 0x1c, 0x38, 0x3c, 0xf, 0xf0,
    0x0, 0x38, 0x0, 0x14,

    /* U+0052 "R" */
    0x3f, 0xe0, 0xd0, 0xe3, 0x42, 0xcd, 0xe, 0x3f,
    0xf0, 0xd2, 0xc3, 0x43, 0xd, 0xe,

    /* U+0053 "S" */
    0x2f, 0xe1, 0xd0, 0x7, 0x0, 0xf, 0x90, 0x6,
    0xe0, 0x1, 0xc1, 0xb, 0x1f, 0xf0,

    /* U+0054 "T" */
    0xff, 0xf0, 0x2c, 0x0, 0xb0, 0x2, 0xc0, 0xb,
    0x0, 0x2c, 0x0, 0xb0, 0x2, 0xc0,

    /* U+0055 "U" */
    0x30, 0x1c, 0xc0, 0x73, 0x1, 0xcc, 0x7, 0x30,
    0x1c, 0xd0, 0x73, 0x83, 0x83, 0xf8,

    /* U+0056 "V" */
    0xa0, 0x2d, 0xc0, 0xe3, 0x3, 0x4d, 0x1c, 0x28,
    0xb0, 0x73, 0x80, 0xdd, 0x3, 0xf0,

    /* U+0057 "W" */
    0xb0, 0x74, 0x39, 0xc2, 0xe0, 0xd3, 0xe, 0xc3,
    0xd, 0x33, 0x1c, 0x35, 0xcd, 0xa0, 0xaa, 0x26,
    0x81, 0xf4, 0x6d, 0x3, 0xc0, 0xf0,

    /* U+0058 "X" */
    0xb0, 0x34, 0xd2, 0x81, 0xdc, 0x3, 0xe0, 0xb,
    0x40, 0x77, 0x3, 0x4e, 0x2c, 0xd,

    /* U+0059 "Y" */
    0xb0, 0x34, 0xd2, 0x82, 0xcc, 0x3, 0xe0, 0xb,
    0x0, 0x1c, 0x0, 0x70, 0x1, 0xc0,

    /* U+005A "Z" */
    0xbf, 0xf0, 0xe, 0x1, 0xc0, 0x34, 0xe, 0x2,
    0xc0, 0x74, 0xb, 0xff,

    /* U+005B "[" */
    0x3e, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x3e, 0x0,

    /* U+005C "\\" */
    0x0, 0x7, 0x0, 0x34, 0x2, 0xc0, 0xd, 0x0,
    0xa0, 0x3, 0x0, 0x28, 0x0, 0xc0, 0x0,

    /* U+005D "]" */
    0xbc, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd,
    0xd, 0xbd, 0x0,

    /* U+005E "^" */
    0xb, 0x0, 0x3a, 0x2, 0x8d, 0x1c, 0x1c,

    /* U+005F "_" */
    0xff, 0xd0, 0x0,

    /* U+0060 "`" */
    0x24, 0xb, 0xc0, 0x0,

    /* U+0061 "a" */
    0x3f, 0x80, 0x1c, 0x1a, 0xc7, 0x5c, 0xb0, 0xd3,
    0xfa,

    /* U+0062 "b" */
    0x30, 0x3, 0x0, 0x3f, 0xc3, 0x4e, 0x30, 0x73,
    0x7, 0x30, 0xe3, 0xf8,

    /* U+0063 "c" */
    0x2f, 0x8d, 0x7, 0x1, 0xc0, 0x74, 0xb, 0xe0,

    /* U+0064 "d" */
    0x0, 0xa0, 0xa, 0x2f, 0xe3, 0x4a, 0x70, 0xa7,
    0xa, 0x70, 0xa2, 0xfe,

    /* U+0065 "e" */
    0x1f, 0x83, 0xe, 0x7f, 0xe7, 0x0, 0x74, 0x2,
    0xfd,

    /* U+0066 "f" */
    0x2f, 0xe, 0xb, 0xf0, 0xd0, 0x34, 0xd, 0x3,
    0x40, 0xd0,

    /* U+0067 "g" */
    0x2f, 0xf1, 0xd3, 0x47, 0xd, 0xf, 0xe0, 0x34,
    0x0, 0xff, 0x87, 0x7, 0x1c, 0x2c, 0x2f, 0xd0,

    /* U+0068 "h" */
    0x30, 0x3, 0x0, 0x3f, 0xc3, 0x4e, 0x30, 0xb3,
    0x7, 0x30, 0x73, 0x7,

    /* U+0069 "i" */
    0x30, 0x43, 0xc, 0x30, 0xc3, 0xc,

    /* U+006A "j" */
    0xc, 0x4, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc,
    0x1c, 0x34, 0x10,

    /* U+006B "k" */
    0x30, 0x3, 0x0, 0x30, 0xd3, 0x2c, 0x3b, 0x3,
    0xb4, 0x32, 0xc3, 0xe,

    /* U+006C "l" */
    0x30, 0xc3, 0xc, 0x30, 0xc3, 0xc,

    /* U+006D "m" */
    0x3f, 0xef, 0x83, 0x4f, 0x1c, 0x30, 0xa0, 0xd3,
    0xa, 0xd, 0x30, 0xa0, 0xd3, 0xa, 0xd,

    /* U+006E "n" */
    0x3f, 0xc3, 0x4a, 0x30, 0x73, 0x7, 0x30, 0x73,
    0x7,

    /* U+006F "o" */
    0x2f, 0x83, 0x4e, 0x70, 0xb7, 0xb, 0x34, 0xe2,
    0xf8,

    /* U+0070 "p" */
    0x3f, 0xc3, 0x4e, 0x30, 0x73, 0x7, 0x30, 0xe3,
    0xfc, 0x30, 0x3, 0x0, 0x30, 0x0,

    /* U+0071 "q" */
    0x2f, 0xe7, 0x4a, 0x70, 0xa7, 0xa, 0x74, 0xe2,
    0xfe, 0x0, 0xa0, 0xa, 0x0, 0xa0,

    /* U+0072 "r" */
    0x0, 0xe, 0xc3, 0x90, 0xc0, 0x30, 0xc, 0x3,
    0x0,

    /* U+0073 "s" */
    0x2f, 0xc7, 0x4, 0x79, 0x0, 0x7c, 0x1, 0xc7,
    0xf8,

    /* U+0074 "t" */
    0x10, 0xd, 0xb, 0xf0, 0xd0, 0x34, 0xd, 0x3,
    0x80, 0x7c,

    /* U+0075 "u" */
    0x70, 0xa7, 0xa, 0x70, 0xa3, 0xa, 0x34, 0xa2,
    0xfe,

    /* U+0076 "v" */
    0xa0, 0xe7, 0xd, 0x31, 0xc3, 0x6c, 0x2b, 0x41,
    0xf0,

    /* U+0077 "w" */
    0xa0, 0xd2, 0xdc, 0x78, 0xa3, 0x2b, 0x34, 0xdd,
    0xcc, 0x2b, 0x3b, 0xb, 0xcb, 0x80,

    /* U+0078 "x" */
    0x71, 0xc3, 0x74, 0x1f, 0x1, 0xf0, 0x37, 0x47,
    0x1c,

    /* U+0079 "y" */
    0xa0, 0xa7, 0xd, 0x31, 0xc3, 0x6c, 0x2b, 0x81,
    0xf4, 0x7, 0x0, 0xb0, 0xe, 0x0,

    /* U+007A "z" */
    0xbf, 0xc0, 0xe0, 0xb0, 0x70, 0x34, 0x2f, 0xf0,

    /* U+007B "{" */
    0x1, 0xe, 0x1c, 0x1c, 0x2c, 0xb4, 0x74, 0x2c,
    0x2c, 0x1c, 0xf, 0x0,

    /* U+007C "|" */
    0x30, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0x0,

    /* U+007D "}" */
    0x0, 0x1e, 0x2, 0xc0, 0x70, 0x2c, 0x3, 0xc0,
    0xe0, 0xb0, 0x1c, 0xb, 0xb, 0x40, 0x0,

    /* U+007E "~" */
    0x2d, 0x12, 0x7e, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 42, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 51, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 6, .adv_w = 76, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 10, .adv_w = 108, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 24, .adv_w = 108, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 45, .adv_w = 108, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 63, .adv_w = 134, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 81, .adv_w = 43, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 83, .adv_w = 55, .box_w = 4, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 94, .adv_w = 55, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 103, .adv_w = 81, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 110, .adv_w = 108, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 121, .adv_w = 47, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 124, .adv_w = 81, .box_w = 5, .box_h = 2, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 127, .adv_w = 46, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 85, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 142, .adv_w = 108, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 156, .adv_w = 108, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 176, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 188, .adv_w = 108, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 202, .adv_w = 108, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 216, .adv_w = 108, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 230, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 242, .adv_w = 108, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 256, .adv_w = 108, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 270, .adv_w = 46, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 274, .adv_w = 51, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 280, .adv_w = 108, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 108, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 295, .adv_w = 108, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 303, .adv_w = 85, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 313, .adv_w = 187, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 346, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 360, .adv_w = 118, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 374, .adv_w = 104, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 388, .adv_w = 123, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 404, .adv_w = 106, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 418, .adv_w = 102, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 432, .adv_w = 118, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 446, .adv_w = 130, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 462, .adv_w = 52, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 468, .adv_w = 57, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 476, .adv_w = 112, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 91, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 502, .adv_w = 163, .box_w = 10, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 522, .adv_w = 132, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 538, .adv_w = 126, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 554, .adv_w = 113, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 568, .adv_w = 126, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 588, .adv_w = 119, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 602, .adv_w = 104, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 616, .adv_w = 101, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 630, .adv_w = 125, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 644, .adv_w = 114, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 658, .adv_w = 175, .box_w = 11, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 680, .adv_w = 108, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 694, .adv_w = 105, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 708, .adv_w = 103, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 720, .adv_w = 66, .box_w = 4, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 731, .adv_w = 90, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 746, .adv_w = 66, .box_w = 4, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 757, .adv_w = 108, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 764, .adv_w = 120, .box_w = 6, .box_h = 2, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 767, .adv_w = 49, .box_w = 5, .box_h = 3, .ofs_x = -1, .ofs_y = 6},
    {.bitmap_index = 771, .adv_w = 97, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 780, .adv_w = 103, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 792, .adv_w = 85, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 800, .adv_w = 104, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 812, .adv_w = 98, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 821, .adv_w = 66, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 831, .adv_w = 101, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 847, .adv_w = 105, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 859, .adv_w = 46, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 865, .adv_w = 46, .box_w = 4, .box_h = 11, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 876, .adv_w = 96, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 888, .adv_w = 49, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 894, .adv_w = 161, .box_w = 10, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 909, .adv_w = 105, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 918, .adv_w = 102, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 927, .adv_w = 103, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 941, .adv_w = 103, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 955, .adv_w = 69, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 964, .adv_w = 90, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 973, .adv_w = 69, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 983, .adv_w = 104, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 992, .adv_w = 94, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1001, .adv_w = 147, .box_w = 9, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1015, .adv_w = 89, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1024, .adv_w = 94, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1038, .adv_w = 87, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1046, .adv_w = 69, .box_w = 4, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1058, .adv_w = 48, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1067, .adv_w = 69, .box_w = 5, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1082, .adv_w = 108, .box_w = 6, .box_h = 3, .ofs_x = 0, .ofs_y = 1}
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
    0, -5, 0, 0, 0, -26, -13, -12,
    0, 0, -2, -7, 0, 0, 0, -3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, -3, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -8, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, -1, 0, 0, -7, -1, -5,
    -3, 0, -10, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, -1, -1, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -2, 0, -4, -3, -1, 0,
    3, -2, -1, 0, -3, -1, -2, 0,
    -1, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -6, 0, 0, 0, -3,
    0, 1, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, -3, -4, 0, -3,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -26, 0, 0, 0,
    0, 0, -3, 0, 0, 0, 0, 0,
    0, -1, -1, 0, 0, -12, -2, -9,
    -6, 0, -14, 0, 0, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 0, -3,
    0, -5, -4, 0, -6, 0, 0, 0,
    -13, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, -4,
    -4, -12, 0, -5, -3, -7, -13, -4,
    0, 0, -1, 0, 0, 0, -2, 0,
    0, 0, 0, -2, 0, -2, -1, -5,
    -2, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -56, 0, 0, 0, -8,
    0, -2, -2, -3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -5, 0,
    -7, -7, 0, -6, 0, -4, -5, 0,
    -4, -2, -2, 0, -2, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -10, 0, -2, 0, 0, -8, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, -1, 0, -1,
    0, 0, -5, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -7, 0, 0, 0, -6, 0, -1, 0,
    0, -4, 0, 0, 0, -2, -2, 0,
    -1, -8, -1, -5, -4, 0, -10, 0,
    -8, -2, 0, 0, -1, -1, -2, -2,
    0, 0, 0, -2, 0, -3, -3, 0,
    -3, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, -2,
    0, 0, 0, -1, 0, -1, 0, -2,
    0, -2, -4, 0, -2, -4, 0, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, 0, 0, 0, 0, 0, -5, 0,
    0, 0, 0, 0, 0, -1, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, -2, -1, -3,
    0, 0, 0, 0, -2, -2, -2, 0,
    -2, 0, 0, 0, 0, 0, 0, -2,
    0, -1, 0, -2, 0, -1, 0, -2,
    0, 0, 0, -2, 0, -2, 0, -2,
    0, -4, -5, -1, -2, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    0, 0, 0, 0, 0, 0, -3, 0,
    0, 0, 0, 0, 0, -1, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, -1, 0, -2,
    0, 0, 0, -1, -1, -1, -2, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, -11, -1, -7, 0, 0, 0, -5,
    0, -1, -1, -3, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -5, 0,
    -3, -4, -1, -4, 0, -4, -3, -1,
    -3, -1, -2, -3, -2, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, -2, 0, 0, -3, 0,
    -1, 0, 0, 0, 0, 0, -1, -1,
    0, 0, 0, 0, 0, -1, -1, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, -1, -1, -2, 0, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, -1, -1, -2,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -6, 0, 0, 0, 0, 0,
    0, -2, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    -3, -3, -2, -3, 0, -1, 0, -2,
    -3, -4, -4, 0, -4, 0, 0, 0,
    -16, 0, 0, 0, -16, 0, -11, 0,
    0, -1, 0, 0, 0, -1, -2, 0,
    0, -16, -2, -11, -8, 0, -17, 0,
    -14, -1, 0, 0, 0, 0, -1, -1,
    0, 0, 0, -3, 0, -7, -5, 0,
    -7, 0, 0, -1, 0, 0, 0, -2,
    0, -1, 0, -2, 0, -1, 0, -2,
    0, 0, 0, -2, 0, -2, 0, -2,
    0, -3, -5, 0, -3, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    0, 0, 0, -1, 0, -13, -1, -7,
    0, 0, 0, -5, 0, 0, 0, -3,
    0, 0, 0, -1, 0, -3, -4, -1,
    -2, -3, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -3, 0, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, -1, 0, -2,
    0, -1, -4, 0, -2, -2, -1, 0,
    -2, -2, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, 0, -1,
    0, 0, 0, -2, 0, -1, -3, 0,
    0, 0, 0, 0, 0, 0, -2, -1,
    0, 0, 0, -1, 0, -2, -2, -2,
    -2, 0, 0, 0, 0, -3, 0, 0,
    0, -11, -12, -10, -10, 0, -4, -8,
    0, -1, -2, -3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -15, -1,
    -15, -15, -3, -17, 0, -13, -15, -3,
    -13, -11, -11, -11, -11, -15, 0, 0,
    0, 0, 0, 0, 0, -2, 0, -2,
    0, 0, 0, -1, 0, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -1, -1, -1, -2,
    0, -1, -1, 0, -1, 0, 0, 0,
    0, -1, 0, 0, 0, -3, 0, 0,
    0, -9, -5, -8, -2, 0, -2, -5,
    0, -2, -2, -3, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -5, 0,
    -6, -6, -1, -6, 0, -4, -4, 0,
    -4, -1, -2, -1, -1, -2, 0, 0,
    0, -1, 0, 0, 0, -6, -3, -5,
    0, 0, 0, -4, 0, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -3, 0, -4, -4, 0, -4,
    0, -3, -3, 0, -2, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, 0, -7, 0, 0, 0, 0, 0,
    0, -3, -3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    -5, -4, -1, -3, 0, -1, 0, -2,
    -3, -5, -5, 0, -5, 0, 0, 0,
    0, -6, 0, 0, 1, -14, -13, -12,
    -8, 0, -7, -10, 0, -5, -6, -5,
    -3, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -12, 0, -13, -13, -4, -14,
    0, -12, -13, -3, -11, -7, -8, -7,
    -7, -9, 0, 0, 0, 0, 0, 0,
    0, 0, -5, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, -2, -1, -3, 0, -1, 0, 0,
    -2, -2, -2, 0, -2, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, -4, -5, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -4, 0, -6, -6, -2, 0,
    1, -2, -2, -3, -5, -5, -5, -2,
    -5, -2, -4, 0, -14, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, -3, 0, -1, -11, -3, -9,
    -6, 0, -13, 0, 0, 0, 0, 0,
    -1, 0, -2, 0, 0, 0, 0, -3,
    0, -4, -3, 0, -4, 0, 0, 0,
    -1, 0, 0, 0, -1, 0, 0, 0,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, -14, -1, -5, -3, 0, -12, 0,
    -8, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, -1, 0,
    -1, 0, 0, -1, -3, 0, 0, -3,
    -2, 0, 0, 0, 0, -4, 0, -1,
    -1, 0, 0, -3, -1, -15, -1, -6,
    -3, -4, -13, -2, -7, -6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, 0, -2, -2, 0, 0, -5,
    0, 0, 0, 0, 0, 0, -5, 0,
    0, -1, 0, 0, 0, 0, 0, -1,
    0, -17, 0, -2, -1, 0, -9, 0,
    -3, -2, 0, 0, -1, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, -2, 0, 0, 0, 0,
    0, -3, 0, 0, 0, 0, 0, -1,
    0, -16, -1, -5, -3, -1, -16, -1,
    -7, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    -2, 0, 0, -3, 0, -1, 0, 0,
    0, -7, -7, -5, 0, 0, 0, -4,
    0, 0, 0, -3, 0, -2, 0, 0,
    0, -1, 1, -1, 0, 0, 0, 0,
    -1, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -12, 0, -1, 0, 0, -6, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, -2,
    -1, 0, 0, 0, 0, -3, 0, 0,
    0, 0, 0, -2, -1, -16, -1, -5,
    -3, 0, -13, -1, -7, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, -1, 0, -1, 0, 0, -2,
    0, 0, 0, 0, 0, 0, -6, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, -12, -1, -2, 0, 0, -7, 0,
    -2, -1, 0, 0, -2, -2, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, -2, 0, 0, -4,
    -1, 0, 0, 0, 0, -4, 0, -1,
    -1, 0, 0, -3, -2, -16, -1, -6,
    -4, -5, -13, -2, -7, -6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, -2, -2, 0, 0, -5,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, -1,
    0, -13, -1, -4, -3, -1, -12, -1,
    -5, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, -1, 0, -1,
    0, -10, -7, -6, 0, 0, 0, -5,
    0, 0, 0, -4, 0, -10, 0, 0,
    0, -5, -5, -2, -2, -4, 0, 0,
    -1, -1, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    0, 0, 0, -2, 0, 0, -1, 0,
    0, -1, 0, 0, 0, 0, 0, -2,
    0, -15, 0, -4, -2, -1, -10, 0,
    -5, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    -1, 0, 0, -4, 0, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -11, 0, -1,
    0, 0, -6, 0, -2, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, -2, 0, -5, -2, -4,
    0, -1, 0, -3, 0, 0, 0, -3,
    0, -11, 0, -1, 0, -5, -7, -2,
    -2, -5, -1, 0, -2, -1, 0, -2,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, -4, 0, 0, 0, -2,
    0, -4, -1, -3, 0, -1, 0, -3,
    -1, 0, 0, -3, 0, -11, 0, -2,
    0, -5, -8, -2, -2, -5, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -4,
    0, 0, 0, 0, 0, 0, -5, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -11, 0, -1, 0, 0, -7, 0,
    -2, -2, 0, 0, -2, -2, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, -6, -2, -4, 0, -1, 0, -3,
    0, 0, 0, -3, 0, -11, 0, -1,
    0, -5, -7, -2, -2, -4, -2, 0,
    -2, -2, 0, -2, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    0, 0, 0, 0, 0, 0, -4, 0,
    0, -1, 0, 0, 0, 0, 0, -1,
    0, -15, -1, -2, -1, 0, -10, 0,
    -4, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, -3, -4, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, 0,
    -5, -5, -1, 0, 3, -2, -2, -2,
    -4, -3, -4, -1, -3, -2, -3, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -3
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

extern const lv_font_t lv_font_montserrat_12;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t titillium_12 = {
#else
lv_font_t titillium_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 13,          /*The maximum line height required by the font*/
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
    .fallback = &lv_font_montserrat_12,
#endif
    .user_data = NULL,
};



#endif /*#if TITILLIUM_12*/


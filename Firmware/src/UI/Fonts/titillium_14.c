/*******************************************************************************
 * Size: 14 px
 * Bpp: 2
 * Opts: --bpp 2 --size 14 --no-compress --font TitilliumWeb-SemiBold.ttf --range 32-127 --format lvgl -o titillium_14.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef TITILLIUM_14
#define TITILLIUM_14 1
#endif

#if TITILLIUM_14

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0xee, 0xee, 0xee, 0xe, 0xe0,

    /* U+0022 "\"" */
    0x37, 0x8d, 0xe3, 0x68, 0x0,

    /* U+0023 "#" */
    0x1c, 0x30, 0x1c, 0x30, 0xbf, 0xfe, 0x1d, 0x74,
    0x1c, 0x30, 0x1c, 0x30, 0xbf, 0xfe, 0x1c, 0x74,
    0x1c, 0x30,

    /* U+0024 "$" */
    0x0, 0x80, 0x0, 0xc0, 0xf, 0xf8, 0x3d, 0xd4,
    0x34, 0x80, 0x3e, 0x80, 0xb, 0xf0, 0x2, 0xbc,
    0x2, 0x2c, 0x17, 0x7c, 0x2f, 0xe0, 0x3, 0x0,
    0x1, 0x0,

    /* U+0025 "%" */
    0x0, 0x0, 0x7c, 0x20, 0x99, 0x90, 0xa9, 0xc0,
    0x3d, 0x80, 0x2, 0x40, 0x3, 0x7c, 0x2, 0xaa,
    0x9, 0xa9, 0xc, 0x3c, 0x0, 0x0,

    /* U+0026 "&" */
    0x7, 0xf4, 0x0, 0xe7, 0xc0, 0xd, 0x2c, 0x0,
    0xe3, 0x80, 0xf, 0xe1, 0x3, 0x9e, 0x34, 0x70,
    0x7f, 0x7, 0x1, 0xf0, 0x3d, 0x6f, 0xc1, 0xff,
    0x8,

    /* U+0027 "'" */
    0x34, 0xd3, 0x40,

    /* U+0028 "(" */
    0xd, 0x1c, 0x3c, 0x38, 0x74, 0x74, 0x74, 0x74,
    0x38, 0x3c, 0x2c, 0xd,

    /* U+0029 ")" */
    0x70, 0x34, 0x3c, 0x2c, 0x1d, 0x1d, 0x1d, 0x1d,
    0x2c, 0x3c, 0x38, 0x70,

    /* U+002A "*" */
    0x2, 0x3, 0xb0, 0xf, 0xd3, 0xa0, 0x2, 0x0,

    /* U+002B "+" */
    0x3, 0x80, 0x3, 0x80, 0x3, 0x80, 0x7f, 0xfc,
    0x17, 0x94, 0x3, 0x80, 0x3, 0x80,

    /* U+002C "," */
    0x14, 0xd7, 0x2c,

    /* U+002D "-" */
    0x0, 0x3, 0xfc, 0x15, 0x40,

    /* U+002E "." */
    0x34, 0xd0,

    /* U+002F "/" */
    0x0, 0x90, 0xd, 0x2, 0xc0, 0x38, 0x7, 0x0,
    0xb0, 0xd, 0x2, 0xc0, 0x38, 0x7, 0x0, 0x0,
    0x0,

    /* U+0030 "0" */
    0xb, 0xe0, 0x3d, 0x7c, 0x74, 0x1c, 0xb0, 0x1d,
    0xb0, 0xd, 0xb0, 0x1d, 0x74, 0x1c, 0x3d, 0x7c,
    0xb, 0xe0,

    /* U+0031 "1" */
    0x0, 0x2, 0xe3, 0xf9, 0x8e, 0x3, 0x80, 0xe0,
    0x38, 0xe, 0x3, 0x80, 0xe0,

    /* U+0032 "2" */
    0x2f, 0xe0, 0x96, 0xe0, 0x3, 0x80, 0xe, 0x0,
    0xf0, 0xf, 0x0, 0xf0, 0xf, 0x55, 0x3f, 0xfc,

    /* U+0033 "3" */
    0x2f, 0xe0, 0x96, 0xe0, 0x3, 0x80, 0x1d, 0xf,
    0xf0, 0x1, 0xe0, 0x2, 0xc5, 0x5e, 0x2f, 0xe0,

    /* U+0034 "4" */
    0x3, 0x40, 0xb, 0x0, 0xe, 0x0, 0x1c, 0x20,
    0x3c, 0x74, 0x34, 0x74, 0x7f, 0xfd, 0x15, 0x74,
    0x0, 0x74,

    /* U+0035 "5" */
    0x0, 0x0, 0x3f, 0xfc, 0x39, 0x50, 0x34, 0x0,
    0x3f, 0xf0, 0x25, 0x7c, 0x0, 0x2c, 0x0, 0x2c,
    0x25, 0xb8, 0x2f, 0xe0,

    /* U+0036 "6" */
    0xb, 0xf8, 0x2d, 0x54, 0x34, 0x0, 0x7f, 0xf4,
    0x79, 0x7c, 0x70, 0x1d, 0x34, 0x1c, 0x2d, 0x7c,
    0xb, 0xe0,

    /* U+0037 "7" */
    0x0, 0xf, 0xff, 0x55, 0xf0, 0xd, 0x2, 0xc0,
    0x38, 0xb, 0x0, 0xe0, 0x1c, 0x2, 0x80, 0x0,
    0x0,

    /* U+0038 "8" */
    0x1f, 0xe0, 0x3d, 0x7c, 0x74, 0x1c, 0x38, 0x3c,
    0x2f, 0xf4, 0x79, 0x7c, 0xb0, 0x1d, 0x7d, 0x7c,
    0x1f, 0xf4,

    /* U+0039 "9" */
    0x1f, 0xe0, 0x3d, 0xb8, 0x70, 0x2c, 0x74, 0x2c,
    0x2f, 0xfc, 0x0, 0x1c, 0x0, 0x2c, 0x15, 0xb8,
    0x3f, 0xe0,

    /* U+003A ":" */
    0x34, 0xd0, 0x0, 0x34, 0xd0,

    /* U+003B ";" */
    0x38, 0xe0, 0x0, 0x14, 0xe3, 0x1c,

    /* U+003C "<" */
    0x0, 0x4, 0x2, 0xe1, 0xf9, 0xf, 0x0, 0x1f,
    0x80, 0x7, 0xe0, 0x1, 0x40,

    /* U+003D "=" */
    0x0, 0x0, 0xff, 0xf1, 0x55, 0x40, 0x0, 0x3f,
    0xfc, 0x55, 0x50,

    /* U+003E ">" */
    0x40, 0xf, 0x80, 0x1f, 0x80, 0x1f, 0xb, 0xdb,
    0xd0, 0x80, 0x0,

    /* U+003F "?" */
    0x7f, 0x81, 0x6e, 0x0, 0xe0, 0x2d, 0xf, 0x40,
    0xd0, 0x0, 0x0, 0xd0, 0xd, 0x0,

    /* U+0040 "@" */
    0x0, 0xbf, 0xe4, 0x0, 0xbd, 0x56, 0xe0, 0x1e,
    0x0, 0x7, 0x43, 0xc2, 0xfe, 0x3c, 0x34, 0xb5,
    0xe1, 0xc7, 0xe, 0xe, 0x1c, 0x70, 0xe0, 0xe1,
    0xc7, 0xe, 0xe, 0x1c, 0x34, 0xb5, 0xf7, 0xc3,
    0x83, 0xf7, 0xf0, 0x2d, 0x0, 0x0, 0x0, 0xbe,
    0x54, 0x0, 0x1, 0xbf, 0xc0, 0x0,

    /* U+0041 "A" */
    0x7, 0xf0, 0x3, 0xec, 0x0, 0xe3, 0x40, 0x74,
    0xe0, 0x2c, 0x2c, 0xf, 0x7, 0x3, 0xff, 0xd1,
    0xd5, 0x78, 0xb0, 0xb, 0x0,

    /* U+0042 "B" */
    0xff, 0xe3, 0xd6, 0xde, 0x3, 0xb8, 0x1d, 0xff,
    0xf3, 0x81, 0xee, 0x2, 0xfd, 0x5e, 0xff, 0xe0,

    /* U+0043 "C" */
    0xb, 0xfc, 0x3e, 0x58, 0x38, 0x0, 0x34, 0x0,
    0x74, 0x0, 0x34, 0x0, 0x38, 0x0, 0x2e, 0x58,
    0xb, 0xfc,

    /* U+0044 "D" */
    0xff, 0xe0, 0xf5, 0xb8, 0xe0, 0x2c, 0xe0, 0x1c,
    0xe0, 0x1d, 0xe0, 0x1c, 0xe0, 0x2c, 0xf5, 0xb8,
    0xff, 0xe0,

    /* U+0045 "E" */
    0xff, 0xf3, 0xd5, 0x4e, 0x0, 0x38, 0x0, 0xff,
    0xd3, 0x95, 0xe, 0x0, 0x3d, 0x54, 0xff, 0xf0,

    /* U+0046 "F" */
    0xff, 0xf3, 0xd5, 0x4e, 0x0, 0x38, 0x0, 0xff,
    0xd3, 0x95, 0xe, 0x0, 0x38, 0x0, 0xe0, 0x0,

    /* U+0047 "G" */
    0xb, 0xfe, 0x2e, 0x55, 0x38, 0x0, 0x34, 0x0,
    0x74, 0x3e, 0x34, 0x1e, 0x38, 0xe, 0x2e, 0x5e,
    0xb, 0xfe,

    /* U+0048 "H" */
    0xe0, 0xd, 0xe0, 0xd, 0xe0, 0xd, 0xe0, 0x1d,
    0xff, 0xfd, 0xe5, 0x5d, 0xe0, 0xd, 0xe0, 0xd,
    0xe0, 0xd,

    /* U+0049 "I" */
    0xee, 0xee, 0xee, 0xee, 0xe0,

    /* U+004A "J" */
    0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c,
    0x2c, 0xb8, 0x0,

    /* U+004B "K" */
    0xe0, 0x3b, 0x82, 0xce, 0xe, 0x38, 0xb0, 0xff,
    0x83, 0xdf, 0xe, 0xe, 0x38, 0x2c, 0xe0, 0x38,

    /* U+004C "L" */
    0xe0, 0xe, 0x0, 0xe0, 0xe, 0x0, 0xe0, 0xe,
    0x0, 0xe0, 0xf, 0x54, 0xff, 0xd0,

    /* U+004D "M" */
    0xfc, 0x3, 0xff, 0xd0, 0x7f, 0xee, 0xb, 0xfe,
    0xb0, 0xdf, 0xe3, 0x1c, 0xfe, 0x36, 0xcf, 0xe2,
    0xf4, 0xfe, 0xf, 0xf, 0xe0, 0xa0, 0xf0,

    /* U+004E "N" */
    0xfc, 0xe, 0xfd, 0xe, 0xeb, 0xe, 0xe7, 0xe,
    0xe3, 0x8e, 0xe2, 0xce, 0xe0, 0xde, 0xe0, 0xbe,
    0xe0, 0x7e,

    /* U+004F "O" */
    0xb, 0xf8, 0xb, 0x9b, 0xc3, 0x80, 0x70, 0xd0,
    0xd, 0x74, 0x3, 0x5d, 0x0, 0xd3, 0x80, 0x74,
    0xb9, 0x7c, 0xb, 0xf8, 0x0,

    /* U+0050 "P" */
    0xff, 0xe3, 0xd6, 0xee, 0x3, 0xf8, 0xf, 0xe0,
    0x7b, 0xff, 0xce, 0x40, 0x38, 0x0, 0xe0, 0x0,

    /* U+0051 "Q" */
    0xb, 0xf8, 0xb, 0x9b, 0xc3, 0x80, 0x70, 0xd0,
    0xd, 0x74, 0x3, 0x5d, 0x0, 0xd3, 0x80, 0x70,
    0xb9, 0x7c, 0xb, 0xfc, 0x0, 0x3, 0x80, 0x0,
    0xa0,

    /* U+0052 "R" */
    0xff, 0xe0, 0xf5, 0x78, 0xe0, 0x2c, 0xe0, 0x3c,
    0xff, 0xf0, 0xe5, 0xe0, 0xe0, 0xb0, 0xe0, 0x74,
    0xe0, 0x3c,

    /* U+0053 "S" */
    0x1f, 0xf8, 0x39, 0x54, 0x70, 0x0, 0x3d, 0x0,
    0xb, 0xf0, 0x0, 0x7c, 0x0, 0x2c, 0x15, 0x78,
    0x3f, 0xe0,

    /* U+0054 "T" */
    0xff, 0xfc, 0x57, 0x94, 0x3, 0x40, 0x3, 0x40,
    0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40,
    0x3, 0x40,

    /* U+0055 "U" */
    0xe0, 0x1c, 0xe0, 0x1c, 0xe0, 0x1c, 0xe0, 0x1c,
    0xe0, 0x1c, 0xe0, 0x1c, 0xe0, 0x2c, 0xb9, 0xb8,
    0x2f, 0xe0,

    /* U+0056 "V" */
    0xb0, 0xb, 0x1d, 0x3, 0x83, 0x80, 0xd0, 0xf0,
    0x70, 0x2c, 0x2c, 0x7, 0x4e, 0x0, 0xe3, 0x40,
    0x3e, 0xc0, 0xb, 0xf0, 0x0,

    /* U+0057 "W" */
    0xb0, 0x2d, 0x7, 0x5c, 0xf, 0x81, 0xc3, 0x43,
    0xb0, 0xb0, 0xe1, 0xdc, 0x38, 0x2c, 0xb3, 0x4d,
    0xb, 0x38, 0xe3, 0x41, 0xcd, 0x2d, 0xc0, 0x3b,
    0x7, 0xb0, 0xf, 0xc0, 0xf8, 0x0,

    /* U+0058 "X" */
    0xb0, 0xd, 0x3c, 0x3c, 0x1d, 0x70, 0xb, 0xe0,
    0x3, 0xc0, 0xb, 0xd0, 0xd, 0xb0, 0x38, 0x38,
    0x70, 0x1d,

    /* U+0059 "Y" */
    0xb0, 0x1d, 0x38, 0x3c, 0x2c, 0x70, 0xe, 0xe0,
    0x7, 0xc0, 0x3, 0x80, 0x3, 0x80, 0x3, 0x80,
    0x3, 0x80,

    /* U+005A "Z" */
    0x7f, 0xfc, 0x55, 0xf0, 0xb, 0x0, 0x74, 0x3,
    0x80, 0x3c, 0x2, 0xc0, 0x1e, 0x55, 0x7f, 0xfc,

    /* U+005B "[" */
    0x0, 0xfd, 0xe4, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
    0xe0, 0xe0, 0xe0, 0xe0, 0xfd, 0x14,

    /* U+005C "\\" */
    0x20, 0x1, 0xd0, 0x3, 0xc0, 0x7, 0x0, 0xe,
    0x0, 0x1c, 0x0, 0x38, 0x0, 0x70, 0x0, 0xe0,
    0x2, 0xc0, 0x0, 0x0,

    /* U+005D "]" */
    0x0, 0x7f, 0x1f, 0xf, 0xf, 0xf, 0xf, 0xf,
    0xf, 0xf, 0xf, 0xf, 0x7f, 0x14,

    /* U+005E "^" */
    0x1, 0x40, 0x7, 0xc0, 0xe, 0xe0, 0x2c, 0x34,
    0x34, 0x2c,

    /* U+005F "_" */
    0x0, 0x2, 0xff, 0xd0, 0x0, 0x0,

    /* U+0060 "`" */
    0x0, 0xf, 0x40, 0xb4, 0x0,

    /* U+0061 "a" */
    0x3f, 0xd0, 0x56, 0xc0, 0xb, 0xf, 0xfc, 0x70,
    0x71, 0xd6, 0xd2, 0xfb, 0x80,

    /* U+0062 "b" */
    0x34, 0x0, 0xd0, 0x3, 0x40, 0xf, 0xf8, 0x39,
    0x78, 0xd0, 0xf3, 0x42, 0xcd, 0xf, 0x39, 0xb8,
    0xff, 0x80,

    /* U+0063 "c" */
    0x1f, 0xe3, 0xd4, 0x74, 0x7, 0x0, 0x74, 0x3,
    0xd4, 0x1f, 0xe0,

    /* U+0064 "d" */
    0x0, 0x38, 0x0, 0xe0, 0x3, 0x87, 0xfe, 0x3d,
    0x79, 0xd0, 0xe7, 0x3, 0x9c, 0xe, 0x38, 0x78,
    0x7f, 0xe0,

    /* U+0065 "e" */
    0xf, 0xd0, 0xe6, 0xc7, 0x3, 0x5f, 0xfe, 0x74,
    0x0, 0xe5, 0x41, 0xff, 0x0,

    /* U+0066 "f" */
    0xf, 0x8b, 0x42, 0xc2, 0xfe, 0x3d, 0xf, 0x3,
    0xc0, 0xf0, 0x3c, 0xf, 0x0,

    /* U+0067 "g" */
    0x1f, 0xfc, 0x39, 0xb4, 0x34, 0x70, 0x2f, 0xe0,
    0x28, 0x0, 0x3d, 0x40, 0x2f, 0xf8, 0x34, 0x2c,
    0x78, 0x7c, 0x2f, 0xe0,

    /* U+0068 "h" */
    0x34, 0x0, 0xd0, 0x3, 0x40, 0xf, 0xf8, 0x3d,
    0x78, 0xd0, 0xe3, 0x42, 0xcd, 0xb, 0x34, 0x2c,
    0xd0, 0xb0,

    /* U+0069 "i" */
    0x34, 0x90, 0xd, 0x34, 0xd3, 0x4d, 0x34, 0xd0,

    /* U+006A "j" */
    0xd, 0x9, 0x0, 0xd, 0xd, 0xd, 0xd, 0xd,
    0xd, 0xd, 0xd, 0x3c, 0x20,

    /* U+006B "k" */
    0x34, 0x0, 0xd0, 0x3, 0x40, 0xd, 0x1d, 0x34,
    0xe0, 0xeb, 0x3, 0xfc, 0xe, 0x74, 0x34, 0xf0,
    0xd0, 0xe0,

    /* U+006C "l" */
    0xee, 0xee, 0xee, 0xee, 0xee,

    /* U+006D "m" */
    0x3f, 0xeb, 0xf0, 0xe1, 0xf5, 0xe3, 0x43, 0x82,
    0xcd, 0xe, 0xb, 0x34, 0x38, 0x2c, 0xd0, 0xe0,
    0xb3, 0x43, 0x82, 0xc0,

    /* U+006E "n" */
    0x3f, 0xe0, 0xe1, 0xe3, 0x43, 0x8d, 0xb, 0x34,
    0x2c, 0xd0, 0xb3, 0x42, 0xc0,

    /* U+006F "o" */
    0x1f, 0xe0, 0xf5, 0xd7, 0x3, 0x9c, 0xb, 0x70,
    0x38, 0xe5, 0xd1, 0xfe, 0x0,

    /* U+0070 "p" */
    0x3f, 0xe0, 0xe1, 0xe3, 0x42, 0xcd, 0xb, 0x34,
    0x3c, 0xe6, 0xe3, 0xfe, 0xd, 0x0, 0x34, 0x0,
    0xd0, 0x0,

    /* U+0071 "q" */
    0x1f, 0xf4, 0xf5, 0xe7, 0x43, 0x9c, 0xe, 0x70,
    0x38, 0xf5, 0xe1, 0xff, 0x80, 0xe, 0x0, 0x38,
    0x0, 0xe0,

    /* U+0072 "r" */
    0x0, 0xe, 0xb3, 0xe4, 0xd0, 0x34, 0xd, 0x3,
    0x40, 0xd0,

    /* U+0073 "s" */
    0x2f, 0xe1, 0xe5, 0x47, 0x80, 0xb, 0xf4, 0x0,
    0xb0, 0x57, 0xc3, 0xfd, 0x0,

    /* U+0074 "t" */
    0x3c, 0xf, 0xb, 0xf8, 0xf4, 0x3c, 0xf, 0x2,
    0xc0, 0xb4, 0xf, 0x80,

    /* U+0075 "u" */
    0x34, 0x38, 0xd0, 0xe3, 0x43, 0x8d, 0xe, 0x34,
    0x38, 0xe1, 0xe1, 0xff, 0x80,

    /* U+0076 "v" */
    0xb0, 0x35, 0xc1, 0xc3, 0x4b, 0xe, 0x38, 0x2c,
    0xd0, 0x7b, 0x0, 0xfc, 0x0,

    /* U+0077 "w" */
    0xb0, 0xb4, 0x35, 0xc3, 0xe1, 0xc3, 0x4f, 0x8b,
    0xe, 0x37, 0x38, 0x29, 0xcc, 0xd0, 0x7e, 0x3b,
    0x0, 0xf4, 0xbc, 0x0,

    /* U+0078 "x" */
    0x70, 0xb0, 0xe3, 0x41, 0xf8, 0x3, 0xd0, 0x1f,
    0xc0, 0xe3, 0x47, 0xb, 0x0,

    /* U+0079 "y" */
    0xb0, 0x39, 0xc1, 0xc3, 0x4b, 0xe, 0x38, 0x2c,
    0xd0, 0x7b, 0x0, 0xfc, 0x0, 0xe0, 0x3, 0x40,
    0x1c, 0x0,

    /* U+007A "z" */
    0x7f, 0xf1, 0x6e, 0x3, 0xc0, 0xb0, 0x2d, 0x7,
    0xd5, 0x7f, 0xf0,

    /* U+007B "{" */
    0x6, 0x43, 0xc1, 0xc0, 0x30, 0x1d, 0x1f, 0xb,
    0x40, 0xb0, 0xd, 0x7, 0x0, 0xd0, 0x2d, 0x0,
    0x0,

    /* U+007C "|" */
    0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0,

    /* U+007D "}" */
    0x64, 0xf, 0x0, 0xd0, 0x30, 0x1d, 0x3, 0xd0,
    0x78, 0x38, 0x1c, 0x3, 0x41, 0xc1, 0xe0, 0x0,
    0x0,

    /* U+007E "~" */
    0x0, 0x0, 0xbd, 0x22, 0x6f, 0x80, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 49, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 60, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 5, .adv_w = 88, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 10, .adv_w = 125, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 28, .adv_w = 125, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 54, .adv_w = 125, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 76, .adv_w = 156, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 50, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 104, .adv_w = 65, .box_w = 4, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 116, .adv_w = 65, .box_w = 4, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 128, .adv_w = 95, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 136, .adv_w = 125, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 150, .adv_w = 55, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 153, .adv_w = 94, .box_w = 6, .box_h = 3, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 158, .adv_w = 54, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 160, .adv_w = 99, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 177, .adv_w = 125, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 125, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 208, .adv_w = 125, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 224, .adv_w = 125, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 240, .adv_w = 125, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 258, .adv_w = 125, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 278, .adv_w = 125, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 125, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 313, .adv_w = 125, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 125, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 349, .adv_w = 54, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 60, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 360, .adv_w = 125, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 373, .adv_w = 125, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 384, .adv_w = 125, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 395, .adv_w = 99, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 409, .adv_w = 218, .box_w = 14, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 455, .adv_w = 134, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 476, .adv_w = 137, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 492, .adv_w = 122, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 510, .adv_w = 144, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 528, .adv_w = 124, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 544, .adv_w = 118, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 560, .adv_w = 137, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 578, .adv_w = 151, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 596, .adv_w = 60, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 601, .adv_w = 66, .box_w = 4, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 612, .adv_w = 131, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 628, .adv_w = 106, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 642, .adv_w = 190, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 665, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 683, .adv_w = 147, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 704, .adv_w = 132, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 720, .adv_w = 147, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 745, .adv_w = 138, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 763, .adv_w = 122, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 781, .adv_w = 118, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 799, .adv_w = 146, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 817, .adv_w = 133, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 838, .adv_w = 204, .box_w = 13, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 868, .adv_w = 126, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 886, .adv_w = 123, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 904, .adv_w = 120, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 920, .adv_w = 77, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 934, .adv_w = 106, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 954, .adv_w = 77, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 968, .adv_w = 125, .box_w = 8, .box_h = 5, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 978, .adv_w = 140, .box_w = 7, .box_h = 3, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 984, .adv_w = 57, .box_w = 5, .box_h = 4, .ofs_x = -1, .ofs_y = 7},
    {.bitmap_index = 989, .adv_w = 114, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1002, .adv_w = 120, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1020, .adv_w = 99, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1031, .adv_w = 121, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1049, .adv_w = 114, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1062, .adv_w = 78, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1075, .adv_w = 118, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1095, .adv_w = 122, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1113, .adv_w = 54, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1121, .adv_w = 54, .box_w = 4, .box_h = 13, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 1134, .adv_w = 112, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1152, .adv_w = 57, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1157, .adv_w = 188, .box_w = 11, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1177, .adv_w = 122, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1190, .adv_w = 119, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1203, .adv_w = 120, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1221, .adv_w = 120, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1239, .adv_w = 81, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1249, .adv_w = 105, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1262, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1274, .adv_w = 122, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1287, .adv_w = 110, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1300, .adv_w = 172, .box_w = 11, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1320, .adv_w = 104, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1333, .adv_w = 110, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1351, .adv_w = 102, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1362, .adv_w = 80, .box_w = 5, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1379, .adv_w = 56, .box_w = 2, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1386, .adv_w = 80, .box_w = 5, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1403, .adv_w = 125, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 2}
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
    0, -5, 0, 0, 0, -30, -15, -14,
    0, 0, -2, -8, 0, 0, 0, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, -4, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -10, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, -1, 0, 0, -8, -1, -6,
    -4, 0, -11, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, -1, -1, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -2, 0, -4, -4, -2, 0,
    4, -2, -2, 0, -3, -2, -2, 0,
    -2, 0, -3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -7, 0, 0, 0, -3,
    0, 2, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, -4, -4, 0, -3,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -30, 0, 0, 0,
    0, 0, -3, 0, 0, 0, 0, 0,
    0, -1, -1, 0, 0, -13, -2, -10,
    -7, 0, -16, 0, 0, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 0, -4,
    0, -6, -5, 0, -7, 0, 0, 0,
    -15, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, -5,
    -5, -13, 0, -6, -4, -8, -15, -5,
    0, 0, -1, 0, 0, 0, -2, 0,
    0, 0, 0, -3, 0, -2, -1, -6,
    -3, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -65, 0, 0, 0, -9,
    0, -2, -2, -4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -6, 0,
    -8, -8, 0, -7, 0, -5, -6, 0,
    -4, -2, -2, 0, -2, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -11, 0, -3, 0, 0, -9, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, -2, 0, -1,
    0, 0, -6, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -8, 0, 0, 0, -7, 0, -1, 0,
    0, -4, 0, 0, 0, -2, -2, 0,
    -2, -9, -1, -6, -4, 0, -11, 0,
    -10, -2, 0, 0, -1, -1, -2, -2,
    0, 0, 0, -3, 0, -4, -3, 0,
    -4, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, -2,
    0, 0, 0, -2, 0, -2, 0, -2,
    0, -3, -5, 0, -3, -4, 0, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, 0, 0, 0, 0, 0, -5, 0,
    0, 0, 0, 0, 0, -1, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, -2, -1, -3,
    0, 0, 0, 0, -2, -2, -2, 0,
    -2, 0, 0, 0, 0, 0, 0, -3,
    0, -2, 0, -2, 0, -1, 0, -3,
    0, 0, 0, -3, 0, -3, 0, -2,
    0, -4, -6, -1, -3, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -4,
    0, 0, 0, 0, 0, 0, -4, 0,
    0, 0, 0, 0, 0, -1, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, -2, 0, -3,
    0, 0, 0, -1, -1, -1, -2, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, -13, -2, -8, 0, 0, 0, -6,
    0, -1, -1, -3, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -5, 0,
    -4, -4, -1, -5, 0, -4, -4, -1,
    -3, -1, -2, -3, -2, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, -2, 0, 0, -4, 0,
    -2, 0, 0, 0, 0, 0, -1, -1,
    0, 0, 0, 0, 0, -1, -2, 0,
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
    0, 0, -7, 0, 0, 0, 0, 0,
    0, -2, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    -4, -3, -2, -3, 0, -1, 0, -2,
    -3, -4, -5, 0, -5, 0, 0, 0,
    -18, 0, 0, 0, -18, 0, -13, 0,
    0, -1, 0, 0, 0, -2, -3, 0,
    0, -19, -2, -13, -10, 0, -19, 0,
    -16, -1, 0, 0, 0, 0, -1, -1,
    0, 0, 0, -3, 0, -9, -6, 0,
    -9, 0, 0, -1, 0, 0, 0, -2,
    0, -1, 0, -2, 0, -1, 0, -2,
    0, 0, 0, -2, 0, -3, 0, -2,
    0, -4, -6, 0, -3, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -4,
    0, 0, 0, -2, 0, -16, -1, -8,
    0, 0, 0, -5, 0, 0, 0, -4,
    0, 0, 0, -2, 0, -4, -4, -1,
    -2, -4, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -3, 0, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, -2,
    0, 0, 0, 0, 0, -1, 0, -2,
    0, -1, -5, 0, -2, -2, -1, 0,
    -2, -2, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, 0, -1,
    0, 0, 0, -2, 0, -1, -3, 0,
    0, 0, 0, 0, 0, 0, -2, -2,
    0, 0, 0, -2, 0, -2, -2, -2,
    -2, 0, 0, 0, 0, -3, 0, 0,
    0, -13, -13, -11, -11, 0, -4, -9,
    0, -1, -3, -3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -18, -1,
    -18, -17, -3, -19, 0, -15, -17, -4,
    -15, -13, -13, -13, -13, -17, 0, 0,
    0, 0, 0, 0, 0, -2, 0, -3,
    0, 0, 0, -1, 0, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -1, -1, -1, -2,
    0, -1, -1, 0, -1, 0, 0, 0,
    0, -1, 0, 0, 0, -4, 0, 0,
    0, -10, -6, -9, -3, 0, -3, -6,
    0, -2, -2, -4, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -6, 0,
    -7, -7, -1, -7, 0, -5, -5, 0,
    -4, -2, -2, -2, -2, -3, 0, 0,
    0, -1, 0, 0, 0, -7, -4, -6,
    0, 0, 0, -4, 0, 0, 0, -3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -3, 0, -4, -4, 0, -5,
    0, -3, -3, 0, -2, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, 0, -8, 0, 0, 0, 0, 0,
    0, -4, -4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    -5, -4, -2, -4, 0, -1, 0, -2,
    -4, -5, -6, 0, -6, 0, 0, 0,
    0, -7, 0, 0, 1, -16, -15, -15,
    -9, 0, -8, -11, 0, -6, -6, -6,
    -4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -14, 0, -16, -15, -5, -16,
    0, -13, -16, -4, -12, -8, -9, -8,
    -9, -11, 0, 0, 0, 0, 0, 0,
    0, 0, -5, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, -2, -1, -3, 0, -1, 0, 0,
    -2, -2, -2, 0, -2, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, -4, -5, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -5, 0, -7, -7, -3, 0,
    2, -2, -2, -4, -6, -6, -6, -2,
    -5, -2, -4, 0, -16, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, -3, 0, -2, -13, -4, -10,
    -7, 0, -16, 0, 0, 0, 0, 0,
    -2, 0, -2, 0, 0, 0, 0, -3,
    0, -5, -4, 0, -5, 0, 0, 0,
    -2, 0, 0, 0, -1, 0, 0, 0,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, -16, -2, -6, -4, 0, -15, 0,
    -9, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, -1, 0,
    -1, 0, 0, -1, -3, 0, 0, -4,
    -2, 0, 0, 0, 0, -4, 0, -1,
    -1, 0, 0, -4, -1, -17, -1, -6,
    -4, -5, -15, -2, -9, -7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, -2, -2, 0, 0, -6,
    0, 0, 0, 0, 0, 0, -6, 0,
    0, -1, 0, 0, 0, 0, 0, -1,
    0, -19, 0, -3, -1, 0, -11, 0,
    -4, -2, 0, 0, -1, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, -2, 0, 0, 0, 0,
    0, -3, 0, 0, 0, 0, 0, -2,
    0, -18, -1, -6, -4, -1, -18, -1,
    -8, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    -2, 0, 0, -4, 0, -1, 0, 0,
    0, -8, -8, -6, 0, 0, 0, -5,
    0, 0, 0, -3, 0, -3, 0, 0,
    0, -1, 1, -1, 0, 0, 0, 0,
    -1, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -13, 0, -1, 0, 0, -7, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, -2,
    -1, 0, 0, 0, 0, -4, 0, 0,
    0, 0, 0, -2, -1, -18, -1, -6,
    -4, 0, -15, -1, -9, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, -1, 0, -1, 0, 0, -2,
    0, 0, 0, 0, 0, 0, -6, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, -14, -1, -2, 0, 0, -9, 0,
    -3, -2, 0, 0, -2, -2, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, -3, 0, 0, -4,
    -1, 0, 0, 0, 0, -4, 0, -1,
    -1, 0, 0, -4, -2, -18, -1, -7,
    -4, -5, -16, -2, -9, -7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, -2, -2, 0, 0, -6,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, -2,
    0, -15, -1, -5, -3, -1, -13, -1,
    -6, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, -1, 0, -2,
    0, -12, -8, -7, 0, 0, 0, -6,
    0, 0, 0, -5, 0, -12, 0, 0,
    0, -6, -5, -2, -2, -5, 0, 0,
    -1, -1, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -4,
    0, 0, 0, -2, 0, 0, -1, 0,
    0, -1, 0, 0, 0, 0, 0, -2,
    0, -17, 0, -4, -3, -1, -12, 0,
    -6, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    -2, 0, 0, -4, 0, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -13, 0, -1,
    0, 0, -7, 0, -3, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, -2, 0, -6, -2, -4,
    0, -1, 0, -4, 0, 0, 0, -4,
    0, -13, 0, -2, 0, -5, -8, -2,
    -3, -6, -2, 0, -2, -2, 0, -2,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, -4, 0, 0, 0, -2,
    0, -5, -1, -3, 0, -1, 0, -3,
    -1, 0, 0, -4, 0, -13, 0, -2,
    0, -5, -9, -3, -3, -6, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 0, 0, 0, -6, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -13, 0, -1, 0, 0, -8, 0,
    -2, -2, 0, 0, -2, -2, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, -7, -3, -4, 0, -1, 0, -4,
    0, 0, 0, -4, 0, -13, 0, -2,
    0, -5, -8, -2, -3, -5, -2, 0,
    -2, -2, 0, -2, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, -4,
    0, 0, 0, 0, 0, 0, -5, 0,
    0, -1, 0, 0, 0, 0, 0, -1,
    0, -17, -1, -3, -1, 0, -11, 0,
    -4, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, -4, -4, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, 0,
    -6, -6, -1, 0, 4, -2, -2, -2,
    -5, -4, -5, -2, -4, -2, -3, 0,
    0, 0, 0, -3, 0, 0, 0, 0,
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

extern const lv_font_t lv_font_montserrat_14;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t titillium_14 = {
#else
lv_font_t titillium_14 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
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
    .fallback = &lv_font_montserrat_14,
#endif
    .user_data = NULL,
};



#endif /*#if TITILLIUM_14*/


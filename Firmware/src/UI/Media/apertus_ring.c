
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#elif defined(LV_BUILD_TEST)
#include "../lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_APERTUS_RING
#define LV_ATTRIBUTE_APERTUS_RING
#endif

static const
LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_APERTUS_RING
uint8_t apertus_ring_map[] = {

    0x48,0x72,0xf4,0xe4,0x49,0x72,0xf4,0x0b,

    0xf0,0x3c,
    0xe0,0x1c,
    0xc0,0x0c,
    0x80,0x04,
    0x07,0x80,
    0x0f,0xc0,
    0x0f,0xc0,
    0x1f,0xc0,
    0x0f,0xc0,
    0x07,0x80,
    0x82,0x04,
    0xc0,0x0c,
    0xe0,0x1c,
    0xf0,0x3c,

};

const lv_image_dsc_t apertus_ring = {
  .header.magic = LV_IMAGE_HEADER_MAGIC,
  .header.cf = LV_COLOR_FORMAT_I1,
  .header.flags = 0,
  .header.w = 14,
  .header.h = 14,
  .header.stride = 2,
  .data_size = sizeof(apertus_ring_map),
  .data = apertus_ring_map,
};


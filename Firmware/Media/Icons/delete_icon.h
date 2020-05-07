#include <stdint.h>

static const struct
{
	    uint16_t width;
	    uint16_t height;
		  uint8_t delete_data[24 * 24 + 1];
} deleteB = {
	    24, 24,
	    {   0x00, 0x7E, 0x00, 0x00, 0xC3, 0x00, 0x80, 0x81, 0x01, 0xF0, 0xFF, 0x0F, 
          0xF0, 0xFF, 0x0F, 0xF0, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x0F, 
          0xF0, 0xFF, 0x0F, 0xF0, 0x7E, 0x0F, 0xE0, 0x66, 0x07, 0xE0, 0x66, 0x07, 
          0xE0, 0x24, 0x07, 0xE0, 0x24, 0x07, 0xE0, 0x24, 0x07, 0xE0, 0x24, 0x07, 
          0xE0, 0x24, 0x07, 0xE0, 0x24, 0x07, 0xE0, 0x24, 0x07, 0xE0, 0x24, 0x07, 
          0xE0, 0x24, 0x07, 0xE0, 0xFF, 0x07, 0xE0, 0xFF, 0x07, 0xC0, 0xFF, 0x03}
};

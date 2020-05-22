#include <stdint.h>

#include "../../UI/Widgets/Icon.h"
 
static struct down_icon : public Icon {
 
    uint8_t data[24 * 24] = {
0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0x00, 0xFF, 0x01, 0xFC, 0xFF, 0x3F, 0xFC, 0xFF, 0x3F, 0xF8, 0xFF, 0x1F, 0xF0, 0xFF, 0x0F, 0xE0, 0xFF, 0x07, 0xC0, 0xFF, 0x03, 0x80, 0xFF, 0x01, 0x00, 0xFF, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x18, 0x00, };
 
    down_icon()
    {
        Icon::Width = 24;
        Icon::Height = 24;
        Icon::Data = &data[0];
    }
}down_icon;

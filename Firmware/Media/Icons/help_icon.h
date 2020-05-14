#include <stdint.h>

#include "../../UI/Widgets/Icon.h"
 
struct IconName : public Icon {
 
    uint8_t data[24 * 24] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x80, 0x07, 0x00, 0xC0, 0x07, 0x00, 0xE0, 0x07, 0x00, 0xF0, 0x07, 0x00, 0xF8, 0x3F, 0x00, 0xFC, 0xFF, 0x01, 0xFE, 0xFF, 0x07, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0x1F, 0xFE, 0xFF, 0x3F, 0xFC, 0xFF, 0x3F, 0xF8, 0xFF, 0x7F, 0xF0, 0x07, 0x7F, 0xE0, 0x07, 0xF0, 0xC0, 0x07, 0xC0, 0x80, 0x07, 0x80, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
 
    IconName()
    {
        Icon::Width = 24;
        Icon::Height = 24;
        Icon::Data = &data[0];
    }
};

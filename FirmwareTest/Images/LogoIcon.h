#include <stdint.h>

static const struct LogoIcon : Icon
{
    // uint16_t width;
    // uint16_t height;
    uint8_t data[10 * 6] = {0xFF, 0x03, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0xFF, 0x03};

    LogoIcon()
    {
        Icon::Width = 10;
        Icon::Height = 6;
        Icon::Data = &data[0];
    }

} LogoIcon;

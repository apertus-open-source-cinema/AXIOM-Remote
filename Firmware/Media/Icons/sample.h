#include <stdint.h>

#include "../../UI/Widgets/Icon.h"
 
struct IconName : public Icon {
 
    uint8_t data[width * height] = {image_data};
 
    IconName()
    {
        Icon::Width = width;
        Icon::Height = height;
        Icon::Data = &data[0];
    }
};

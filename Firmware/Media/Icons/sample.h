#include <stdint.h>

#include "../../UI/Widgets/Icon.h"
 
static struct IconName : public Icon {
 
    uint8_t data[%WIDTH% * %HEIGHT%] = {%IMAGE_DATA%};
 
    IconName()
    {
        Icon::Width = %WIDTH%;
        Icon::Height = %HEIGHT%;
        Icon::Data = &data[0];
    }
}icon;

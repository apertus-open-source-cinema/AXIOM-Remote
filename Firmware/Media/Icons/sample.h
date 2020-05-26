#ifndef SAMPLE_H
#define SAMPLE_H

#include "../../UI/Widgets/Icon.h"

static const struct IconName : public Icon
{
    uint8_t data[%WIDTH% * %HEIGHT%] = {%IMAGE_DATA%};

    IconName()
    {
        Icon::Width = %WIDTH%;
        Icon::Height = %HEIGHT%;
        Icon::Data = &data[0];
    }
}icon STATIC_SECTION;

#endif // SAMPLE_H

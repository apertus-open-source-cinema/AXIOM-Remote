#ifndef IUSBDEVICE_H
#define IUSBDEVICE_H

#include <stdint.h>

#define UNUSED(x) (void)(x)

class IUSBDevice
{
public:
    virtual void Send(uint8_t *data, uint16_t length)
    {
        UNUSED(data);
        UNUSED(length);
    }
};

#endif //IUSBDEVICE_H
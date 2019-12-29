#ifndef IUSBDEVICE_H
#define IUSBDEVICE_H

#include <stdint.h>

class IUSBDevice
{
public:
    virtual void Send(uint8_t *data, uint16_t length)
    {
    }
};

#endif //IUSBDEVICE_H
#ifndef USBCDCDEVICE_H
#define USBCDCDEVICE_H

#include <inttypes.h>

class USBCDCDevice
{
public:
    void Initialize();

    void Process();

    void Send(uint8_t *data, uint16_t length);
};

#endif //USBCDCDEVICE_H

#ifndef USBCDCDEVICE_H
#define USBCDCDEVICE_H

#include <stdint.h>

#include "IUSBDevice.h"

class USBCDCDevice : public IUSBDevice
{
public:
    void Initialize();

    void Process();

    void Send(uint8_t *data, uint16_t length) override;
};

#endif //USBCDCDEVICE_H

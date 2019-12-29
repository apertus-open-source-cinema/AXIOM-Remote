#ifndef USBCDCTERMINALDEVICE_H
#define USBCDCTERMINALDEVICE_H

#include "../Bootloader/Periphery/USB/IUSBDevice.h"

class USBCDCTerminalDevice : public IUSBDevice
{
public:
    void Send(uint8_t *data, uint16_t length) override
    {
        std::string dataString((char*)data, length);
        std::cout << "UART: " << dataString << std::endl;
    }
};

#endif //USBCDCTERMINALDEVICE_H
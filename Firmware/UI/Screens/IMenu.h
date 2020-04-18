#ifndef IMENU_H
#define IMENU_H

#include <stdint.h>

#include "IScreen.h"

#include "../../Utils.h"
#include "../Color565.h"

class IMenu : public IScreen
{
  public:
    IMenu(IUSBDevice* usbDevice = nullptr) : IScreen(usbDevice)
    {
    }
};

#endif /* IMENU_H */

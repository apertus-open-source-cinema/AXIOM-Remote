#ifndef WHITEBALANCESCREEN_H
#define WHITEBALANCESCREEN_H

#include "IScreen.h"
#include "../Widgets/PushButton.h"

class IPainter;
class IUSBDevice;

class WhiteBalanceScreen : public IScreen
{
    PushButton _homeButton;
    PushButton _testButton;

  public:
    explicit WhiteBalanceScreen(IUSBDevice* usbDevice);

    virtual void Draw(IPainter* painter);

    static void HomeButtonHandler(void* sender);
    static void TestButtonHandler(void* sender);

    void Update(Button button, int8_t knob, IMenuSystem* menuSystem);
};

#endif // WHITEBALANCESCREEN_H
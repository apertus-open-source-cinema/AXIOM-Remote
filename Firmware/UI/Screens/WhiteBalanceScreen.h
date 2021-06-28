#ifndef WHITEBALANCESCREEN_H
#define WHITEBALANCESCREEN_H

#include "IScreen.h"
#include "../Widgets/PushButton.h"
#include "../Widgets/ImageButton.h"

class IPainter;
class IUSBDevice;
enum class ButtonID;

class WhiteBalanceScreen : public IScreen
{
    PushButton _cancelButton;
    PushButton _setButton;
    PushButton _addPresetButton;

  public:
    explicit WhiteBalanceScreen(IUSBDevice* usbDevice);

    virtual void Draw(IPainter* painter) override;

    static void CancelButtonHandler(void* sender);
    static void SetButtonHandler(void* sender);
    static void AddPresetButtonHandler(void* sender);
    void Drawheader(IPainter* painter);

    void Update(ButtonID button, int8_t knob, IMenuSystem* menuSystem) override;
};

#endif // WHITEBALANCESCREEN_H

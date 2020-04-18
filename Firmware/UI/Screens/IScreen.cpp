#include "IScreen.h"

#include "../Color565.h"

IScreen::IScreen(IUSBDevice* usbDevice) : _usbDevice(usbDevice), _bottomButtonBar(ButtonBar(0, 210, 320, 30))
{
}

IScreen::~IScreen()
{
}

Color565 IScreen::GetBackgroundColor()
{
    return Color565::MenuBackground;
}

void IScreen::Draw(IPainter* painter)
{
    UNUSED(painter);
}

void IScreen::Update(Button button, int8_t knob, IMenuSystem* menuSystem)
{
    UNUSED(button);
    UNUSED(knob);
    UNUSED(menuSystem);
}

void IScreen::DrawCaption()
{
}

void IScreen::DrawTopButtonBar()
{
}

void IScreen::DrawBottomButtonBar(IPainter* painter)
{
    _bottomButtonBar.Draw(painter);
}

void IScreen::SetBottomButton(ButtonPosition position, IButton* button)
{
    _bottomButtonBar.SetButton(position, button);
}

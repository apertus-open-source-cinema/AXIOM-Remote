#include "IScreen.h"

#include "../Color565.h"

#include "../../Media/Icons/home_icon.h"

IScreen::IScreen(IUSBDevice* usbDevice) :
    _usbDevice(usbDevice), _bottomButtonBar(ButtonBar(0, 210, 320, 30)), _leftButtonBar(0, 31, 34, 180),
    _homeButton(&home_icon)
{
    _leftButtonBar.SetButton(ButtonPosition::Left, &_homeButton);
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

void IScreen::Update(ButtonID button, int8_t knob, IMenuSystem* menuSystem)
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

void IScreen::DrawLeftButtonBar(IPainter* painter)
{
    _leftButtonBar.Draw(painter);
}

void IScreen::SetBottomButton(ButtonPosition position, IButton* button)
{
    _bottomButtonBar.SetButton(position, button);
}

void IScreen::SetLeftButton(ButtonPosition position, IButton* button)
{
    _leftButtonBar.SetButton(position, button);
}

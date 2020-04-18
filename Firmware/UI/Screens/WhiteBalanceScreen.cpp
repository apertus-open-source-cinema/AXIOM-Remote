#include "WhiteBalanceScreen.h"

#include "../MenuDefinitions.h"
#include "../ButtonDefinitions.h"

#include "../../../Bootloader/Periphery/USB/IUSBDevice.h"
#include "../IMenuSystem.h"

WhiteBalanceScreen::WhiteBalanceScreen(IUSBDevice* usbDevice) :
    IScreen(usbDevice), _homeButton("Home"), _testButton("Test")
{
    _homeButton.SetHandler(&HomeButtonHandler);
    _bottomButtonBar.SetButton(ButtonPosition::Left, &_homeButton);

    _testButton.SetCornerRadius(10);
    _testButton.SetHandler(&TestButtonHandler);
    _bottomButtonBar.SetButton(ButtonPosition::Right, &_testButton);
}

void WhiteBalanceScreen::Draw(IPainter* painter)
{
    painter->DrawFillRectangle(20, 20, 70, 30, (uint16_t)Color565::Yellow);

    DrawBottomButtonBar(painter);
}

// TODO: Evaluate if menu system should be sent as another argument
void WhiteBalanceScreen::HomeButtonHandler(void* sender)
{
    WhiteBalanceScreen* screen = static_cast<WhiteBalanceScreen*>(sender);
    // IMenuSystem* menuSystem = screen->GetMenuSystem();
    // menuSystem->SetCurrentScreen(AvailableScreens::MainPage);
}

void WhiteBalanceScreen::TestButtonHandler(void* sender)
{
    WhiteBalanceScreen* screen = static_cast<WhiteBalanceScreen*>(sender);
    // IMenuSystem* menuSystem = screen->GetMenuSystem();
    // menuSystem->SetCurrentScreen(AvailableScreens::SettingsMenu);
}

// TODO: Check the handling of buttons and provide general example
void WhiteBalanceScreen::Update(Button button, int8_t knob, IMenuSystem* menuSystem)
{
    switch (button)
    {
    case Button::BUTTON_4_UP:
        menuSystem->SetCurrentScreen(AvailableScreens::MainPage);
        break;
    case Button::BUTTON_6_UP:
        menuSystem->SetCurrentScreen(AvailableScreens::SettingsMenu);
        break;
    default:
        break;
    }
}
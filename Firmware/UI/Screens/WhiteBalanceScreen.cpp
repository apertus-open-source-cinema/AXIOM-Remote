#include "WhiteBalanceScreen.h"

#include "../MenuDefinitions.h"
#include "../ButtonDefinitions.h"

//#include "Periphery/USB/IUSBDevice.h"
#include "../IMenuSystem.h"

#include "../../GlobalSettings.h"

class IUSBDevice;

WhiteBalanceScreen::WhiteBalanceScreen(IUSBDevice* usbDevice) :
    IScreen(usbDevice), _cancelButton("Cancel"), _setButton("Set"), _addPresetButton("Add")
{
    _cancelButton.SetHandler(&CancelButtonHandler);
    _bottomButtonBar.SetButton(ButtonPosition::Left, &_cancelButton);

    _addPresetButton.SetHandler(&AddPresetButtonHandler);
    _bottomButtonBar.SetButton(ButtonPosition::Center, &_addPresetButton);

    // This is the primary button in this menu
    _setButton.SetHandler(&SetButtonHandler);
    _setButton.SetBackgroundColor((uint16_t)Color565::AXIOM_Orange);
    _bottomButtonBar.SetButton(ButtonPosition::Right, &_setButton);

    //_leftButtonBar.SetButton(ButtonPosition::Left, &_homeButton); //already done in IScreen
}

void WhiteBalanceScreen::Draw(IPainter* painter)
{
    Drawheader(painter);

    // painter->DrawFillRectangle(20, 50, 70, 30, (uint16_t)Color565::Yellow);

    DrawBottomButtonBar(painter);

    DrawLeftButtonBar(painter);
}

void WhiteBalanceScreen::Drawheader(IPainter* painter)
{
    // header background
    painter->DrawFillRectangle(0, 0, GlobalSettings::LCDWidth, 30, (uint16_t)Color565::Black);

    //  header title
    painter->SetFont(Font::FreeSans12pt7b);
    painter->DrawText(86, 22, "White Balance", (uint16_t)Color565::White, TextAlign::TEXT_ALIGN_LEFT, 0);

    // header separation lines
    painter->DrawLine(0, 29, GlobalSettings::LCDWidth - 1, 29, (uint16_t)Color565::AXIOM_Orange);
}

// TODO: Evaluate if menu system should be sent as another argument
void WhiteBalanceScreen::CancelButtonHandler(void* sender)
{
    // WhiteBalanceScreen* screen = static_cast<WhiteBalanceScreen*>(sender);
    // IMenuSystem* menuSystem = screen->
    // menuSystem->SetCurrentScreen(AvailableScreens::MainPage);
}

void WhiteBalanceScreen::SetButtonHandler(void* sender)
{
    // WhiteBalanceScreen* screen = static_cast<WhiteBalanceScreen*>(sender);
    // IMenuSystem* menuSystem = screen->GetMenuSystem();
    // menuSystem->SetCurrentScreen(AvailableScreens::SettingsMenu);
}

void WhiteBalanceScreen::AddPresetButtonHandler(void* sender)
{
}

// TODO: Check the handling of buttons and provide general example
void WhiteBalanceScreen::Update(Button button, int8_t knob, IMenuSystem* menuSystem)
{
    switch (button)
    {
    case Button::BUTTON_4_DOWN:
        _cancelButton.SetHighlighted(true);
        break;
    case Button::BUTTON_4_UP:
        _cancelButton.SetHighlighted(false);
        menuSystem->SetCurrentScreen(AvailableScreens::MainPage);
        break;
    case Button::BUTTON_5_DOWN:
        _addPresetButton.SetHighlighted(true);
        break;
    case Button::BUTTON_5_UP:
        _addPresetButton.SetHighlighted(false);
        break;
    case Button::BUTTON_6_DOWN:
        _setButton.SetHighlighted(true);
        break;
    case Button::BUTTON_6_UP:
        _setButton.SetHighlighted(false);
        menuSystem->SetCurrentScreen(AvailableScreens::MainPage);
        break;
    case Button::BUTTON_7_DOWN:
        _homeButton.SetHighlighted(true);
        break;
    case Button::BUTTON_7_UP:
        _homeButton.SetHighlighted(false);
        menuSystem->SetCurrentScreen(AvailableScreens::MainPage);
        break;
    default:
        break;
    }
}

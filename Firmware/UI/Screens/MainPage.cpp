#include "MainPage.h"

#include "../MenuDefinitions.h"
#include "../MenuSystem.h"

// TODO: Add assignment of menu system to IMenu
MainPage::MainPage(IUSBDevice* cdcDevice) : IMenu(cdcDevice), _backgroundColor(Color565::MenuBackground)
{
    SetupButtons();
}

void MainPage::SetupButtons()
{
    // Top row
    _fpsButton = MainPageButton(10, 0, 90, "FPS", false);

    _analogGainButton = MainPageButton(115, 0, 90, "A. Gain", false);
    _analogGainButton.SetHandler(&AnalogGainButtonHandler);

    _digitalGainButton = MainPageButton(220, 0, 90, "D. Gain", false),
    _digitalGainButton.SetHandler(&DigitalGainButtonHandler);

    // Bottom row
    _menuButton = MainPageButton(10, 210, 90, "MENU", true, ButtonType::BUTTON);
    _menuButton.SetLabelHeight(30);
    _menuButton.SetLabelFont(Font::FreeSans12pt7b);
    _menuButton.SetHandler(&MenuButtonHandler);

    _shutterButton = MainPageButton(115, 180, 90, "Shutter", true);
    _whiteBalanceButton = MainPageButton(220, 180, 90, "WB", true);
}

Color565 MainPage::GetBackgroundColor()
{
    return _backgroundColor;
}

void MainPage::SetBackgroundColor(Color565 color)
{
    _backgroundColor = color;
}

void MainPage::MenuButtonHandler(void* sender)
{
    MainPage* menu = static_cast<MainPage*>(sender);
    menu->SetBackgroundColor(Color565::Red);
}

void MainPage::AnalogGainButtonHandler(void* sender)
{
    MainPage* menu = static_cast<MainPage*>(sender);
    menu->SetBackgroundColor(Color565::Green);
}

void MainPage::DigitalGainButtonHandler(void* sender)
{
    MainPage* menu = static_cast<MainPage*>(sender);
    menu->SetBackgroundColor(Color565::MenuBackground);
}

void MainPage::Draw(IPainter* painter)
{
    painter->DrawIcon(&ApertusTextLogo, 58, 89, utils::RGB565(194, 191, 188));
    painter->DrawIcon(&ApertusRingLogo, 58 + ApertusTextLogo.Width, 89, utils::RGB565(244, 114, 72));

    for (uint8_t index = 0; index < 6; index++)
    {
        IWidget* widget = _widgetArray[index];
        if (widget == nullptr)
        {
            return;
        }

        widget->Draw(painter);
    }
}

void MainPage::Update(ButtonID button, int8_t knob, IMenuSystem* menuSystem)
{
    switch (button)
    {
    case ButtonID::BUTTON_1_UP:
        _fpsButton.SetValue((char*)"1U");
        _fpsButton.SetHighlighted(false);
        break;
    case ButtonID::BUTTON_1_DOWN:
        _fpsButton.SetValue((char*)"1D");
        _fpsButton.SetHighlighted(true);
        break;
    case ButtonID::BUTTON_2_UP:
        //_fpsButton.SetValue((char*)"2");
        _analogGainButton.SetHighlighted(false);
        _analogGainButton.Activate(this);
        // _usbDevice->Send((uint8_t*)"Button 2\r\n", 10);
        break;
    case ButtonID::BUTTON_2_DOWN:
        _analogGainButton.SetHighlighted(true);
        break;
    case ButtonID::BUTTON_3_UP:
        //_fpsButton.SetValue((char*)"3");
        _digitalGainButton.SetHighlighted(false);
        _digitalGainButton.Activate(this);
        break;
    case ButtonID::BUTTON_3_DOWN:
        _fpsButton.SetValue((char*)"3");
        _digitalGainButton.SetHighlighted(true);
        break;
    case ButtonID::BUTTON_4_UP:
        //_menuButton.Activate(this);
        _menuButton.SetHighlighted(false);
        menuSystem->SetCurrentScreen(AvailableScreens::MainMenu);
        break;
    case ButtonID::BUTTON_4_DOWN:
        _menuButton.SetHighlighted(true);
        break;
    case ButtonID::BUTTON_6_UP:
        //_menuButton.Activate(this);
        _whiteBalanceButton.SetHighlighted(false);
        menuSystem->SetCurrentScreen(AvailableScreens::WhiteBalance);
        break;
    case ButtonID::BUTTON_6_DOWN:
        _whiteBalanceButton.SetHighlighted(true);
        break;
    default:
        break;
    }

    /*if (knob < 0)
    {
        _usbDevice->Send((uint8_t*)"Knob \r\n", 10);
    }*/
}

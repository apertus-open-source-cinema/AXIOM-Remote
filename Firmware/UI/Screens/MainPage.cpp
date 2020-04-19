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

    _shutterButton = MainPageButton(115, 179, 90, "Shutter", true);
    _whiteBalanceButton = MainPageButton(220, 179, 90, "WB", true);
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
    /*painter->DrawIcon(apertus_text.pixel_data_text, 58, 89, apertus_text.width, apertus_text.height,
                      RGB565(194, 191, 188));
    painter->DrawIcon(apertus_ring.pixel_data_ring, 58 + apertus_text.width, 89, apertus_ring.width,
                      apertus_ring.height, RGB565(244, 114, 72));
                      */

    painter->SetFont(Font::FreeSans9pt7b);
    painter->DrawText(50, 80, "FreeSans9pt7b", (uint16_t)Color565::White, TextAlign::TEXT_ALIGN_LEFT, 0);
    painter->SetFont(Font::FreeSans12pt7b);
    painter->DrawText(50, 100, "FreeSans12pt7b", (uint16_t)Color565::White, TextAlign::TEXT_ALIGN_LEFT, 0);
    painter->SetFont(Font::FreeSans18pt7b);
    painter->DrawText(50, 130, "FreeSans18pt7b", (uint16_t)Color565::White, TextAlign::TEXT_ALIGN_LEFT, 0);
    painter->SetFont(Font::FreeSans24pt7b);
    painter->DrawText(50, 170, "FreeSans24pt7b", (uint16_t)Color565::White, TextAlign::TEXT_ALIGN_LEFT, 0);

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

void MainPage::Update(Button button, int8_t knob, IMenuSystem* menuSystem)
{
    switch (button)
    {
    case Button::BUTTON_1_UP:
        _fpsButton.SetValue((char*)"1U");
        _fpsButton.SetHighlighted(false);
        break;
    case Button::BUTTON_1_DOWN:
        _fpsButton.SetValue((char*)"1D");
        _fpsButton.SetHighlighted(true);
        break;
    case Button::BUTTON_2_UP:
        //_fpsButton.SetValue((char*)"2");
        _analogGainButton.SetHighlighted(false);
        _analogGainButton.Activate(this);
        // _usbDevice->Send((uint8_t*)"Button 2\r\n", 10);
        break;
    case Button::BUTTON_2_DOWN:
        _analogGainButton.SetHighlighted(true);
        break;
    case Button::BUTTON_3_UP:
        //_fpsButton.SetValue((char*)"3");
        _digitalGainButton.SetHighlighted(false);
        _digitalGainButton.Activate(this);
        break;
    case Button::BUTTON_3_DOWN:
        _fpsButton.SetValue((char*)"3");
        _digitalGainButton.SetHighlighted(true);
        break;
    case Button::BUTTON_4_UP:
        //_menuButton.Activate(this);
        _menuButton.SetHighlighted(false);
        menuSystem->SetCurrentScreen(AvailableScreens::SettingsMenu);
        break;
    case Button::BUTTON_4_DOWN:
        _menuButton.SetHighlighted(true);
        break;
    case Button::BUTTON_6_UP:
        //_menuButton.Activate(this);
        _whiteBalanceButton.SetHighlighted(false);
        menuSystem->SetCurrentScreen(AvailableScreens::WhiteBalance);
        break;
    case Button::BUTTON_6_DOWN:
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

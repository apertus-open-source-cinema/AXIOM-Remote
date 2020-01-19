#ifndef MainPage_H
#define MainPage_H

#include "IMenu.h"
#include "../Painter.h"
#include "../MenuSystem.h"

#include "../Widgets/MainPageButton.h"

#include "../../Media/Images/ApertusLogo.h"

#include "../ButtonDefinitions.h"

#include "../../../Bootloader/Periphery/USB/IUSBDevice.h"

class MenuSystem;

class MainPage : public IMenu
{
    IUSBDevice* _usbDevice;

    MainPageButton _fpsButton;
    MainPageButton _analogGainButton;
    MainPageButton _digitalGainButton;
    MainPageButton _menuButton;
    MainPageButton _shutterButton;
    MainPageButton _whiteBalanceButton;

    IButton* _widgetArray[6] = {&_fpsButton,  &_analogGainButton, &_digitalGainButton,
                                &_menuButton, &_shutterButton,    &_whiteBalanceButton};

    Color565 _backgroundColor;

  public:
    explicit MainPage(IUSBDevice* cdcDevice) :
        _usbDevice(cdcDevice), _fpsButton(MainPageButton(10, 179, 90, "FPS", true)),
        _analogGainButton(MainPageButton(115, 179, 90, "A. Gain", true)),
        _digitalGainButton(MainPageButton(220, 179, 90, "D. Gain", true)),
        _menuButton(MainPageButton(10, 0, 90, "MENU", false, ButtonType::BUTTON)),
        _shutterButton(MainPageButton(115, 0, 90, "Shutter")), _whiteBalanceButton(MainPageButton(220, 0, 90, "WB")),
        _backgroundColor(Color565::MenuBackground)
    {
        _menuButton.SetLabelHeight(30);
        //_menuButton.HideValue(true);
        _menuButton.SetLabelFont(Font::FreeSans12pt7b);

        _menuButton.SetHandler(&MenuButtonHandler);
        _analogGainButton.SetHandler(&AnalogGainButtonHandler);
        _digitalGainButton.SetHandler(&DigitalGainButtonHandler);
    }

  protected:
    virtual Color565 GetBackgroundColor() override
    {
        return _backgroundColor;
    }

    virtual void SetBackgroundColor(Color565 color)
    {
        _backgroundColor = color;
    }

    static void MenuButtonHandler(void* sender)
    {
        MainPage* menu = static_cast<MainPage*>(sender);
        menu->SetBackgroundColor(Color565::Red);
    }

    static void AnalogGainButtonHandler(void* sender)
    {
        MainPage* menu = static_cast<MainPage*>(sender);
        menu->SetBackgroundColor(Color565::Green);
    }

    static void DigitalGainButtonHandler(void* sender)
    {
        MainPage* menu = static_cast<MainPage*>(sender);
        menu->SetBackgroundColor(Color565::MenuBackground);
    }

    void Draw(Painter* painter) override
    {
        painter->DrawImage(apertus_logo.pixel_data, 58, 89, apertus_logo.width, apertus_logo.height);

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

    void Update(Button button, int8_t knob, IMenuSystem* menuSystem, USBCDCDevice* cdcDevice) override
    {
        switch (button)
        {
        case Button::BUTTON_1_UP:
            _fpsButton.SetValue((char*)"1U");
            // _usbDevice->Send((uint8_t*)"Button 1 Up\r\n", 10);
            break;
        case Button::BUTTON_1_DOWN:
            _fpsButton.SetValue((char*)"1D");
            //_usbDevice->Send((uint8_t*)"Button 1 Down\r\n", 10);
            break;
        case Button::BUTTON_2_UP:
            _fpsButton.SetValue((char*)"2");
            // _analogGainButton.Activate(this);
            // _usbDevice->Send((uint8_t*)"Button 2\r\n", 10);
            break;
        case Button::BUTTON_3_UP:
            _fpsButton.SetValue((char*)"3");
            //_digitalGainButton.Activate(this);
            //_usbDevice->Send((uint8_t*)"Button 3\r\n", 10);
            break;
        case Button::BUTTON_4_UP:
            _fpsButton.SetValue((char*)"4");
            //_menuButton.Activate(this);
            //_usbDevice->Send((uint8_t*)"Button 4\r\n", 10);

            menuSystem->SetCurrentScreen(AvailableScreens::SettingsMenu);

            break;
        default:
            break;
        }

        /*if (knob < 0)
        {
            _usbDevice->Send((uint8_t*)"Knob \r\n", 10);
        }*/
    }
};

#endif /* MainPage_H */

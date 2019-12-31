#ifndef MAINMENU_H
#define MAINMENU_H

#include "IMenu.h"
#include "Painter.h"
//#include "IWidget.h"

#include "MainMenuButton.h"

#include "../Media/Images/ApertusLogo.h"

#include "ButtonDefinitions.h"

#include "../../Bootloader/Periphery/USB/IUSBDevice.h"

class MainMenu : public IMenu {
    IUSBDevice* _usbDevice;

    MainMenuButton _fpsButton;
    MainMenuButton _analogGainButton;
    MainMenuButton _digitalGainButton;
    MainMenuButton _menuButton;
    MainMenuButton _shutterButton;
    MainMenuButton _whiteBalanceButton;

    IButton* _widgetArray[6] = {&_fpsButton,  &_analogGainButton, &_digitalGainButton,
                                &_menuButton, &_shutterButton,    &_whiteBalanceButton};

    uint16_t _backgroundColor;

  public:
    explicit MainMenu(IUSBDevice* cdcDevice) :
        _usbDevice(cdcDevice),
        _fpsButton(MainMenuButton(10, 0, 90, "FPS")),
        _analogGainButton(MainMenuButton(115, 0, 90, "A. Gain")),
        _digitalGainButton(MainMenuButton(220, 0, 90, "D. Gain")),
        _menuButton(MainMenuButton(10, 210, 90, "MENU", true)),
        _shutterButton(MainMenuButton(115, 180, 90, "Shutter", true)),
        _whiteBalanceButton(MainMenuButton(220, 180, 90, "WB", true)),
        _backgroundColor(RGB565(180, 180, 180))
    {
        _menuButton.SetCaptionHeight(30);
        _menuButton.HideValue(true);
        _menuButton.SetCaptionFont(Font::FreeSans12pt7b);
        _menuButton.SetHandler(&MenuButtonHandler);

        _analogGainButton.SetHandler(&AnalogGainButtonHandler);
        _digitalGainButton.SetHandler(&DigitalGainButtonHandler);
    }

  protected:
    virtual uint16_t GetBackgroundColor() override
    {
        return _backgroundColor;
    }

    virtual void SetBackgroundColor(uint16_t color)
    {
        _backgroundColor = color;
    }

    static void MenuButtonHandler(void* sender)
    {
        MainMenu* menu = static_cast<MainMenu*>(sender);
        menu->SetBackgroundColor(RGB565(255, 0, 0));
    }

    static void AnalogGainButtonHandler(void* sender)
    {
        MainMenu* menu = static_cast<MainMenu*>(sender);
        menu->SetBackgroundColor(RGB565(0, 255, 64));
    }

    static void DigitalGainButtonHandler(void* sender)
    {
        MainMenu* menu = static_cast<MainMenu*>(sender);
        menu->SetBackgroundColor(RGB565(180, 180, 180));
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

    void Update(Button button) override
    {
        switch (button)
        {
        case Button::BUTTON_1_UP:
            _fpsButton.SetValue((char*)"1U");
            _usbDevice->Send((uint8_t*)"Button 1 Up\r\n", 10);
            break;
        case Button::BUTTON_1_DOWN:
            _fpsButton.SetValue((char*)"1D");
            _usbDevice->Send((uint8_t*)"Button 1 Down\r\n", 10);
            break;
        case Button::BUTTON_2_UP:
            _fpsButton.SetValue((char*)"2");
            _analogGainButton.Activate(this);
            _usbDevice->Send((uint8_t*)"Button 2\r\n", 10);
            break;
        case Button::BUTTON_3_UP:
            _fpsButton.SetValue((char*)"3");
            _digitalGainButton.Activate(this);
            _usbDevice->Send((uint8_t*)"Button 3\r\n", 10);
            break;
        case Button::BUTTON_4_UP:
            _fpsButton.SetValue((char*)"4");
            _menuButton.Activate(this);
            _usbDevice->Send((uint8_t*)"Button 4\r\n", 10);
            break;
        default:
            break;
        }
    }
};

#endif /* MAINMENU_H */

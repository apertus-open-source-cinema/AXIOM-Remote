#ifndef MAINPAGE_H
#define MAINPAGE_H

#include "IMenu.h"
#include "../Painter.h"

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

    void SetupButtons();

  public:
    MainPage(IUSBDevice* cdcDevice);
    
  protected:
    virtual Color565 GetBackgroundColor() override;
    virtual void SetBackgroundColor(Color565 color);

    static void MenuButtonHandler(void* sender);
    static void AnalogGainButtonHandler(void* sender);

    static void DigitalGainButtonHandler(void* sender);

    void Draw(Painter* painter) override;
    void Update(Button button, int8_t knob, IMenuSystem* menuSystem) override;
};

#endif /* MAINPAGE_H */

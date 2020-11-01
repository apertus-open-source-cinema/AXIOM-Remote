#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include "IMenuSystem.h"

#include "MenuDefinitions.h"

#include "Screens/MainPage.h"
#include "Screens/MainMenu.h"
#include "Screens/SettingsSubMenu1.h"
#include "Screens/WhiteBalanceScreen.h"
#include "../../Bootloader/Periphery/ILI9341/ILI9341Device.h"
//#include "../CentralDB.h"

class Painter;
class IScreen;
class IUSBDevice;

class MenuSystem : public IMenuSystem
{
  protected:
    IScreen* _currentScreen;
    AvailableScreens _currentScreenType;

    // TODO: Length has to be adjusted manually, as currently we use static init to save space on the MCU
    IScreen* _availableScreens[4];

    IUSBDevice* _usbDevice;

    // Menus, static init
    MainPage _mainPage;
    MainMenu _MainMenu;
    SettingsSubMenu1 _settingsSubMenu1;
    WhiteBalanceScreen _whiteBalance;

    void InitializeAvailableScreens();

  public:
    explicit MenuSystem(IUSBDevice* usbDevice, CentralDB* centraldb);
    virtual ~MenuSystem();

    void SetCurrentScreen(AvailableScreens menu) override;
    AvailableScreens GetCurrentScreen();

    void Draw(IPainter* painter);

    void Update(Button button, int8_t knob);
};

#endif // MENUSYSTEM_H

#ifndef MENUSYSTEM_H
#define	MENUSYSTEM_H

#include "MenuDefinitions.h"

#include "Screens/MainPage.h"
#include "Screens/SettingsMenu.h"

class Painter;
class IMenu;
class IUSBDevice;

class MenuSystem
{
    IMenu* _currentMenu;

    // TODO: Length has to be adjusted manually, as currently we use static init to save space on the MCU
    IMenu* _availableMenus[3];

    IUSBDevice* _usbDevice;

    // Menus, static init
    MainPage _mainPage;
    SettingsMenu _settingsMenu;

    void InitializeAvailableMenus();

public:
    MenuSystem(IUSBDevice* usbDevice);
    virtual ~MenuSystem();

    void SetCurrentMenu(AvailableMenus menu);

    void Draw(Painter* painter);
};

#endif	//MENUSYSTEM_H


#ifndef SETTINGSSUBMENU1_H
#define SETTINGSSUBMENU1_H

#include <cstring>

#include "Menu.h"
#include "../Painter/Painter.h"
#include "../Widgets/MenuItem.h"
#include "../Widgets/ScreenLinkMenuItem.h"

//#include "../Widgets/MainPageButton.h"

#include "../ButtonDefinitions.h"

#include "../../GlobalSettings.h"

#include "Periphery/USB/IUSBDevice.h"

//#include <Helpers.h>

class SettingsSubMenu1 : public Menu
{
    ScreenLinkMenuItem _menuItemBack;
    MenuItem _menuItemOpt1;
    MenuItem _menuItemOpt2;
    MenuItem _menuItemOpt3;

  public:
    // TODO: Add assignment of menu system to IMenu
    explicit SettingsSubMenu1(IUSBDevice* cdcDevice, CentralDB* centraldb) : Menu(cdcDevice, centraldb)
    {

        _label = "Menu";
        _menuBreadcrumbs = "Menu > SubMenu 1";

        // Added for testing
        _menuItemBack = ScreenLinkMenuItem(_db, "< Up", AvailableScreens::MainMenu, false);
        AddMenuItem(&_menuItemBack);

        _menuItemOpt1 = MenuItem(_db, "Option 1");
        AddMenuItem(&_menuItemOpt1);

        _menuItemOpt2 = MenuItem(_db, "Option 2");
        AddMenuItem(&_menuItemOpt2);

        _menuItemOpt3 = MenuItem(_db, "Option 3");
        AddMenuItem(&_menuItemOpt3);
    }
};

#endif // SETTINGSSUBMENU1_H

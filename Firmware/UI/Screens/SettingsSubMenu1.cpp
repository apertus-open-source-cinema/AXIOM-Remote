#include "SettingsSubMenu1.h"

// TODO: Add assignment of menu system to IMenu
SettingsSubMenu1::SettingsSubMenu1(IUSBDevice* cdcDevice, CentralDB* centraldb) :
    Menu(cdcDevice, centraldb), _menuItemBack(ScreenLinkMenuItem(_db, "< Up", AvailableScreens::MainMenu, false))
{

    _label = "Menu";
    _menuBreadcrumbs = "Menu > SubMenu 1";

    // Added for testing

    AddMenuItem(&_menuItemBack);

    _menuItemOpt1 = MenuItem(_db, "Option 1");
    AddMenuItem(&_menuItemOpt1);

    _menuItemOpt2 = MenuItem(_db, "Option 2");
    AddMenuItem(&_menuItemOpt2);

    _menuItemOpt3 = MenuItem(_db, "Option 3");
    AddMenuItem(&_menuItemOpt3);
}
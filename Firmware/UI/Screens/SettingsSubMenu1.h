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

#include "../../../Bootloader/Periphery/USB/IUSBDevice.h"

//#include <Helpers.h>

class SettingsSubMenu1 : public Menu
{
    ScreenLinkMenuItem _menuItemBack;
    MenuItem _menuItemOpt1;
    MenuItem _menuItemOpt2;
    MenuItem _menuItemOpt3;

  public:
    // TODO: Add assignment of menu system to IMenu
    explicit SettingsSubMenu1(IUSBDevice* cdcDevice, ILI9341Display* display) : Menu(cdcDevice, _display)
    {

        _label = "Menu";
        _menuBreadcrumbs = "Menu > SubMenu 1";

        // Added for testing
        _menuItemBack = ScreenLinkMenuItem("< Up", AvailableScreens::MainMenu, false);
        /*_menuItemBack.SetMenuType(MenuItemType::MENU_ITEM_TYPE_SCREENLINK);
        _menuItemBack.SetTargetScreen(AvailableScreens::MainMenu);*/
        AddMenuItem(&_menuItemBack);

        _menuItemOpt1 = MenuItem("Option 1");
        AddMenuItem(&_menuItemOpt1);

        _menuItemOpt2 = MenuItem("Option 2");
        AddMenuItem(&_menuItemOpt2);

        _menuItemOpt3 = MenuItem("Option 3");
        AddMenuItem(&_menuItemOpt3);
    }
};

#endif // SETTINGSSUBMENU1_H

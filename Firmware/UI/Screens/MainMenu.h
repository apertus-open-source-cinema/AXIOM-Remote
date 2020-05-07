#ifndef MAINMENU_H
#define MAINMENU_H

#include <cstring>

#include "../Painter/Painter.h"
#include "../Widgets/MenuItem.h"

//#include "../Widgets/PopUpParameterMenu.h"
#include "../Widgets/CheckboxMenuItem.h"
#include "../Widgets/PopUpMenuItem.h"
#include "../Widgets/ScreenLinkMenuItem.h"
#include "Menu.h"

#include "../ButtonDefinitions.h"

#include "../../GlobalSettings.h"

#include "../../../Bootloader/Periphery/USB/IUSBDevice.h"

//#include <Helpers.h>

class MainMenu : public Menu
{

    CheckBoxMenuItem _funCheckboxMenuItem;
    PopUpMenuItem _funLevelCheckboxMenuItem;
    ScreenLinkMenuItem _exitLinkMenuItem;
    ScreenLinkMenuItem _subMenu1LinkMenuItem;
    ScreenLinkMenuItem _subMenu2LinkMenuItem;
    ScreenLinkMenuItem _WBLinkMenuItem;

    // just for testing for now
    MenuItem _menuItems[10];

  public:
    // TODO: Add assignment of menu system to IMenu
    explicit MainMenu(IUSBDevice* cdcDevice) : Menu(cdcDevice)
    {
        // Added for testing - demo menu items

        _exitLinkMenuItem = ScreenLinkMenuItem("Exit Menu", AvailableScreens::MainPage, false);
        //_menuItems[0].SetMenuType(MenuItemType::MENU_ITEM_TYPE_SCREENLINK);
        //_menuItems[0].SetTargetScreen(AvailableScreens::MainPage);
        AddMenuItem(&_exitLinkMenuItem);

        _menuItems[1] = MenuItem("Disabled Item");
        _menuItems[1].SetDisabled(true);
        AddMenuItem(&_menuItems[1]);

        _subMenu1LinkMenuItem = ScreenLinkMenuItem("Submenu 1", AvailableScreens::SettingsSubMenu1);
        //_menuItems[2].SetMenuType(MenuItemType::MENU_ITEM_TYPE_SCREENLINK);
        //_menuItems[2].SetTargetScreen(AvailableScreens::SettingsSubMenu1);
        //_menuItems[2].SetValue(">");
        AddMenuItem(&_subMenu1LinkMenuItem);

        _subMenu2LinkMenuItem = ScreenLinkMenuItem("Submenu 2");
        //_menuItems[3].SetMenuType(MenuItemType::MENU_ITEM_TYPE_SCREENLINK);
        //_menuItems[3].SetValue(">");
        AddMenuItem(&_subMenu2LinkMenuItem);

        /*
                _menuItems[4].SetMenuType(MenuItemType::MENU_ITEM_TYPE_CHECKBOX);
                const char* funchoices[2];
                funchoices[0] = "off";
                funchoices[1] = "on";
                _menuItems[4].SetChoices(funchoices, 2);
                _menuItems[4].UpdateChoice(0);
                _menuItems[4].SetLabel("Fun");
                _menuItem[4] = &_menuItems[4];
                */

        _funCheckboxMenuItem = CheckBoxMenuItem("Fun");
        AddMenuItem(&_funCheckboxMenuItem);

        _funLevelCheckboxMenuItem = PopUpMenuItem("Funlevel");
        const char* funlevelchoices[4];
        funlevelchoices[0] = "low";
        funlevelchoices[1] = "medium";
        funlevelchoices[2] = "high";
        funlevelchoices[3] = "crazy";
        _funLevelCheckboxMenuItem.SetChoices(funlevelchoices, 4);
        _funLevelCheckboxMenuItem.UpdateChoice(0);
        AddMenuItem(&_funLevelCheckboxMenuItem);

        _menuItems[6] = MenuItem("Read-only Setting");
        _menuItems[6].SetMenuType(MenuItemType::MENU_ITEM_TYPE_READONLY);
        AddMenuItem(&_menuItems[6]);

        _WBLinkMenuItem = ScreenLinkMenuItem("White Balance", AvailableScreens::WhiteBalance);
        AddMenuItem(&_WBLinkMenuItem);

        _menuItems[8] = MenuItem("Test Entry");
        AddMenuItem(&_menuItems[8]);

        _menuItems[9] = MenuItem("Another Entry");
        AddMenuItem(&_menuItems[9]);

        /*
        _menuButton.SetHandler(&MenuButtonHandler);
        _analogGainButton.SetHandler(&AnalogGainButtonHandler);
        _digitalGainButton.SetHandler(&DigitalGainButtonHandler);
        */
    }
};
#endif // MAINMENU_H
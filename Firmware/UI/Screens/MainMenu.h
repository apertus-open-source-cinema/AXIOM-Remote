#ifndef MAINMENU_H
#define MAINMENU_H

#include <cstring>

#include "../Painter/Painter.h"
#include "../Widgets/MenuItem.h"

#include "../Widgets/PopUpParameterMenu.h"
#include "../Widgets/CheckboxMenuItem.h"
#include "../Widgets/PopUpMenuItem.h"
#include "Menu.h"

#include "../ButtonDefinitions.h"

#include "../../GlobalSettings.h"

#include "../../../Bootloader/Periphery/USB/IUSBDevice.h"

//#include <Helpers.h>

class MainMenu : public Menu
{

    CheckBoxMenuItem FunCheckboxMenuItem = CheckBoxMenuItem();
    PopUpMenuItem FunLevelCheckboxMenuItem = PopUpMenuItem();

    MenuItem* _menuItem[11] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                               nullptr, nullptr, nullptr, nullptr, nullptr};
    MenuItem _menuItems[10] = {MenuItem(),
                               MenuItem(),
                               MenuItem(),
                               MenuItem(),
                               MenuItem(),
                               MenuItem(),
                               MenuItem(),
                               MenuItem("Whitebalance Settings"),
                               MenuItem("Test Item 9"),
                               MenuItem("Test Item 10")};

  public:
    // TODO: Add assignment of menu system to IMenu
    explicit MainMenu(IUSBDevice* cdcDevice) : Menu(cdcDevice)
    {

        // Added for testing

        _menuItems[0].SetMenuType(MenuItemType::MENU_ITEM_TYPE_SCREENLINK);
        _menuItems[0].SetTargetScreen(AvailableScreens::MainPage);
        _menuItems[0].SetLabel("Exit Menu");
        _menuItem[0] = &_menuItems[0];

        _menuItems[1].SetDisabled(true);
        _menuItems[1].SetLabel("Disabled Item");
        _menuItem[1] = &_menuItems[1];

        _menuItems[2].SetLabel("Submenu 1");
        _menuItems[2].SetMenuType(MenuItemType::MENU_ITEM_TYPE_SCREENLINK);
        _menuItems[2].SetTargetScreen(AvailableScreens::SettingsSubMenu1);
        _menuItems[2].SetValue(">");
        _menuItem[2] = &_menuItems[2];

        _menuItems[3].SetLabel("Submenu 2");
        _menuItems[3].SetMenuType(MenuItemType::MENU_ITEM_TYPE_SCREENLINK);
        _menuItems[3].SetValue(">");
        _menuItem[3] = &_menuItems[3];

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

        FunCheckboxMenuItem.SetLabel("Fun");
        _menuItem[4] = &FunCheckboxMenuItem;

        FunLevelCheckboxMenuItem.SetLabel("Funlevel");
        //_menuItems[5].SetMenuType(MenuItemType::MENU_ITEM_TYPE_DROPDOWN);
        //_menuItems[5].SetLabel("Funlevel");
        const char* funlevelchoices[4];
        funlevelchoices[0] = "low";
        funlevelchoices[1] = "medium";
        funlevelchoices[2] = "high";
        funlevelchoices[3] = "crazy";
        FunLevelCheckboxMenuItem.SetChoices(funlevelchoices, 4);
        FunLevelCheckboxMenuItem.UpdateChoice(0);
        _menuItem[5] = &FunLevelCheckboxMenuItem;

        _menuItems[6].SetLabel("Read-only Setting");
        _menuItems[6].SetMenuType(MenuItemType::MENU_ITEM_TYPE_READONLY);
        _menuItem[6] = &_menuItems[6];

        _menuItems[7].SetMenuType(MenuItemType::MENU_ITEM_TYPE_SCREENLINK);
        _menuItems[7].SetTargetScreen(AvailableScreens::WhiteBalance);
        _menuItem[7] = &_menuItems[7];

        _menuItem[8] = &_menuItems[8];
        _menuItem[9] = &_menuItems[9];

        // Color defintions
        _menuBackgroundColor = RGB565(180, 180, 180);
        _menuItemColor = (uint16_t)Color565::White;
        _menuSelectedItemColor = RGB565(255, 128, 0);
        _menuDimmedItemColor = RGB565(247, 251, 247);
        _menuSelectedTextColor = RGB565(255, 255, 255);
        _menuHightlightedItemColor = RGB565(0, 128, 255);
        _menuTextColor = (uint16_t)Color565::Black;
        _menuDisabledTextColor = RGB565(40, 40, 40);
        _menuDisabledItemColor = RGB565(180, 180, 180);

        // init menu selection indexes
        _parameterMenuActive = 0;
        _parameterSelectionIndex = 0;
        _menuOffset = 0;

        // Default selection is first item
        _menuItem[_menuSelectionIndex]->SetHighlighted(true);

        _popUpParameterMenuActive = -1;

        /*
                _menuItems[2] = new MenuItem();
                _menuItems[2].SetLabel("Test Item 3");

                _menuButton.SetHandler(&MenuButtonHandler);

                _analogGainButton.SetHandler(&AnalogGainButtonHandler);
                _digitalGainButton.SetHandler(&DigitalGainButtonHandler);*/
    }

    virtual void Draw(IPainter* painter) override
    {
        Menu::Draw(painter);
    }
};
#endif // MAINMENU_H
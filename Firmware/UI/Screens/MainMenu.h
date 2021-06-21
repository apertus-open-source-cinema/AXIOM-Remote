#ifndef MAINMENU_H
#define MAINMENU_H

#include <cstring>

#include "../Painter/Painter.h"
#include "../Widgets/MenuItem.h"

//#include "../Widgets/PopUpParameterMenu.h"
#include "../Widgets/CheckboxMenuItem.h"
#include "../Widgets/PopUpMenuItem.h"
#include "../Widgets/ScreenLinkMenuItem.h"
#include "../Widgets/ParameterListMenuItem.h"
#include "../Widgets/NumericMenuItem.h"
#include "Menu.h"

#include "../ButtonDefinitions.h"

#include "../../GlobalSettings.h"

class IUSBDevice;

class MainMenu : public Menu
{

    CheckBoxMenuItem _funCheckboxMenuItem;
    PopUpMenuItem _funLevelCheckboxMenuItem;
    ScreenLinkMenuItem _exitLinkMenuItem;
    ScreenLinkMenuItem _subMenu1LinkMenuItem;
    ScreenLinkMenuItem _WBLinkMenuItem;
    ParameterListMenuItem _funactivitiesItem;
    NumericMenuItem _funCount;
    NumericMenuItem _lcdBrightness;

    // CentralDBObserver _lcdBrightnessObserver;

    // just for testing for now
    MenuItem _menuItems[10];

  public:
    // TODO: Add assignment of menu system to IMenu
    explicit MainMenu(IUSBDevice* cdcDevice, CentralDB* centralDB) : Menu(cdcDevice, centralDB)
    {
        // Added for testing - demo menu items

        _exitLinkMenuItem = ScreenLinkMenuItem(_db, "Exit Menu", AvailableScreens::MainPage, false);
        AddMenuItem(&_exitLinkMenuItem);

        _menuItems[1] = MenuItem(_db, "Disabled Item");
        _menuItems[1].SetDisabled(true);
        AddMenuItem(&_menuItems[1]);

        _lcdBrightness = NumericMenuItem(_db, "LCD Brightness", 100, 5, 100, 5, "%");
        _lcdBrightness.SetHandler(&LCDBrightnessMenuItemHandler);
        /*_lcdBrightnessObserver = CentralDBObserver(Attribute::ID::REMOTE_LCD_BRIGHTNESS, [this](const CentralDB& db) {
            _lcdBrightness.SetValue(db.GetUint32(Attribute::ID::REMOTE_LCD_BRIGHTNESS));
        });
        centralDB->Attach(&_lcdBrightnessObserver);*/

        AddMenuItem(&_lcdBrightness);
        //_lcdBrightness.attachObserver(); // TODO: add which value to subscribe to as parameter

        _subMenu1LinkMenuItem = ScreenLinkMenuItem(_db, "Submenu 1", AvailableScreens::SettingsSubMenu1);
        AddMenuItem(&_subMenu1LinkMenuItem);

        _funactivitiesItem = ParameterListMenuItem(_db, "Fun Activity");
        const char* funactivitychoices[5];
        funactivitychoices[0] = "Refill Sandbags";
        funactivitychoices[1] = "Repolish Lenses";
        funactivitychoices[2] = "Demagnetize Tapes";
        funactivitychoices[3] = "Repaint Tallylights";
        funactivitychoices[4] = "Fluidhead Oilchange";
        _funactivitiesItem.SetOptions(funactivitychoices, 5);
        _funactivitiesItem.UpdateChoice(0);
        AddMenuItem(&_funactivitiesItem);

        //_subMenu2LinkMenuItem = ScreenLinkMenuItem("Numeric Menu", AvailableScreens::ParameterListScreen);
        // AddMenuItem(&_subMenu2LinkMenuItem);

        _funCheckboxMenuItem = CheckBoxMenuItem(_db, "Fun");
        AddMenuItem(&_funCheckboxMenuItem);

        _funLevelCheckboxMenuItem = PopUpMenuItem(_db, "Funlevel");
        const char* funlevelchoices[4];
        funlevelchoices[0] = "low";
        funlevelchoices[1] = "medium";
        funlevelchoices[2] = "high";
        funlevelchoices[3] = "crazy";
        _funLevelCheckboxMenuItem.SetChoices(funlevelchoices, 4);
        _funLevelCheckboxMenuItem.UpdateChoice(0);
        AddMenuItem(&_funLevelCheckboxMenuItem);

        _menuItems[6] = MenuItem(_db, "Read-only Setting");
        _menuItems[6].SetMenuType(MenuItemType::MENU_ITEM_TYPE_READONLY);
        AddMenuItem(&_menuItems[6]);

        _funCount = NumericMenuItem(_db, "Fun Count", 100, 0, 100, 1, "%");
        AddMenuItem(&_funCount);

        _WBLinkMenuItem = ScreenLinkMenuItem(_db, "White Balance", AvailableScreens::WhiteBalance);
        AddMenuItem(&_WBLinkMenuItem);

        _menuItems[8] = MenuItem(_db, "Test Entry");
        AddMenuItem(&_menuItems[8]);

        /* _menuItems[9] = MenuItem("Another Entry");
         AddMenuItem(&_menuItems[9]);*/

        /*
        _menuButton.SetHandler(&MenuButtonHandler);
        _analogGainButton.SetHandler(&AnalogGainButtonHandler);
        _digitalGainButton.SetHandler(&DigitalGainButtonHandler);
        */
    }

    static void LCDBrightnessMenuItemHandler(void* sender, CentralDB* db)
    {
        NumericMenuItem* menuitem = static_cast<NumericMenuItem*>(sender);
        db->SetUint32(Attribute::ID::REMOTE_LCD_BRIGHTNESS, menuitem->GetValue());
        // display->SetBacklight((uint8_t)menuitem->GetValue());
    }
};
#endif // MAINMENU_H
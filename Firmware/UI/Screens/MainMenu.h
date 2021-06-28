#ifndef MAINMENU_H
#define MAINMENU_H

// #include <cstring>

// #include "../Painter/Painter.h"
// #include "../Widgets/MenuItem.h"

// //#include "../Widgets/PopUpParameterMenu.h"
#include "../Widgets/CheckboxMenuItem.h"
#include "../Widgets/PopUpMenuItem.h"
#include "../Widgets/ScreenLinkMenuItem.h"
#include "../Widgets/ParameterListMenuItem.h"
#include "../Widgets/NumericMenuItem.h"
#include "Menu.h"

// #include "../ButtonDefinitions.h"

// #include "../../GlobalSettings.h"

class CentralDB;
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
    explicit MainMenu(IUSBDevice* cdcDevice, CentralDB* centralDB);
};
#endif // MAINMENU_H

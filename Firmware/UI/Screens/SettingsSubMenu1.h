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

//#include <Helpers.h>

class IUSBDevice;

class SettingsSubMenu1 : public Menu
{
    ScreenLinkMenuItem _menuItemBack;
    MenuItem _menuItemOpt1;
    MenuItem _menuItemOpt2;
    MenuItem _menuItemOpt3;

  public:
    // TODO: Add assignment of menu system to IMenu
    explicit SettingsSubMenu1(IUSBDevice* cdcDevice, CentralDB* centraldb);
};

#endif // SETTINGSSUBMENU1_H

#include "MenuSystem.h"

#include "Painter.h"

#include "Screens/MainPage.h"
#include "Screens/SettingsMenu.h"

MenuSystem::MenuSystem(IUSBDevice* usbDevice) :
    _currentMenu(nullptr), _usbDevice(usbDevice), _mainPage(usbDevice), _settingsMenu(usbDevice)
{
    InitializeAvailableMenus();

    SetCurrentMenu(AvailableMenus::MainPage);
}

MenuSystem::~MenuSystem()
{
}

void MenuSystem::SetCurrentMenu(AvailableMenus menu)
{
    _currentMenu = _availableMenus[(uint8_t)menu];
}

void MenuSystem::InitializeAvailableMenus()
{
    _availableMenus[0] = &_mainPage;
    _availableMenus[1] = &_settingsMenu;
}

void MenuSystem::Draw(Painter* painter)
{
    if (_currentMenu == nullptr)
    {
        return;
    }

    painter->Fill(_currentMenu->GetBackgroundColor());
    _currentMenu->Draw(painter);
}

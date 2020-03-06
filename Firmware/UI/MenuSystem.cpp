#include "MenuSystem.h"

#include "Painter.h"

#include "Screens/MainPage.h"
#include "Screens/SettingsMenu.h"

MenuSystem::MenuSystem(IUSBDevice* usbDevice) :
    _currentScreen(nullptr), _usbDevice(usbDevice), _mainPage(usbDevice), _settingsMenu(usbDevice)
{
    InitializeAvailableScreens();

    SetCurrentScreen(AvailableScreens::MainPage);
}

IUSBDevice* _usbDevice;

MenuSystem::~MenuSystem()
{
}

void MenuSystem::SetCurrentScreen(AvailableScreens menu)
{
    _currentScreenType = menu;
    _currentScreen = _availableScreens[(uint8_t)menu];
}

AvailableScreens MenuSystem::GetCurrentScreen()
{
    return _currentScreenType;
}

void MenuSystem::InitializeAvailableScreens()
{
    _availableScreens[0] = &_mainPage;
    _availableScreens[1] = &_settingsMenu;
}

void MenuSystem::Draw(Painter* painter)
{
    if (_currentScreen == nullptr)
    {
        return;
    }

    painter->Fill((uint16_t)_currentScreen->GetBackgroundColor());
    _currentScreen->Draw(painter);
}

void MenuSystem::Update(Button button, int8_t knob, IUSBDevice* cdcDevice)
{
    _currentScreen->Update(button, knob, this, cdcDevice);
}

#include "MenuSystem.h"

#include "Painter/IPainter.h"
#include "Screens/IScreen.h"


MenuSystem::MenuSystem(IUSBDevice* usbDevice, CentralDB* centraldb) :
    _currentScreen(nullptr), _usbDevice(usbDevice), _mainPage(usbDevice), _MainMenu(usbDevice, centraldb),
    _settingsSubMenu1(usbDevice, centraldb), _whiteBalance(usbDevice)
{
    InitializeAvailableScreens();

    SetCurrentScreen(AvailableScreens::MainPage);
}

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
    _availableScreens[1] = &_MainMenu;
    _availableScreens[2] = &_settingsSubMenu1;
    _availableScreens[3] = &_whiteBalance;
}

void MenuSystem::Draw(IPainter* painter)
{
    if (_currentScreen == nullptr)
    {
        return;
    }

    painter->Fill((uint16_t)_currentScreen->GetBackgroundColor());
    _currentScreen->Draw(painter);
}

void MenuSystem::Update(Button button, int8_t knob)
{
    _currentScreen->Update(button, knob, this);
}

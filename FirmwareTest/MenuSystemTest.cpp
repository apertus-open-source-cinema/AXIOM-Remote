#include "catch2/catch.hpp"

#include "../Firmware/UI/MenuSystem.h"

class DummyUSBDevice : public IUSBDevice
{
};

TEST_CASE("SetCurrentScreen test")
{
    DummyUSBDevice dummyUSBDevice;
    MenuSystem menuSystem(&dummyUSBDevice);
    menuSystem.SetCurrentScreen(AvailableScreens::SettingsMenu);

    REQUIRE(menuSystem.GetCurrentScreen() == AvailableScreens::SettingsMenu);
}
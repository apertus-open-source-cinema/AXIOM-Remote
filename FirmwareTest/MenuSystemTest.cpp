#include "catch2/catch.hpp"

#include "../Firmware/UI/MenuSystem.h"

#include "PainterMod.h"

class DummyUSBDevice : public IUSBDevice
{
};

TEST_CASE("SetCurrentScreen test")
{
    DummyUSBDevice dummyUSBDevice;
    CentralDB centralDB;
    MenuSystem menuSystem(&dummyUSBDevice, &centralDB);
    menuSystem.SetCurrentScreen(AvailableScreens::MainMenu);

    REQUIRE(menuSystem.GetCurrentScreen() == AvailableScreens::MainMenu);
}

TEST_CASE("Draw test")
{
    DummyUSBDevice dummyUSBDevice;
    CentralDB centralDB;
    MenuSystem menuSystem(&dummyUSBDevice, &centralDB);

    uint16_t* framebuffer = new uint16_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
    PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    menuSystem.Draw(&painter);

    REQUIRE(painter.GetPixel(20, 20) != 0);
}
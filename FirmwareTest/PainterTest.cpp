#include "catch2/catch.hpp"

#include <cstring>

#include "PainterMod.h"

// Test data
#include "Images/LogoIcon.h"
#include "Images/LogoIconOutput.h"

// Setup frame buffer and reset all values to 0
uint16_t* SetupFramebuffer(uint16_t width, uint16_t height)
{
    uint16_t* framebuffer = new uint16_t[width * height];
    memset(framebuffer, 0, width * height * 2); // uint16_t -> 2 bytes -> * 2

    return framebuffer;
}

TEST_CASE("DrawFillRectangle test")
{
    uint16_t* framebuffer = SetupFramebuffer(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    painter.DrawFillRectangle(0, 0, 10, 10, 0xFFFF);

    REQUIRE(painter.wrongDirection == false);
}

TEST_CASE("DrawLine test")
{
    uint16_t* framebuffer = SetupFramebuffer(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    painter.DrawLine(0, 0, 10, 10, 0xFFFF);

    REQUIRE(painter.wrongDirection == false);
}

TEST_CASE("DrawFastVLine test")
{
    uint16_t* framebuffer = SetupFramebuffer(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    painter.DrawFastVLine(0, 0, 10, 0xFFFF);

    REQUIRE(painter.wrongDirection == false);
}

TEST_CASE("DrawFastHLine test")
{
    uint16_t* framebuffer = SetupFramebuffer(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    painter.DrawFastHLine(0, 0, 10, 0xFFFF);

    REQUIRE(painter.wrongDirection == false);
}

// TEST_CASE("DrawFillRoundRectangle test")
// {
//     uint16_t* framebuffer = SetupFramebuffer(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
//     PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

//     painter.DrawFillRoundRectangle(0, 0, 10, 10, 3, 0xFFFF);

//     REQUIRE(painter.wrongDirection == false);
// }

TEST_CASE("DrawRectangle() test")
{
    uint16_t expectedOutput[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //
                                 0x0, 0x7, 0x7, 0x7, 0x7, 0x7, 0x0, //
                                 0x0, 0x7, 0x0, 0x0, 0x0, 0x7, 0x0, //
                                 0x0, 0x7, 0x0, 0x0, 0x0, 0x7, 0x0, //
                                 0x0, 0x7, 0x0, 0x0, 0x0, 0x7, 0x0, //
                                 0x0, 0x7, 0x7, 0x7, 0x7, 0x7, 0x0, //
                                 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    uint8_t width = 7;
    uint8_t height = 7;

    uint16_t* framebuffer = SetupFramebuffer(width, height);
    PainterMod painter(framebuffer, width, height);

    painter.DrawRectangle(1, 1, 5, 5, 0x7);

    bool correctOutput = true;
    uint8_t pixelIndex = 0;
    for (; pixelIndex < width * height; pixelIndex++)
    {
        if (framebuffer[pixelIndex] != expectedOutput[pixelIndex])
        {
            correctOutput = false;
            break;
        }
    }

    REQUIRE(pixelIndex == 49);
    REQUIRE(correctOutput == true);
}

// TEST_CASE("DrawRectangle() drawing direction test")
// {
//     uint16_t* framebuffer = SetupFramebuffer(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
//     PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

//     painter.DrawRectangle(0, 0, 10, 10, 0xFFFF);

//     REQUIRE(painter.wrongDirection == false);
// }

TEST_CASE("DrawStripedRectangle() test")
{
    uint16_t* framebuffer = SetupFramebuffer(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    painter.DrawStripedRectangle(0, 0, 10, 10, 0x0, 0x0, 0, 0);

    REQUIRE(painter.wrongDirection == false);
}

// TEST_CASE("DrawCircle test")
// {
//     uint16_t* framebuffer = new uint16_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
//     PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

//     painter.DrawCircle(0, 0, 10, 0x0);

//     REQUIRE(painter.wrongDirection == false);
// }

// TEST_CASE("DrawFillCircle test")
// {
//     uint16_t* framebuffer = new uint16_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
//     PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

//     painter.DrawFillCircle(20, 20, 10, 0x0);

//     REQUIRE(painter.wrongDirection == false);
// }

// TEST_CASE("DrawCircleQuarter test")
// {
//     uint16_t* framebuffer = new uint16_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
//     PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

//     painter.DrawCircleQuarter(20, 20, 10, 1, 0x0);
//     painter.DrawCircleQuarter(20, 20, 10, 2, 0x0);
//     painter.DrawCircleQuarter(20, 20, 10, 4, 0x0);
//     painter.DrawCircleQuarter(20, 20, 10, 8, 0x0);

//     REQUIRE(painter.wrongDirection == false);
// }

// TEST_CASE("DrawText test")
// {
//     uint16_t* framebuffer = new uint16_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
//     PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

//     painter.DrawText(0, 0, "Test", 0x0, Font::FreeSans9pt7b, TextAlign::TEXT_ALIGN_LEFT, 10);

//     REQUIRE(painter.wrongDirection == false);
// }

TEST_CASE("Fill() test")
{
    uint16_t* framebuffer = SetupFramebuffer(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    painter.Fill(0x20);

    REQUIRE(painter.GetPixel(20, 30) == 0x20);
    REQUIRE(painter.GetPixel(300, 210) == 0x20);
}

TEST_CASE("DrawPixel() test")
{
    uint16_t* framebuffer = SetupFramebuffer(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    painter.DrawPixel(20, 30, 0x53);

    REQUIRE(painter.GetPixel(20, 30) == 0x53);
}

TEST_CASE("DrawIcon test")
{
    uint16_t* framebuffer = new uint16_t[15 * 15];
    PainterMod painter(framebuffer, 15, 15);
    memset(framebuffer, 0, 15 * 15);

    painter.DrawIcon(&LogoIcon, 0, 0, 0x7);

    int index = 0;
    bool check = true;

    for (uint16_t yIndex = 0; yIndex < LogoIcon.Height; yIndex++)
    {
        for (uint16_t xIndex = 0; xIndex < LogoIcon.Width; xIndex++)
        {
            if (framebuffer[yIndex * 15 + xIndex] != LogoIconOutput[index])
            {
                check = false;
                break;
            }

            index++;
        }
    }

    REQUIRE(index == 60);
    REQUIRE(check == true);
}

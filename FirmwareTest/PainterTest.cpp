// 010-TestCase.cpp

// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"

#include "../Firmware/UI/Painter.h"

class PainterMod : public Painter
{
  public:
    uint16_t currentX = 0;
    uint16_t currentY = 0;

    bool wrongDirection = false;

    PainterMod(volatile uint16_t* framebuffer, uint16_t framebufferWidth, uint8_t framebufferHeight) :
        Painter(framebuffer, framebufferWidth, framebufferHeight)
    {
    }

    void DrawPixel(uint16_t x, uint16_t y, uint16_t color) override
    {
        if (x < currentX || y < currentY)
        {
            wrongDirection = true;
            std::cout << "Wrong direction!" << std::endl;
        }

        currentX = x;
        currentY = y;
    }
};

// int Factorial(int number)
// {
//     return number <= 1 ? number : Factorial(number - 1) * number; // fail
//     // return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
// }

// TEST_CASE("Factorial of 0 is 1 (fail)", "[single-file]")
// {
//     REQUIRE(Factorial(0) == 1);
// }

// TEST_CASE("Factorials of 1 and higher are computed (pass)", "[single-file]")
// {
//     REQUIRE(Factorial(1) == 1);
//     REQUIRE(Factorial(2) == 2);
//     REQUIRE(Factorial(3) == 6);
//     REQUIRE(Factorial(10) == 3628800);
// }

// TEST_CASE("Factorials of 1 and higher are computed (pass)", "[single-file]")
// {
//     REQUIRE(Factorial(1) == 1);
//     REQUIRE(Factorial(2) == 2);
//     REQUIRE(Factorial(3) == 6);
//     REQUIRE(Factorial(10) == 3628800);
// }

#define FRAMEBUFFER_WIDTH 320
#define FRAMEBUFFER_HEIGHT 240

TEST_CASE("DrawFillRectangle test")
{
    uint16_t* framebuffer = new uint16_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
    PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    painter.DrawFillRectangle(0, 0, 10, 10, 0xFFFF);

    REQUIRE(painter.wrongDirection == false);
}

TEST_CASE("DrawLine test")
{
    uint16_t* framebuffer = new uint16_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
    PainterMod painter(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    painter.DrawLine(0, 0, 10, 10, 0xFFFF);

    REQUIRE(painter.wrongDirection == false);
}


// Compile & run:
// - g++ -std=c++11 -Wall -I$(CATCH_SINGLE_INCLUDE) -o 010-TestCase 010-TestCase.cpp && 010-TestCase --success
// - cl -EHsc -I%CATCH_SINGLE_INCLUDE% 010-TestCase.cpp && 010-TestCase --success

// Expected compact output (all assertions):
//
// prompt> 010-TestCase --reporter compact --success
// 010-TestCase.cpp:14: failed: Factorial(0) == 1 for: 0 == 1
// 010-TestCase.cpp:18: passed: Factorial(1) == 1 for: 1 == 1
// 010-TestCase.cpp:19: passed: Factorial(2) == 2 for: 2 == 2
// 010-TestCase.cpp:20: passed: Factorial(3) == 6 for: 6 == 6
// 010-TestCase.cpp:21: passed: Factorial(10) == 3628800 for: 3628800 (0x375f00) == 3628800 (0x375f00)
// Failed 1 test case, failed 1 assertion.

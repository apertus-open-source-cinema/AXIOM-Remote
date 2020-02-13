#ifndef PAINTERMOD_H
#define PAINTERMOD_H

#include <iostream>

#include "../Firmware/UI/Painter.h"

// Used for checks of proper draw direction, otherwise performance can degrade massively
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
        }

        currentX = x;
        currentY = y;
    }
};

#endif //PAINTERMOD_H

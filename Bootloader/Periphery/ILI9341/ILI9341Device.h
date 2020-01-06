/* 
 * File:   LCD.h
 * Author: dev
 *
 * Created on 23 July 2019, 21:24
 */

#ifndef LCD_H
#define	LCD_H

#include <stdint.h>

#include "LCDDefinitions.h"
//#include "ILCDDevice.h"

class ILI9341Display// : public ILCDDevice
{
    // Values are defined for landscape mode
    //const uint16_t _width;
    //const uint8_t _height;
    static const uint32_t _framebufferSize = ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT;
    //_framebuffer(new uint16_t(_framebufferSize))

    volatile uint16_t* _framebuffer;//[_framebufferSize];
    //uint16_t _framebuffer[320 * 240];

    unsigned LCDPumpRead(void);
    unsigned LCDPumpReadf(void);
    void LCDPumpWrite(uint16_t data);
    void LCDPumpWrite16(uint16_t data);
    void LCDPumpCommand(uint8_t cmd);

    //    void SetOrientation(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

public:

    explicit ILI9341Display(volatile uint16_t* framebuffer);

    uint16_t GetWidth()
    {
        return ILI9341_TFTHEIGHT;
    }

    uint16_t GetHeight()
    {
        return ILI9341_TFTWIDTH;
    }

    volatile uint16_t* GetFramebuffer();
    void ClearFramebuffer(uint16_t color);
    void UpdateFramebuffer();

    void DisplayFramebuffer();

    void SetupBacklightControl();
    void SetBacklight(uint8_t percentage);

    void Initialize();
};

#endif	/* LCD_H */


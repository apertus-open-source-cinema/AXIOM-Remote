#include "ILI9341Device.h"

#include <xc.h>

//#include "LCDDefinitions.h"
#include "Helpers.h"

ILI9341Display::ILI9341Display(volatile uint16_t* framebuffer)
{
    _framebuffer = framebuffer;
}

void ILI9341Display::SetupBacklightControl()
{
    // Timer 3
    T3CONbits.TCKPS = 0b111;
    T3CONbits.TCS = 0;
    PR3 = 749;
    TMR3 = 0;

    // Output Compare 6 / PWM
    OC6CONbits.OCM = 0b110;
    OC6CONbits.OCTSEL = 1;
//    OC6RS = backlight_linear_percentage[GlobalSettings::brightnessPercentage];

    // Switch OC6 and Timer 3 on
    T3CONbits.ON = 1;
    OC6CONbits.ON = 1;
}

unsigned ILI9341Display::LCDPumpRead(void)
{
    while (PMMODEbits.BUSY);
    return PMDIN;
}

unsigned ILI9341Display::LCDPumpReadf(void)
{
    PMCONbits.PTRDEN = 0;
    unsigned res = LCDPumpRead();
    PMCONbits.PTRDEN = 1;
    return res;
}

void ILI9341Display::LCDPumpWrite(uint16_t data)
{
    while (PMMODEbits.BUSY);
    PMDIN = data;
}

void ILI9341Display::LCDPumpWrite16(uint16_t data)
{
    LCDPumpWrite(data >> 8);
    LCDPumpWrite((uint8_t) data);
}

void ILI9341Display::LCDPumpCommand(uint8_t cmd)
{
    LCD_DCX_O = 0;
    LCDPumpWrite(cmd);
    LCD_DCX_O = 1;
}

volatile uint16_t* ILI9341Display::GetFramebuffer()
{
    return _framebuffer;
}

void ILI9341Display::ClearFramebuffer(uint16_t color)
{
    for (uint32_t index = 0; index < _framebufferSize; index++)
    {
        _framebuffer[index] = color;
    }
}

void ILI9341Display::UpdateFramebuffer()
{

}

void ILI9341Display::Initialize()
{
    CFGEBIAbits.EBIPINEN = 0;

    PMCON = 0;

    RPC1R = 0;
    RPC2R = 0;
    RPC3R = 0;
    RPC4R = 0;
    RPG6R = 0;
    RPG7R = 0;
    RPG8R = 0;

    LCD_TE_A = 0;
    LCD_CSX_A = 0;
    LCD_DCX_A = 0;
    LCD_WRX_A = 0;
    LCD_RDX_A = 0;
    LCD_SDI_A = 0;
    LCD_SDO_A = 0;

    LCD_TE_C = 0;
    LCD_CSX_C = 0;
    LCD_DCX_C = 0;
    LCD_WRX_C = 0;
    LCD_RDX_C = 0;
    LCD_SDI_C = 0;
    LCD_SDO_C = 0;

    LCD_TE_T = 1;
    LCD_CSX_T = 1;
    LCD_DCX_T = 0;
    LCD_WRX_T = 0;
    LCD_RDX_T = 0;
    LCD_SDI_T = 0;
    LCD_SDO_T = 1;

    LCD_TE_U = 0;
    LCD_CSX_U = 1;
    LCD_DCX_U = 1;
    LCD_WRX_U = 1;
    LCD_RDX_U = 1;
    LCD_SDI_U = 1;
    LCD_SDO_U = 1;

    LCD_RSX_T = 0;

    LCD_IM0_T = 1;
    LCD_IM1_T = 1;
    LCD_IM2_T = 1;
    LCD_IM3_T = 1;

    LCD_BLT_T = 0;

    LCD_D0_T = 0;
    LCD_D1_T = 0;
    LCD_D2_T = 0;
    LCD_D3_T = 0;
    LCD_D4_T = 0;
    LCD_D5_T = 0;
    LCD_D6_T = 0;
    LCD_D7_T = 0;
    LCD_D8_T = 0;
    LCD_D9_T = 0;
    LCD_D10_T = 0;
    LCD_D11_T = 0;
    LCD_D12_T = 0;
    LCD_D13_T = 0;
    LCD_D14_T = 0;
    LCD_D15_T = 0;

    LCD_D0_O = 0;
    LCD_D1_O = 0;
    LCD_D2_O = 0;
    LCD_D3_O = 0;
    LCD_D4_O = 0;
    LCD_D5_O = 0;
    LCD_D6_O = 0;
    LCD_D7_O = 0;
    LCD_D8_O = 0;
    LCD_D9_O = 0;
    LCD_D10_O = 0;
    LCD_D11_O = 0;
    LCD_D12_O = 0;
    LCD_D13_O = 0;
    LCD_D14_O = 0;
    LCD_D15_O = 0;

    LCD_RDX_O = 1;

    /* IMx is configured as input and pull up or pull 
       down is used to select the desired output level
       as a work around for Silicon Errata #24 */

    /* 16-bit MCU interface I  D[15:0],CSX,DCX,WRX,RDX */
    LCD_IM0_U = 1;
    LCD_IM1_D = 1;
    LCD_IM2_D = 1;
    LCD_IM3_D = 1;
    LCD_IM0_D = 1;

    /* backlight off */
    //LCD_BLT_O = 0;

    //DelayMs(100);

    /* Reset is not required after changing IMx but it
       doesn't hurt to get everything into a known state */

    // Software Reset
    LCD_RSX_O = 0;
    DelayMs(10);
    LCD_RSX_O = 1;

    DelayMs(100);

    /* configure PMP */

    PMMODEbits.MODE16 = 1;
    PMMODEbits.MODE = 0b10;
    PMMODEbits.INCM = 0b00;

    PMAENbits.PTEN = 0x8000;

    PMCONbits.ADRMUX = 0b00;
    PMCONbits.PTWREN = 1;
    PMCONbits.PTRDEN = 1;
    PMCONbits.CSF = 0b00;
    PMCONbits.ON = 1;

    PMADDR = 0x0000;

    //uart2_str0("\n\rSleep Out ... ");

    LCDPumpCommand(0x11);
    DelayMs(5);

    //uart2_str0("\n\rTE Line ON ... ");

    //LCDPumpCommand(0x34);
    LCDPumpCommand(0x35);
    LCDPumpWrite(0x1);

    //uart2_str0("\n\rRead Display Status ... ");

    //    LCDPumpCommand(ILI9341_RDDST);
    //    LCDPumpRead(); // previous
    //    static uint32_t res = LCDPumpRead(); // 1p dummy
    //    res = (res << 8) | LCDPumpRead(); // 2p [31:24]
    //    res = (res << 8) | LCDPumpRead(); // 3p [23:16]
    //    res = (res << 8) | LCDPumpRead(); // 4p [15:8]
    //    res = (res << 8) | LCDPumpReadf(); // 5p [7:0]

    //uart2_long(res);

    //uart2_str0("\n\rDisplay ON ... ");

    LCDPumpCommand(0x29);

    //uart2_str0("\n\rRead Display Status ... ");

    //    LCDPumpCommand(ILI9341_RDDST);
    //    LCDPumpRead(); // previous
    //    res = LCDPumpRead(); // 1p dummy
    //    res = (res << 8) | LCDPumpRead(); // 2p [31:24]
    //    res = (res << 8) | LCDPumpRead(); // 3p [23:16]
    //    res = (res << 8) | LCDPumpRead(); // 4p [15:8]
    //    res = (res << 8) | LCDPumpReadf(); // 5p [7:0]

    //uart2_long(res);

    //uart2_str0("\n\rNormal Mode ON ... ");

    LCDPumpCommand(ILI9341_NORON);

    //uart2_str0("\n\rMemory Access ... ");

    //LCDPumpCommand(0x36);
    //LCDPumpWrite(0x08);

    //uart2_str0("\n\rPixel Format Set ... ");

    LCDPumpCommand(ILI9341_PIXFMT);
    LCDPumpWrite(0x55);

    //        LCDPumpCommand(ILI9341_CASET);
    //    //    LCDPumpWrite16(0);
    //    //    LCDPumpWrite16(240);
    //    LCDPumpWrite(0x00);
    //    LCDPumpWrite(0x00);
    //    LCDPumpWrite(0x00);
    //    LCDPumpWrite(0xEF);
    //
    //    LCDPumpCommand(ILI9341_PASET);
    //    //LCDPumpWrite16(0);
    //    //LCDPumpWrite16(320);
    //    LCDPumpWrite(0x00);
    //    LCDPumpWrite(0x00);
    //    LCDPumpWrite(0x01);
    //    LCDPumpWrite(0x3F);

    LCDPumpCommand(0xF2); // 3Gamma Function Disable
    LCDPumpWrite(0x00);

    LCDPumpCommand(ILI9341_GAMMASET); //Gamma curve selected
    LCDPumpWrite(0x01);

    LCDPumpWrite(ILI9341_GMCTRP1); //Set Gamma
    LCDPumpWrite(0x0F);
    LCDPumpWrite(0x31);
    LCDPumpWrite(0x2B);
    LCDPumpWrite(0x0C);
    LCDPumpWrite(0x0E);
    LCDPumpWrite(0x08);
    LCDPumpWrite(0x4E);
    LCDPumpWrite(0xF1);
    LCDPumpWrite(0x37);
    LCDPumpWrite(0x07);
    LCDPumpWrite(0x10);
    LCDPumpWrite(0x03);
    LCDPumpWrite(0x0E);
    LCDPumpWrite(0x09);
    LCDPumpWrite(0x00);

    LCDPumpWrite(ILI9341_GMCTRN1); //Set Gamma
    LCDPumpWrite(0x00);
    LCDPumpWrite(0x0E);
    LCDPumpWrite(0x14);
    LCDPumpWrite(0x03);
    LCDPumpWrite(0x11);
    LCDPumpWrite(0x07);
    LCDPumpWrite(0x31);
    LCDPumpWrite(0xC1);
    LCDPumpWrite(0x48);
    LCDPumpWrite(0x08);
    LCDPumpWrite(0x0F);
    LCDPumpWrite(0x0C);
    LCDPumpWrite(0x31);
    LCDPumpWrite(0x36);
    LCDPumpWrite(0x0F);

    // Landscape mode
    LCDPumpCommand(ILI9341_MADCTL);
    LCDPumpWrite(0b00101000);
    LCDPumpCommand(ILI9341_PASET);
    LCDPumpWrite16(0);
    LCDPumpWrite16(ILI9341_TFTWIDTH - 1);
    LCDPumpCommand(ILI9341_CASET);
    LCDPumpWrite16(0);
    LCDPumpWrite16(ILI9341_TFTHEIGHT - 1);

    SetupBacklightControl();

    // Switch backlight on
    //LCD_BLT_O = 1;

    ClearFramebuffer(ILI9341_GREEN);

    // Clear the image
    //clearframebuffer(ILI9341_WHITE);

    // Clear the transition_frame buffer
    //clear_transitionframebuffer(ILI9341_WHITE);

    //    SetOrientation()
}

//void ILI9341Display::SetOrientation(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
//{
//    LCDPumpCommand(ILI9341_CASET);
//    int16_t _x = x;
//    int16_t _y = y;
//
//    uint32_t xa = ((uint32_t) _x << 16) | (_x + h - 1);
//    uint32_t ya = ((uint32_t) _y << 16) | (_y + w - 1);
//
//    LCDPumpCommand(ILI9341_CASET); // Column addr set
//    LCDPumpWrite(xa >> 24);
//    LCDPumpWrite(xa >> 16);
//
//    LCDPumpCommand(ILI9341_PASET); // Row addr set
//    LCDPumpWrite(ya >> 24);
//    LCDPumpWrite(ya >> 16);
//
//    LCDPumpCommand(ILI9341_RAMWR); // write to RAM
//}

void ILI9341Display::DisplayFramebuffer()
{
    while (!LCD_TE_I);
    while (LCD_TE_I);

    LCD_RSX_O = 0;
    LCD_RSX_O = 1;

    LCDPumpCommand(ILI9341_RAMWR);

    for (uint32_t index = 0; index < _framebufferSize; index++)
    {
        LCDPumpWrite(_framebuffer[index]);
    }

    LCD_RSX_O = 0;
    LCD_RSX_O = 1;
}

void ILI9341Display::SetBacklight(uint8_t percentage)
{
    OC6RS = backlight_linear_percentage[percentage];
}

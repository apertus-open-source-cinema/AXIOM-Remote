// TODO: Remove all BL includes when BL and UA are separated,
//       as BL will init all important PIC32 settings and modules

// Configuration
#include "../Bootloader/Configuration/PIC32.h"

// Periphery
#include "../Bootloader/Periphery/ILI9341/ILI9341Device.h"

// Menus/Pages
#include "UI/MenuSystem.h"
#include "UI/MainMenu.h"
#include "UI/AnalogGainMenu.h"

#include <Helpers.h>

#define LCD_BLT_O LATDbits.LATD10

void ConfigGPIO()
{
    // Enable pull-up resistor for E9 to prevent UANT RX interferences
    CNPUEbits.CNPUE9 = 1;
}

void InitPBUS(void)
{
    UnlockPIC32();
    PB2DIVbits.PBDIV = 0b000001; // divide by 2
    PB2DIVbits.ON = 1;

    PB7DIVbits.PBDIV = 0b000000; // divide by 1
    PB7DIVbits.ON = 1;

    CFGCONbits.OCACLK = 1;
    TRISDbits.TRISD10 = 0;
    RPD10R = 0b1100;

    LockPIC32();
}

// Remove when BL is ready
void SetupInterrupts()
{
    INTCONbits.MVEC = 1; // Multi Vector Interrupts
    PRISSbits.SS0 = 0;   // Normal Register Set

    PRISSbits.PRI7SS = 7; // Assign Shadow Register Set

    PRECONbits.PREFEN = 0b10; // instruction and data
    PRECONbits.PFMWS = 0b010; // two wait states

    // cacheable, non-coherent, write-back, alloc
    __builtin_mtc0(_CP0_CONFIG, 0, 0b011);
}

void Setup(ILI9341Display &display, USBCDCDevice& cdcDevice)
{
    DisableIRQ();

    InitPBUS();
    PRECONbits.PREFEN = 0b10; // instruction and data
    PRECONbits.PFMWS = 0b010; // two wait states

    // cacheable, non-coherent, write-back, alloc
    __builtin_mtc0(_CP0_CONFIG, 0, 0b011);

    SetupInterrupts();

    ConfigGPIO();

    cdcDevice.Initialize();
    display.Initialize();

    EnableIRQ();
}

volatile extern uint16_t framebuffer[ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT];

int main()
{
    ILI9341Display display(framebuffer);
    USBCDCDevice cdcDevice;

    Setup(display, cdcDevice);

    //display.SetBacklight(GlobalSettings::brightnessPercentage);

    MenuSystem menuSystem;
    
    Painter painter(display.GetFramebuffer(), display.GetWidth(), display.GetHeight());
    MainMenu mainMenu(&cdcDevice);
    
    // static uint8_t rgb[4];
    // rgb[0] = 0x14;
    // rgb[1] = 0x24;
    // rgb[2] = 0x00;
    // rgb[3] = 0x01;
    // i2c3_setn(0x20, rgb, 4);

    LCD_BLT_O = 1;

    //    display.ClearFramebuffer(RGB565(255,0,0));
    //    display.DisplayFramebuffer();

    while (1)
    {
    // Display Bottom
        DelayMs(2000);
        LCD_BLT_O = !LCD_BLT_O;
    }

    return 0;
}

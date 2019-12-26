
// Periphery
#include "../Bootloader/Periphery/ILI9341/ILI9341Device.h"
#include "../Bootloader/Periphery/I2C/I2C.h"

// Menus/Pages
#include "UI/MenuSystem.h"
#include "UI/MainMenu.h"
#include "UI/AnalogGainMenu.h"

#include "GlobalSettings.h"

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

void init_icsp_w(void)
{
    ICSP_W_MCLR_T = 0; // MCLR out
    ICSP_W_MCLR_O = 0; // MCLR low
}

void init_icsp_e(void)
{
    ICSP_E_MCLR_T = 0; // MCLR out
    ICSP_E_MCLR_O = 0; // MCLR low
}

static inline
void set_mclr_w(unsigned val)
{
    ICSP_W_MCLR_O = (val & 1) ? 1 : 0;
}

static inline
void set_mclr_e(unsigned val)
{
    ICSP_E_MCLR_O = (val & 1) ? 1 : 0;
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

    init_icsp_w();
    init_icsp_e();

    init_i2c_w();
    init_i2c_e();

    set_mclr_w(1);
    set_mclr_e(1);

    cdcDevice.Initialize();
    display.Initialize();

    EnableIRQ();
}

static uint8_t data[16];
uint16_t knob_position[2] = {0, 0};
int8_t diff = 0;

char debugText[32];

void PollKMW(ILI9341Display* display, USBCDCDevice* cdcDevice)
{
    i2c3_getn(0x10, data, 2);

    int8_t brightness = GlobalSettings::brightnessPercentage;

    if (data[1] != knob_position[1])
    {
        diff = data[1] - knob_position[1];
        if (diff < 0)
        {
            brightness -= 10;
            if (brightness < 10)
            {
                brightness = 10;
            }
        }
        else
        {
            brightness += 10;
            if (brightness > 100)
            {
                brightness = 100;
            }
        }

        sprintf(debugText, "nob/diff: %d\r\n", diff);
        cdcDevice->Send((uint8_t*) debugText, 32);

        knob_position[1] = data[1];
        GlobalSettings::brightnessPercentage = brightness;
        display->SetBacklight(GlobalSettings::brightnessPercentage);
    }
}

Button PollKME()
{
    static uint8_t data_status[16];

    i2c2_getn(0x00, data, 3);
    i2c2_getn(0x04, data_status, 3);

    if (data[0] || data[1] || data[2])
    {
        if (data[1] & 0x08)
        {
            if (data_status[1] & 0x08)
            {

            }
            else
            {
                return Button::BUTTON_1;
            }
        }

        if (data[1] & 0x10)
        {
            if (data_status[1] & 0x10)
            {

            }
            else
            {
                return Button::BUTTON_2;
            }
        }

        if (data[1] & 0x20)
        {
            if (data_status[1] & 0x20)
            {

            }
            else
            {
                return Button::BUTTON_3;
            }
        }

        if (data[2] & 0x80)
        {
            if (data_status[2] & 0x80)
            {

            }
            else
            {
                return Button::BUTTON_4;
            }
        }

        if (data[2] & 0x40)
        {
            if (data_status[2] & 0x40)
            {

            }
            else
            {
                return Button::BUTTON_5;
            }
        }

        if (data[2] & 0x20)
        {
            if (data_status[2] & 0x20)
            {

            }
            else
            {
                return Button::BUTTON_6;
            }
        }
    }

    return Button::BUTTON_NONE;
}

int main()
{
    // Defined in procdefs.ld
    volatile extern uint16_t framebuffer[ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT];

    ILI9341Display display(framebuffer);
    USBCDCDevice cdcDevice;

    Setup(display, cdcDevice);

    //display.SetBacklight(GlobalSettings::brightnessPercentage);

    MenuSystem menuSystem;
    
    Painter painter(display.GetFramebuffer(), display.GetWidth(), display.GetHeight());
    MainMenu mainMenu(&cdcDevice);
    
     IMenu* currentMenu = &mainMenu;

    static uint8_t rgb[4];
    rgb[0] = 0x14;
    rgb[1] = 0x24;
    rgb[2] = 0x00;
    rgb[3] = 0x01;
    i2c3_setn(0x20, rgb, 4);

    LCD_BLT_O = 1;

    // //    display.ClearFramebuffer(RGB565(255,0,0));
    // //    display.DisplayFramebuffer();

    // while (1)
    // {
    // // Display Bottom
    //     DelayMs(2000);
    //     LCD_BLT_O = !LCD_BLT_O;
    // }

    char debugText[32];

    uint16_t counter = 0;
    while (1)
    {
        cdcDevice.Process();

        // Buttons and knobs, PIC16 (west)
        PollKMW(&display, &cdcDevice);

        // Buttons, PIC16 (east)
        currentMenu->Update(PollKME());

        display.ClearFramebuffer(currentMenu->GetBackgroundColor());
        currentMenu->Draw(&painter);
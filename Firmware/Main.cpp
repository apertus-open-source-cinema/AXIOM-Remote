// Configuration
#include "../Bootloader/Configuration/PIC32.h"

// Periphery
#include "../Bootloader/Periphery/ILI9341/ILI9341Device.h"
#include "../Bootloader/Periphery/USB/USBCDCDevice.h"
#include "../Bootloader/Periphery/I2C/I2C.h"

// Menus/Pages
#include "UI/MenuSystem.h"
#include "UI/Screens/MainPage.h"
#include "UI/Screens/MainMenu.h"
#include "UI/Screens/AnalogGainMenu.h"

//#define DEBUG_DRAW
#ifdef DEBUG_DRAW
#include "UI/Painter/DebugPainter.h"
#endif

#include "GlobalSettings.h"
//#include "CentralDB.h"

#include <Helpers.h>

#define LCD_BLT_O LATDbits.LATD10

// Defined in procdefs.ld
volatile extern uint16_t framebuffer[ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT];
ILI9341Display display(framebuffer);

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

static inline void set_mclr_w(unsigned val)
{
    ICSP_W_MCLR_O = (val & 1) ? 1 : 0;
}

static inline void set_mclr_e(unsigned val)
{
    ICSP_E_MCLR_O = (val & 1) ? 1 : 0;
}

void Setup(ILI9341Display& display, USBCDCDevice& cdcDevice)
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
static uint8_t data_status[16];
uint16_t knob_position[2] = {0, 0};

char debugText[32];
bool DEBUG = true;

int8_t PollKMW(USBCDCDevice* cdcDevice)
{
    i2c3_getn(0x10, data, 2);

    if (data[0] != knob_position[0])
    {
        int8_t diff = data[0] - knob_position[0];

        knob_position[0] = data[0];

        if (DEBUG)
        {
            sprintf(debugText, "Knob E1: %d \r\n", knob_position[0]);
            cdcDevice->Send((uint8_t*)debugText, 32);

            sprintf(debugText, "data[0]: %d \r\n", data[0]);
            cdcDevice->Send((uint8_t*)debugText, 32);

            sprintf(debugText, "diff E1: %d \r\n", diff);
            cdcDevice->Send((uint8_t*)debugText, 32);
        }

        return diff;
    }

    /* ignore knob 2 for now
    if (data[1] != knob_position[1])
    {
        int8_t diff = data[1] - knob_position[1];

        knob_position[1] = data[1];

        if (DEBUG)
        {
            sprintf(debugText, "Knob E2: %d \r\n", knob_position[1]);
            cdcDevice->Send((uint8_t*)debugText, 32);

            sprintf(debugText, "diff E2: %d \r\n", diff);
            cdcDevice->Send((uint8_t*)debugText, 32);
        }

        return diff;
    }*/
    return 0;
}

Button PollButtons(USBCDCDevice* cdcDevice)
{
    /*
     *  the first three registers (i2c3_getn(0x00, data, 3); data[0], data[1], data[2]) provide change information
     *  i.e. a bit set there shows that something on the respective input (port A,B or C) changed
     *  so, a button pressed or a button released will trigger a bit change there
     *
     *  register 4 to 6 (i2c3_getn(0x04, data_status, 3) data_status[0], data_status[1], data_status[2]) contain the
     *  status registers of each button/knobs current state
     */

    i2c3_getn(0x00, data, 3);
    i2c3_getn(0x04, data_status, 3);

    if ((data[0] & 0x3F) || data[1] || (data[2] & 0x1F))
    {

        if (data[0] & 0x20)
        {
            // Rotary Knob
            if (data_status[0] & 0x20)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"E_1_UP\r\n", 32);
                }
                return Button::E_1_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"E_1_DOWN\r\n", 32);
                }
                return Button::E_1_DOWN;
            }
        }

        if (data[2] & 0x10)
        {
            // Rotary Knob
            if (data_status[2] & 0x10)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"E_2_UP\r\n", 32);
                }
                return Button::E_2_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"E_2_DOWN\r\n", 32);
                }
                return Button::E_2_DOWN;
            }
        }
    }

    i2c2_getn(0x00, data, 3);
    i2c2_getn(0x04, data_status, 3);

    if (data[0] || data[1] || data[2])
    {
        if (DEBUG)
        {
            sprintf(debugText, "data[0]: %02X \r\n", data[0]);
            cdcDevice->Send((uint8_t*)debugText, 32);

            sprintf(debugText, "data[1]: %02X \r\n", data[1]);
            cdcDevice->Send((uint8_t*)debugText, 32);

            sprintf(debugText, "data[2]: %02X \r\n", data[2]);
            cdcDevice->Send((uint8_t*)debugText, 32);

            sprintf(debugText, "data_status[0]: %02X \r\n", data_status[0]);
            cdcDevice->Send((uint8_t*)debugText, 32);

            sprintf(debugText, "data_status[1]: %02X \r\n", data_status[1]);
            cdcDevice->Send((uint8_t*)debugText, 32);

            sprintf(debugText, "data_status[2]: %02X \r\n", data_status[2]);
            cdcDevice->Send((uint8_t*)debugText, 32);
        }

        if (data[1] & 0x08)
        {
            if (data_status[1] & 0x08)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_1_UP\r\n", 32);
                }
                // Button released
                return Button::BUTTON_1_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_1_DOWN\r\n", 32);
                }
                // Button pressed down
                return Button::BUTTON_1_DOWN;
            }
        }

        if (data[1] & 0x10)
        {
            if (data_status[1] & 0x10)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_2_UP\r\n", 32);
                }
                return Button::BUTTON_2_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_2_DOWN\r\n", 32);
                }
                return Button::BUTTON_2_DOWN;
            }
        }

        if (data[1] & 0x20)
        {
            if (data_status[1] & 0x20)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_3_UP\r\n", 32);
                }
                return Button::BUTTON_3_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_3_DOWN\r\n", 32);
                }
                return Button::BUTTON_3_DOWN;
            }
        }

        if (data[2] & 0x80)
        {
            if (data_status[2] & 0x80)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_4_UP\r\n", 32);
                }
                return Button::BUTTON_4_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_4_DOWN\r\n", 32);
                }
                return Button::BUTTON_4_DOWN;
            }
        }

        if (data[2] & 0x40)
        {
            if (data_status[2] & 0x40)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_5_UP\r\n", 32);
                }
                return Button::BUTTON_5_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_5_DOWN\r\n", 32);
                }
                return Button::BUTTON_5_DOWN;
            }
        }

        if (data[2] & 0x20)
        {
            if (data_status[2] & 0x20)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_6_UP\r\n", 32);
                }
                return Button::BUTTON_6_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_6_DOWN\r\n", 32);
                }
                return Button::BUTTON_6_DOWN;
            }
        }
        if (data[2] & 0x10)
        {
            if (data_status[2] & 0x10)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_7_UP\r\n", 32);
                }
                return Button::BUTTON_7_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_7_DOWN\r\n", 32);
                }
                return Button::BUTTON_7_DOWN;
            }
        }
        if (data[2] & 0x08)
        {
            if (data_status[2] & 0x08)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_8_UP\r\n", 32);
                }
                return Button::BUTTON_8_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_8_DOWN\r\n", 32);
                }
                return Button::BUTTON_8_DOWN;
            }
        }
        if (data[2] & 0x04)
        {
            if (data_status[2] & 0x04)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_9_UP\r\n", 32);
                }
                return Button::BUTTON_9_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_9_DOWN\r\n", 32);
                }
                return Button::BUTTON_9_DOWN;
            }
        }
        if (data[1] & 0x04)
        {
            if (data_status[1] & 0x04)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_10_UP\r\n", 32);
                }
                return Button::BUTTON_10_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_10_DOWN\r\n", 32);
                }
                return Button::BUTTON_10_DOWN;
            }
        }
        if (data[1] & 0x02)
        {
            if (data_status[1] & 0x02)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_11_UP\r\n", 32);
                }
                return Button::BUTTON_11_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_11_DOWN\r\n", 32);
                }
                return Button::BUTTON_11_DOWN;
            }
        }
        if (data[1] & 0x01)
        {
            if (data_status[1] & 0x01)
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_12_UP\r\n", 32);
                }
                return Button::BUTTON_12_UP;
            } else
            {
                if (DEBUG)
                {
                    cdcDevice->Send((uint8_t*)"BUTTON_12_DOWN\r\n", 32);
                }
                return Button::BUTTON_12_DOWN;
            }
        }
    }

    return Button::BUTTON_NONE;
}

static inline void irq_disable()
{
    asm volatile("di");
    asm volatile("ehb");
}

static inline void irq_enable()
{
    asm volatile("ei");
}

void init_uart2()
{
    irq_disable();

    U2MODEbits.ON = 0;

    TRISEbits.TRISE8 = 0; // U2TX out
    TRISEbits.TRISE9 = 1; // U2RX in
    ANSELEbits.ANSE8 = 0; // digital
    ANSELEbits.ANSE9 = 0; // digital

    CFGCONbits.IOLOCK = 0;
    RPE8Rbits.RPE8R = 0b0010; // U2TX
    U2RXRbits.U2RXR = 0b1101; // RPE9
    CFGCONbits.IOLOCK = 1;

    IPC36bits.U2TXIP = 0b000; //! Interrupt priority of 7
    IPC36bits.U2TXIS = 0b00;  //! Interrupt sub-priority of 0
    IPC36bits.U2RXIP = 0b111; //! Interrupt priority of 7
    IPC36bits.U2RXIS = 0b00;  //! Interrupt sub-priority of 0

    IEC4SET = _IEC4_U2RXIE_MASK; //! Rx INT Enable
    IFS4bits.U2TXIF = 0;         //! Clear Tx flag
    IFS4bits.U2RXIF = 0;         //! Clear Rx flag

    U2BRG = 24; // 1MBaud @ 50MHz
    U2STA = 0;

    U2MODEbits.BRGH = 1;
    U2MODEbits.PDSEL = 0b00;
    U2MODEbits.STSEL = 0;
    U2MODEbits.UEN = 0b00;
    U2MODEbits.ON = 1;
    U2STASET = 0x9400; // Enable Transmit and Receive

    irq_enable();
}

int main()
{
    USBCDCDevice cdcDevice;

    Setup(display, cdcDevice);

    CentralDB centralDB;
    CentralDBObserver lcdObserver(Attribute::ID::REMOTE_LCD_BRIGHTNESS, [](const CentralDB& db) {
        uint32_t backlightPercentage = db.GetUint32(Attribute::ID::REMOTE_LCD_BRIGHTNESS);
        display.SetBacklight(backlightPercentage);
    });
    centralDB.Attach(&lcdObserver);

    // Set to 50% brightness as default for now
    // centralDB.SetUint32(Attribute::ID::REMOTE_LCD_BRIGHTNESS, 50);

    display.SetBacklight(50);

    MenuSystem menuSystem(&cdcDevice, &centralDB);

    Painter generalPainter(display.GetFramebuffer(), display.GetWidth(), display.GetHeight());
    IPainter* painter = &generalPainter;

#ifdef DEBUG_DRAW
    DebugPainter debugPainter(&generalPainter);
    painter = &debugPainter;
#endif

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

    init_uart2();

    uint16_t counter = 0;
    while (1)
    {
        cdcDevice.Process();

        menuSystem.Update(PollButtons(&cdcDevice), PollKMW(&cdcDevice));

        menuSystem.Draw(painter);

        counter++;
        sprintf(debugText, "%d\r\n", counter);
        painter->DrawText(3, 90, debugText, (uint16_t)Color565::Red, TextAlign::TEXT_ALIGN_LEFT, 0);

        // painter.DrawFillRoundRectangle(50, 120, 100, 40, 5, (uint16_t)Color565::Black);
        // Test
        /*
        painter.DrawCirlce(50, 120, counter % 20, Color565::White);
        painter.DrawFillCirlce(120, 120, counter % 20, Color565::White);
        painter.DrawCircleQuarter(200, 120, counter % 20, 1, Color565::White);
        painter.DrawFillCircleQuarter(280, 120, counter % 20, 1, 0, Color565::White);
        painter.DrawLine(50, 110, 300, 130, Color565::White);
        */
        // painter.DrawCirlce(100, 160, (counter+5)%30, Color565::Red);
        // painter.DrawCirlce(270, 150, (counter+18)%14, Color565::Blue);
        // painter.DrawCirlce(160, 120, (counter+2)%25, Color565::Green);
        // painter.DrawCirlce(200, 90, (counter+12)%17, Color565::White);
        // painter.DrawCircleQuarter(50, 150, counter%15, 1, Color565::White);

        // cdcDevice.Send((uint8_t*)debugText, 10);

        display.DisplayFramebuffer();

        // DelayMs(30);
    }

    return 0;
}

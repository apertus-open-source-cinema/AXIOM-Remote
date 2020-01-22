// MCU header
//#include <xc.h>

// Configuration
#include "Configuration/PIC32.h"

// Firmware headers
#include "Configuration/PIC32.h"
#include "Periphery/USB/USBCDCDevice.h"
#include "Periphery/ICSP/ICSProgrammer.h"

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

USBCDCDevice usbDevice;

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

void Setup()
{
    DisableIRQ();

    InitPBUS();
    PRECONbits.PREFEN = 0b10; // instruction and data
    PRECONbits.PFMWS = 0b010; // two wait states

    // cacheable, non-coherent, write-back, alloc
    __builtin_mtc0(_CP0_CONFIG, 0, 0b011);

    SetupInterrupts();

    ConfigGPIO();

    usbDevice.Initialize();

    EnableIRQ();
}

char outputBuffer[128];

void GetKMConfig(ICSProgrammer& icsp, USBCDCDevice& usbDevice)
{
    // Added 1 to force sending of data, which is disabled for 0,
    // e.g. increment address command
    icsp.SendCommand(ICSPCommand::LoadConfiguration, 1);
    uint16_t address = 0x8000;

    for (int i = 0; i < 10; i++)
    {
        uint16_t value = icsp.Receive();
        sprintf(outputBuffer, "Address: 0x%.8X, value/dec: %u, value/hex: 0x%X\r\n", address + i, value, value);
        usbDevice.Send((uint8_t*)outputBuffer, strlen(outputBuffer));
        icsp.SendCommand(ICSPCommand::IncrementAddress);
    }
}

void GetKMProgramData(ICSProgrammer& icsp, USBCDCDevice& usbDevice)
{
    icsp.SendCommand(ICSPCommand::ResetAddress);
    uint16_t count = 0;
    while (count <= 0x3FFF)
    {
        uint16_t value = icsp.Receive();
        sprintf(outputBuffer, "0x%.8X: %d 0x%X\r\n", count, value, value);
        usbDevice.Send((uint8_t*)outputBuffer, strlen(outputBuffer));
        icsp.SendCommand(ICSPCommand::IncrementAddress);

        count++;
    }
}

void RetrieveKMData(ICSProgrammer& icsp, USBCDCDevice& usbDevice)
{
    // icsp.SelectKM(KeyManager::East);
    icsp.EnterLVP();

    GetKMConfig(icsp, usbDevice);

    //DelayMs(100);

    GetKMProgramData(icsp, usbDevice);

    icsp.ExitLVP();
}

int main()
{
    Setup();

    DelayMs(5000);

    // TODO: Add wait routine till USB is ready
    usbDevice.Send((uint8_t*)"=== AXIOM Remote bootloader ===\r\n", 33);

    ICSProgrammer icsp;
    RetrieveKMData(icsp, usbDevice);

    while (1)
    {
        LCD_BLT_O = !LCD_BLT_O;

        DelayMs(500);
    }

    return 0;
}

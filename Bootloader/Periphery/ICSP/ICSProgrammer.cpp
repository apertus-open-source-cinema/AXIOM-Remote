#include "ICSProgrammer.h"

#include "ICSPDefinitions.h"
#include <Helpers.h>

#define MAGIC_NUMBER 0x4d434850

// Define general names for pins, defaults to west key manager
#define PCLK ICSP_W_PCLK_O
#define PDAT_OUT ICSP_W_PDAT_O
#define PDAT_IN ICSP_W_PDAT_I
#define PDAT_DIR ICSP_W_PDAT_T
#define MCLR ICSP_W_MCLR_O

void ICSProgrammer::TransferBit(uint8_t value)
{
    // Clock edge up
    PCLK = 1;

    PDAT_OUT = value;

    // Clock edge down
    PCLK = 0;
}

bool ICSProgrammer::ReceiveBit()
{
    // Clock edge up
    PCLK = 1;

    bool value = PDAT_IN;

    // Clock edge down
    PCLK = 0;

    return value;
}

void ICSProgrammer::Send(uint32_t data, uint8_t length)
{
    while (length)
    {
        bool bit = data & 1;
        TransferBit(bit);

        data >>= 1;
        length--;
    }
}

uint16_t ICSProgrammer::Read()
{
    int8_t length = 16;
    uint16_t value = 0;

    while (length)
    {
        bool bit = ReceiveBit();
        value |= (bit << (15 - length));
        length--;
    }

    // Compensate the shift for 14 bits
    return value >> 1;
}

ICSProgrammer::ICSProgrammer()
{
    SetupEastKM();
    SetupWestKM();

    SelectKM(KeyManager::West);
}

ICSProgrammer::~ICSProgrammer()
{
}

void ICSProgrammer::SelectKM(KeyManager device)
{
    switch (device)
    {
    case KeyManager::East:
#undef PCLK
#undef PDAT_OUT
#undef PDAT_IN
#undef PDAT_DIR
#undef MCLR

#define PCLK ICSP_E_PCLK_O
#define PDAT_OUT ICSP_E_PDAT_O
#define PDAT_IN ICSP_E_PDAT_I
#define PDAT_DIR ICSP_E_PDAT_T
#define MCLR ICSP_E_MCLR_O
        break;
    case KeyManager::West:
#undef PCLK
#undef PDAT_OUT
#undef PDAT_IN
#undef PDAT_DIR
#undef MCLR

#define PCLK ICSP_W_PCLK_O
#define PDAT_OUT ICSP_W_PDAT_O
#define PDAT_IN ICSP_W_PDAT_I
#define PDAT_DIR ICSP_W_PDAT_T
#define MCLR ICSP_W_MCLR_O
        break;
    }
}

void ICSProgrammer::SetupEastKM()
{
    ICSP_E_MCLR_T = 0; // MCLR out
    ICSP_E_PCLK_T = 0; // PCLK out
    ICSP_E_PDAT_T = 0; // PDAT out

    I2C2CONbits.ON = 0; // disable I2C
}

void ICSProgrammer::SetupWestKM()
{
    ICSP_W_MCLR_T = 0; // MCLR out
    ICSP_W_PCLK_T = 0; // PCLK out
    ICSP_W_PDAT_T = 0; // PDAT out

    I2C3CONbits.ON = 0; // disable I2C
}

void ICSProgrammer::SendCommand(ICSPCommand command, uint16_t value)
{
    // Set PDAT to output
    PDAT_DIR = 0;

    // Send command
    Send((uint8_t)command, 6);

    DelayUs(5);

    if (value != 0)
    {
        // Create 16bit pattern, data is 14bit long -> 0, data, 0
        value = (value << 1) & 0x7FFE;
        Send(value, 16);
    }

    PDAT_DIR = 1;
}

void ICSProgrammer::EnterLVP()
{
    MCLR = 1;
    DelayMs(10);

    PDAT_DIR = 0;
    MCLR = 0;

    Send(MAGIC_NUMBER, 32);

    // Send 33rd pulse
    PCLK = 1;
    PCLK = 0;

    PDAT_DIR = 1;
}

void ICSProgrammer::ExitLVP()
{
    MCLR = 1;
}

uint16_t ICSProgrammer::Receive()
{
    // Set PDAT to output
    PDAT_DIR = 0;

    // Send command
    Send((uint8_t)ICSPCommand::ReadDataFromProgMem, 6);

    DelayUs(5);

    // Set PDAT to input
    PDAT_DIR = 1;

    uint16_t value = Read();

    return value;
}

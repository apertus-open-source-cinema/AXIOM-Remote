#include "UART.h"

#include <xc.h>
#include <sys/attribs.h>

#include "../Common/Definitions.h"

#include "Helpers.h"

static volatile bool dataAvailable = false;

// TODO: remove or modify after initial tests
#define BUFFER_SIZE 128
static unsigned char stringBuffer[BUFFER_SIZE] = {};
uint8_t bufferIndex = 0;

UART::UART()
{
}

void UART::Initialize(uint32_t baudRate)
{
    U2MODEbits.ON = 0;

    CFGCONbits.IOLOCK = 0;
    RPE8Rbits.RPE8R = 0b0010; // U2TX
    U2RXRbits.U2RXR = 0b1101; // RPE9
    CFGCONbits.IOLOCK = 1;

    IPC36bits.U2TXIP = 6; //! Interrupt priority of 7
    IPC36bits.U2TXIS = 0; //! Interrupt sub-priority of 0
    IPC36bits.U2RXIP = 6; //! Interrupt priority of 7
    IPC36bits.U2RXIS = 0; //! Interrupt sub-priority of 0

    IEC4SET = _IEC4_U2RXIE_MASK; //! Rx INT Enable
    IFS4bits.U2TXIF = 0;         //! Clear Tx flag
    IFS4bits.U2RXIF = 0;         //! Clear Rx flag

    //    U2TXREG = 0;
    // IEC4bits.U2TXIE = 1; //! Enable Tx flag
    // IPC36bits.U2TXIP = 1;
    IEC4bits.U2RXIE = 1; //! Enable Rx flag
    IPC36bits.U2RXIP = 1;

    U2BRG = PBCLK_FREQ / (4 * baudRate) - 1;
    U2STA = 0;

    U2MODEbits.BRGH = 1;
    U2MODEbits.PDSEL = 0b00;
    U2MODEbits.STSEL = 0;
    U2MODEbits.UEN = 0b00;

    ANSELEbits.ANSE8 = 0; // digital
    ANSELEbits.ANSE9 = 0; // digital

    TRISEbits.TRISE8 = 0; // U2TX out
    TRISEbits.TRISE9 = 1; // U2RX in

    U2MODEbits.ON = 1;

    U2STASET = 0x9400; // Enable Transmit and Receive
}

inline void SendChar(const unsigned char ch)
{
    while (U2STAbits.UTXBF)
    {
    }

    U2TXREG = ch;
}

void UART::SendText(const char* message) const
{
    while (*message)
    {
        SendChar(*message++);
    }
}

void UART::SendLine(const char* message) const
{
    SendText(message);

    SendChar('\r');
    SendChar('\n');
}

void UART::SendHex(uint8_t value) const
{
    value &= 0xF;
    if (value > 9)
        SendChar(value + 'A' - 10);
    else
        SendChar(value + '0');
}

void UART::SendByte(const uint8_t value) const
{
    SendHex(value >> 4);
    SendHex(value);
}

inline void StoreInFIFO(const char ch)
{
    stringBuffer[bufferIndex] = ch;
    bufferIndex++;
}

extern "C" void __ISR(_UART2_RX_VECTOR, IPL6SRS) UART2_ISR(void)
{
    while (U2STAbits.URXDA)
    {
        char ch = U2RXREG;

        StoreInFIFO(ch);

        if (ch == 0x0)
        {
            dataAvailable = true;
            bufferIndex = 0;
        }
    }

    IFS4bits.U2RXIF = 0; // clear rx interrupt flag
}

bool UART::IsDataAvailable()
{
    return dataAvailable;
}

unsigned char* UART::GetData()
{
    dataAvailable = false;
    return stringBuffer;
}

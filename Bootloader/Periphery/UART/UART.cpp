#include "UART.h"

#include <xc.h>
#include <sys/attribs.h>

#include "../Common/Definitions.h"

volatile bool dataAvailable = false;

UART::UART()
{
}

void UART::Initialize(uint32_t baudRate)
{
    U2MODEbits.ON = 0;

    ANSELEbits.ANSE8 = 0; // digital
    ANSELEbits.ANSE9 = 0; // digital

    TRISEbits.TRISE8 = 0; // U2TX out
    TRISEbits.TRISE9 = 1; // U2RX in

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

    U2TXREG = 0;
    // IEC4bits.U2TXIE = 1; //! Enable Tx flag
    // IPC36bits.U2TXIP = 1;
    IEC4bits.U2RXIE = 1; //! Enable Rx flag
    IPC36bits.U2RXIP = 1;

    // U2BRG = 24; // 1MBaud @ 50MHz
    // U2BRG = 51; // 115200Baud @ 192MHz
    uint32_t peripheralBusClock = (CPU_FREQ / 2);
    U2BRG = peripheralBusClock / (4 * baudRate) - 1;
    // U2STA = 0;

    U2MODEbits.BRGH = 1;
    U2MODEbits.PDSEL = 0b00;
    U2MODEbits.STSEL = 0;
    U2MODEbits.UEN = 0b00;

    U2MODEbits.ON = 1;

    U2STASET = 0x9400; // Enable Transmit and Receive
}

inline void uart2_ch(char ch)
{
    while (U2STAbits.UTXBF)
    {
    }

    U2TXREG = ch;
}

inline void uart2_str0(const char* str)
{
    while (*str)
        uart2_ch(*str++);
}

void UART::SendText(const char* message) const
{
    // uart2_ch('T');
    uart2_str0(message);
    uart2_ch('\r');
    uart2_ch('\n');
}

void UART::SendHex(uint8_t value) const
{
    value &= 0xF;
    if (value > 9)
        uart2_ch(value + 'A' - 10);
    else
        uart2_ch(value + '0');
}

void UART::SendByte(uint8_t value) const
{
    SendHex(value >> 4);
    SendHex(value);
}

uint8_t seqNo = 48;
uint8_t chip;
uartCallbackFunc uartCallback = nullptr;

void UART::SetCallback(uartCallbackFunc callback)
{
    uartCallback = callback;
}

// TODO: remove or modify after initial tests
#define BUFFER_SIZE 128
unsigned char stringBuffer[BUFFER_SIZE];
unsigned char* pbPut = stringBuffer;
uint8_t bufferIndex = 0;

void putString(char ch)
{
    stringBuffer[bufferIndex] = ch;
    bufferIndex++;
    //*pbPut = ch;
    // pbPut++;

    if (pbPut >= (stringBuffer + BUFFER_SIZE))
        pbPut = stringBuffer;
}

bool queueFree = true;

extern "C" void __ISR(_UART2_RX_VECTOR, IPL6SRS) UART2_ISR(void)
{
    char ch;
    while (U2STAbits.URXDA)
    {
        ch = U2RXREG;

        // uart2_ch(ch);
        // only use the data if there was no error
        putString(ch);
        // uart2_str0((const char*)stringBuffer);
    }

    if (ch == 0x0)
    // uartCallback != nullptr && pbPut == stringBuffer + BUFFER_SIZE - 1)
    {
        // uart2_str0((const char*)stringBuffer);
        // uartCallback(stringBuffer, seqNo, chip);
        dataAvailable = true;
        bufferIndex = 0;
    }

    IFS4bits.U2RXIF = 0; // clear rx interrupt flag
}

void UART::Process()
{
    while (U2STAbits.URXDA)
    { // process buffer
        char ch = U2RXREG;
        uart2_ch(ch); // echo back
    }
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

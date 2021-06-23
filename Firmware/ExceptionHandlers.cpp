// void LogException()
// {
//     char debugText2[256];

//     uart2_ch('T');

//     snprintf(debugText2, 100, "PWRCON: %X\n\r", PWRCON);
//     log((const char*)debugText2);
// }

#include <stdint.h>

#include <xc.h>

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

inline void uart2_hex(uint8_t hex)
{
    hex &= 0xF;
    if (hex > 9)
        uart2_ch(hex + 'A' - 10);
    else
        uart2_ch(hex + '0');
}

inline void uart2_byte(uint8_t val)
{
    uart2_hex(val >> 4);
    uart2_hex(val);
}

inline void uart2_word(uint16_t val)
{
    uart2_byte(val >> 8);
    uart2_byte(val);
}

inline void uart2_long(uint32_t val)
{
    uart2_word(val >> 16);
    uart2_word(val);
}

#define STACK_OFFSET_RA ((72 + 24) / 4) // values to be checked before field release
#define STACK_OFFSET_V0 ((8 + 24) / 4)  // values to be checked before field release
#define STACK_OFFSET_V1 ((12 + 24) / 4) // values to be checked before field release

bool exception = false;

extern "C" void __attribute__((nomips16)) _general_exception_handler(void)
{
    register int stackptr asm("sp");

    uint32_t* pStack = (uint32_t*)stackptr;
    uint32_t ra = *(pStack); // + STACK_OFFSET_RA);

    uint32_t address = _CP0_GET_EPC();
    uint32_t vAddress = _CP0_GET_BADVADDR();
    uint32_t cause = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;

    uart2_str0("GE EA:");
    uart2_long(address);
    uart2_str0(" C:");
    uart2_byte(cause);
    uart2_str0("\r\n");

    while (1)
    {
    }
}

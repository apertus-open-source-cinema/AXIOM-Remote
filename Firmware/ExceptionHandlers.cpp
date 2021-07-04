#include <stdint.h>

#include <xc.h>

uint32_t address;
uint8_t cause;

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

void LogException(const char id[4])
{
    register int stackptr asm("sp");
    uint32_t* pStack = (uint32_t*)stackptr;
    uint32_t ra = *(pStack + STACK_OFFSET_RA);
    uint32_t v0 = *(pStack + STACK_OFFSET_V0);
    uint32_t v1 = *(pStack + STACK_OFFSET_V1);

    address = _CP0_GET_EPC();
    cause = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;

    uart2_str0(id);
    uart2_str0(" EA:");
    uart2_long(address);
    uart2_str0(" C:");
    uart2_byte(cause);
    uart2_str0(" RA:");
    uart2_long(ra);
    uart2_str0(" V0:");
    uart2_long(v0);
    uart2_str0(" V1:");
    uart2_long(v1);
    uart2_str0("\r\n");
}

extern "C" void __attribute__((nomips16)) _general_exception_handler(void)
{
    LogException((const char*)"GE");

    while (1)
    {
    }
}

extern "C" void __attribute__((nomips16)) _simple_tlb_refill_exception_handler(void)
{
    LogException((const char*)"TLB");

    while (1)
    {
    }
}

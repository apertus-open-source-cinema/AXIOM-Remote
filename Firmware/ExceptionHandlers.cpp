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

extern "C" void _general_exception_handler(void)
{
    address = _CP0_GET_EPC();
    cause = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;

    uart2_str0("GE EA:");
    uart2_long(address);
    uart2_str0(" C:");
    uart2_byte(cause);
    uart2_str0("\r\n");

    while (1)
    {
    }
}

extern "C" void _simple_tlb_refill_exception_handler(void)
{
    address = _CP0_GET_EPC();
    cause = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;

    uart2_str0("TLB EA:");
    uart2_long(address);
    uart2_str0(" C:");
    uart2_byte(cause);
    uart2_str0("\r\n");

    while (1)
    {
    }
}

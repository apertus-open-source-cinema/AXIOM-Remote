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

    if (exception)
    {
        while (1)
        {
        }
    }

    exception = true;

    uart2_str0("General exception!\r\n");

    uart2_str0("RCON: ");
    uart2_long(RCON);
    uart2_str0("\r\n");

    uart2_str0("Exception address: ");
    uart2_long(address);
    uart2_str0("\r\n");

    uart2_str0("Virtual address: ");
    uart2_long(vAddress);
    uart2_str0("\r\n");

    uart2_str0("Cause: ");
    uart2_long(cause);
    uart2_str0("\r\n");

    uart2_str0("RA: ");
    uart2_long(stackptr);
    uart2_str0("\r\n");
}
//__builtin_disable_interrupts();
// printf("RCON: %X\n\r", RCON);

/*  LogException();
      // log((const char*)debugText2);



      snprintf(debugText, 100, "RNMICON: %X\n\r", RNMICON);
      // log((const char*)debugText);

      // uint64_t address = _CP0_GET_EPC();
      exceptionStatus = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;
      uint8_t rgb[4] = {0x0, 0x12, 0x12, 0x1};
      i2c3_setn(0x20, rgb, 4);
      // int status_mod = (status & _CP0_CAUSE_EXCCODE_MASK) >> _CP0_CAUSE_EXCCODE_POSITION;
      // uint8_t rgb2[4] = {0x12, 0x0, 0x0, 0x1};
      // i2c3_setn(0x20, rgb2, 4);
      // if (exceptionStatus != 0)
      //{
      snprintf(debugText, 100, "Status: %u\n\r", exceptionStatus);
      log((const char*)debugText);
      //}
      uint8_t rgb3[4] = {0x0, 0x0, 0x12, 0x1};
      i2c3_setn(0x20, rgb3, 4);

      sprintf((char*)debugText, "General end\n\r");
      log((const char*)debugText);

      // uint32_t exceptionCode = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;
      /*uint32_t exceptionAddr = _CP0_GET_EPC();

      //sprintf((char*)debugText, "Address: %X, Code: %X\n\r", exceptionAddr, exceptionCode);
      sprintf((char*)debugText, "Address: %X\n\r", exceptionAddr);
      log((const char*)debugText);*//*


    // Enter a loop to stop reset
    while (1)
    {
    }
}

extern "C" void __attribute__((nomips16)) _cache_err_exception_handler(void)
{
    uart2_str0("Cache exception!\n\r");
}

extern "C" void __attribute__((nomips16)) _simple_tlb_refill_exception_handler(void)
{
    if (exception)
    {
        while (1)
        {
        }
    }

    exception = true;

    uart2_str0("TLB exception!\n\r");
    // uint16_t exceptionCode = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;

    _CP0_SET_STATUS(_CP0_GET_STATUS() & 0xFFFFFFE);

    sprintf((char*)debugText, "Test\n\r");
    log((const char*)debugText);
    // unsigned int _epc_code;
    volatile unsigned int _excep_addr;
    // asm volatile("mfc0 %0,$13" : "=r" (_excep_code));
    asm volatile("mfc0 %0,$14" : "=r"(_excep_addr));
    Nop();
    Nop();
    Nop();
    sprintf((char*)debugText, "Address calc\n\r");
    sprintf((char*)debugText, "Address: %X\n\r", _excep_addr);
    log((const char*)debugText);
    sprintf((char*)debugText, "TLB end\n\r");
    // sprintf((char*)debugText, "Address: %X, Code: %X\n\r", exceptionAddr, exceptionCode);
    // log((const char*)debugText);

    // Enter a loop to stop reset
    while (1)
    {
    }
}

*/
/*extern "C" void __attribute__((nomips16)) _on_reset(void)
{
    uart2_str0("Reset happened!\n\r");
    sprintf((char*)debugText, "RCON: %X\n\r", RCON);
    uart2_str0((const char*)debugText);

    RCON = 0;
}*/

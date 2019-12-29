#ifndef HELPERS_H
#define	HELPERS_H

#include "stdint.h"

#include "xc.h"

#include "Definitions.h"

#define DELAY_1MS CPU_FREQ / 10000
#define DELAY_1US CPU_FREQ / 10000000

static inline void DelayMs(uint16_t time)
{
    while(time--) {
        unsigned i;

        for(i = 0; i < DELAY_1MS; i++)
        {
            __asm("nop");
        }
    }
}

static inline void DelayUs(uint16_t time)
{
    while(time--) {
        unsigned i;

        for(i = 0; i < DELAY_1US; i++)
        {
            __asm("nop");
        }
    }
}

static inline void DisableIRQ(void)
{
    __asm volatile("di");
    __asm volatile("ehb");
}

static inline void EnableIRQ(void)
{
    __asm volatile("ei");
}

static inline
void UnlockPIC32(void)
{
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
}

static inline
void LockPIC32(void)
{
    SYSKEY = 0x33333333;
}

#endif	/* HELPERS_H */

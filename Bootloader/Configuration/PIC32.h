#ifndef PIC32CONFIG_H
#define	PIC32CONFIG_H

// DEVCFG0
#pragma config BOOTISA = MIPS32
#pragma config ICESEL = ICS_PGx1
#pragma config FECCCON = OFF_UNLOCKED
#pragma config FSLEEP = 0
#pragma config EJTAGBEN = NORMAL
#pragma config DBGPER = PG_ALL

// DEVCFG1
#pragma	config FDMTEN = OFF
#pragma config DMTCNT = DMT31
#pragma	config FWDTEN = OFF
#pragma config POSCMOD = EC
#pragma config OSCIOFNC = OFF
#pragma config FSOSCEN = OFF
#pragma config FNOSC = SPLL
#pragma config FCKSM = CSECMD

// DEVCFG2
/*	Clock Setup
**	
**	24MHz External Oscillator
**	PLL in = (24MHz/2) = 12MHz	[8-16MHz range]
**	PLL freq = 12MHz*32 = 384MHz	[350MHz - 700MHz]
**	PLL out = 384MHz/2 = 192MHz
*/
#pragma config FPLLICLK = PLL_POSC
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLRNG = RANGE_8_16_MHZ
#pragma config FPLLMULT = MUL_32
#pragma config FPLLODIV = DIV_2
#pragma config UPLLEN = ON
#pragma config UPLLFSEL = FREQ_24MHZ

// DEVCFG3
#pragma config USERID = 0xC0DE
#pragma config FMIIEN = OFF
#pragma config PGL1WAY = OFF
#pragma config PMDL1WAY = OFF
#pragma config IOL1WAY = OFF
#pragma config FUSBIDIO = ON

// DEVCP0
#pragma config CP = OFF

#endif	/* PIC32CONFIG_H */

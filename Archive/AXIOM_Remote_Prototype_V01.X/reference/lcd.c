
/*	PIC32MZ Demo Code
**
**	Copyright (C) 2018 Herbert Poetzl
**
**	This program is free software; you can redistribute it and/or modify
**    	it under the terms of the GNU General Public License 2 as published 
**	by the Free Software Foundation.
**
**	Compile with -O6 for best experience
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

	// DEVCFG0
#pragma config BOOTISA = MIPS32
#pragma config ICESEL = ICS_PGx1
#pragma config FECCCON = OFF_UNLOCKED
#pragma config FSLEEP = 0

	// DEVCFG1
#pragma	config FDMTEN = OFF
#pragma	config FWDTEN = OFF
#pragma config POSCMOD = OFF
#pragma config OSCIOFNC = ON
#pragma config FSOSCEN = OFF
#pragma config FNOSC = SPLL
#pragma config FCKSM = CSECMD

	// DEVCFG2
#pragma config FPLLICLK = PLL_FRC
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLRNG = RANGE_5_10_MHZ
#pragma config FPLLMULT = MUL_100
#pragma config FPLLODIV = DIV_4
#pragma config UPLLEN = OFF
#pragma config UPLLFSEL = FREQ_24MHZ

	// DEVCFG3
#pragma config USERID = 0xC0DE
#pragma config FMIIEN = OFF
#pragma config PGL1WAY = OFF
#pragma config PMDL1WAY = OFF
#pragma config IOL1WAY = OFF
#pragma config FUSBIDIO = OFF

	// DEVCP0
#pragma config CP = OFF


#define	MCU_16BIT


#define ICSP_W_MCLR_O	LATCbits.LATC13
#define ICSP_W_MCLR_T	TRISCbits.TRISC13

#define	ICSP_W_PCLK_O	LATFbits.LATF8
#define	ICSP_W_PCLK_T	TRISFbits.TRISF8
#define ICSP_W_PCLK_U	CNPUFbits.CNPUF8

#define	ICSP_W_PDAT_O	LATFbits.LATF2
#define ICSP_W_PDAT_I	PORTFbits.RF2
#define ICSP_W_PDAT_T	TRISFbits.TRISF2
#define ICSP_W_PDAT_U	CNPUFbits.CNPUF2


#define ICSP_E_MCLR_O	LATCbits.LATC14
#define ICSP_E_MCLR_T	TRISCbits.TRISC14

#define	ICSP_E_PCLK_O	LATAbits.LATA2
#define	ICSP_E_PCLK_T	TRISAbits.TRISA2
#define	ICSP_E_PCLK_U	CNPUAbits.CNPUA2

#define	ICSP_E_PDAT_O	LATAbits.LATA3
#define ICSP_E_PDAT_I	PORTAbits.RA3
#define ICSP_E_PDAT_T	TRISAbits.TRISA3
#define ICSP_E_PDAT_U	CNPUAbits.CNPUA3


#define LCD_BLT_O	LATDbits.LATD10
#define	LCD_BLT_T	TRISDbits.TRISD10

#define	LCD_IM0_O       LATBbits.LATB15
#define LCD_IM0_T       TRISBbits.TRISB15
#define LCD_IM0_D       CNPDBbits.CNPDB15
#define LCD_IM0_U       CNPUBbits.CNPUB15

#define	LCD_IM1_O       LATBbits.LATB14
#define LCD_IM1_T       TRISBbits.TRISB14
#define LCD_IM1_D       CNPDBbits.CNPDB14
#define LCD_IM1_U       CNPUBbits.CNPUB14

#define	LCD_IM2_O       LATBbits.LATB13
#define LCD_IM2_T       TRISBbits.TRISB13
#define LCD_IM2_D       CNPDBbits.CNPDB13
#define LCD_IM2_U       CNPUBbits.CNPUB13

#define	LCD_IM3_O       LATBbits.LATB12
#define LCD_IM3_T       TRISBbits.TRISB12
#define LCD_IM3_D       CNPDBbits.CNPDB12
#define LCD_IM3_U       CNPUBbits.CNPUB12


#define	LCD_RSX_O       LATDbits.LATD11
#define LCD_RSX_T       TRISDbits.TRISD11


#define	LCD_TE_I	PORTGbits.RG15
#define	LCD_TE_O	LATGbits.LATG15
#define	LCD_TE_C	ODCGbits.ODCG15
#define	LCD_TE_A	ANSELGbits.ANSG15
#define	LCD_TE_T	TRISGbits.TRISG15
#define	LCD_TE_U	CNPUGbits.CNPUG15

#define	LCD_CSX_O	LATCbits.LATC1
#define	LCD_CSX_C	ODCCbits.ODCC1
#define	LCD_CSX_A	ANSELCbits.ANSC1
#define	LCD_CSX_T	TRISCbits.TRISC1
#define	LCD_CSX_U	CNPUCbits.CNPUC1

#define	LCD_DCX_O	LATDbits.LATD15
#define	LCD_DCX_C	ODCDbits.ODCD15
#define	LCD_DCX_A	ANSELDbits.ANSD15
#define	LCD_DCX_T	TRISDbits.TRISD15
#define	LCD_DCX_U	CNPUDbits.CNPUD15

#define	LCD_WRX_O	LATCbits.LATC3
#define	LCD_WRX_C	ODCCbits.ODCC3
#define	LCD_WRX_A	ANSELCbits.ANSC3
#define	LCD_WRX_T	TRISCbits.TRISC3
#define	LCD_WRX_U	CNPUCbits.CNPUC3

#define	LCD_RDX_O	LATCbits.LATC4
#define	LCD_RDX_C	ODCCbits.ODCC4
#define	LCD_RDX_A	ANSELCbits.ANSC4
#define	LCD_RDX_T	TRISCbits.TRISC4
#define	LCD_RDX_U	CNPUCbits.CNPUC4

#define	LCD_SDI_I	PORTGbits.RG6
#define	LCD_SDI_O	LATGbits.LATG6
#define	LCD_SDI_C	ODCGbits.ODCG6
#define	LCD_SDI_A	ANSELGbits.ANSG6
#define	LCD_SDI_T	TRISGbits.TRISG6
#define	LCD_SDI_U	CNPUGbits.CNPUG6

#define	LCD_SDO_I	PORTCbits.RC2
#define	LCD_SDO_O	LATCbits.LATC2
#define	LCD_SDO_C	ODCCbits.ODCC2
#define	LCD_SDO_A	ANSELCbits.ANSC2
#define	LCD_SDO_T	TRISCbits.TRISC2
#define	LCD_SDO_U	CNPUCbits.CNPUC2


#define	LCD_D0_O	LATEbits.LATE0
#define	LCD_D0_T	TRISEbits.TRISE0

#define	LCD_D1_O	LATEbits.LATE1
#define	LCD_D1_T	TRISEbits.TRISE1

#define	LCD_D2_O	LATEbits.LATE2
#define	LCD_D2_T	TRISEbits.TRISE2

#define	LCD_D3_O	LATEbits.LATE3
#define	LCD_D3_T	TRISEbits.TRISE3

#define	LCD_D4_O	LATEbits.LATE4
#define	LCD_D4_T	TRISEbits.TRISE4

#define	LCD_D5_O	LATEbits.LATE5
#define	LCD_D5_T	TRISEbits.TRISE5

#define	LCD_D6_O	LATEbits.LATE6
#define	LCD_D6_T	TRISEbits.TRISE6

#define	LCD_D7_O	LATEbits.LATE7
#define	LCD_D7_T	TRISEbits.TRISE7

#define	LCD_D8_O	LATGbits.LATG0
#define	LCD_D8_T	TRISGbits.TRISG0

#define	LCD_D9_O	LATGbits.LATG1
#define	LCD_D9_T	TRISGbits.TRISG1

#define	LCD_D10_O	LATFbits.LATF1
#define	LCD_D10_T	TRISFbits.TRISF1

#define	LCD_D11_O	LATFbits.LATF0
#define	LCD_D11_T	TRISFbits.TRISF0

#define	LCD_D12_O	LATDbits.LATD12
#define	LCD_D12_T	TRISDbits.TRISD12

#define	LCD_D13_O	LATDbits.LATD13
#define	LCD_D13_T	TRISDbits.TRISD13

#define	LCD_D14_O	LATDbits.LATD2
#define	LCD_D14_T	TRISDbits.TRISD2

#define	LCD_D15_O	LATDbits.LATD3
#define	LCD_D15_T	TRISDbits.TRISD3



static inline
void	unlock(void)
{
	SYSKEY = 0xAA996655;
	SYSKEY = 0x556699AA;
}

static inline
void	lock(void)
{
	SYSKEY = 0x33333333;
}

static inline
void	irq_disable(void)
{
	asm volatile("di");
	asm volatile("ehb");
}

static inline
void	irq_enable(void)
{
	asm volatile("ei");
}


void	init_pbus(void)
{
	unlock();
	PB2DIVbits.PBDIV = 0b000001;	// divide by 2
	PB2DIVbits.ON = 1;

	PB7DIVbits.PBDIV = 0b000000;	// divide by 1
	PB7DIVbits.ON = 1;
	lock();
}


void	init_icsp_w(void)
{
	ICSP_W_MCLR_T = 0;		// MCLR out
	ICSP_W_MCLR_O = 0;		// MCLR low
}


void	init_icsp_e(void)
{
	ICSP_E_MCLR_T = 0;		// MCLR out
	ICSP_E_MCLR_O = 0;		// MCLR low
}


void	init_i2c_e(void)
{
	ICSP_E_PCLK_U = 1;
	ICSP_E_PDAT_U = 1;

	I2C2BRG = 128;

	I2C2ADD = 0xFF;
	I2C2MSK = 0xFF;
	
	ICSP_E_PDAT_O = 0;		// clear SDA
	ICSP_E_PDAT_T = 0;		// SDA to out
	ICSP_E_PCLK_O = 1;		// set SCL
	ICSP_E_PCLK_T = 1;		// SCL to in

	I2C2CONbits.ON = 1;
}

void	init_i2c_w(void)
{
	ICSP_W_PCLK_U = 1;
	ICSP_W_PDAT_U = 1;

	I2C3ADD = 0xFF;
	I2C3MSK = 0xFF;

	I2C3BRG = 128;

	ICSP_W_PDAT_O = 0;		// clear SDA
	ICSP_W_PDAT_T = 0;		// SDA to out
	ICSP_W_PCLK_O = 1;		// set SCL
	ICSP_W_PCLK_T = 1;		// SCL to in

	I2C3CONbits.ON = 1;
}



void	init_uart2(void)
{
	irq_disable();

	U2MODEbits.ON = 0;

	TRISEbits.TRISE8 = 0;		// U2TX out
	TRISEbits.TRISE9 = 1;		// U2RX in
	ANSELEbits.ANSE8 = 0;		// digital
	ANSELEbits.ANSE9 = 0;		// digital

	CFGCONbits.IOLOCK = 0;
	RPE8Rbits.RPE8R = 0b0010;	// U2TX
	U2RXRbits.U2RXR = 0b1101;	// RPE9
	CFGCONbits.IOLOCK = 1;

	IPC36bits.U2TXIP = 0b000;	//! Interrupt priority of 7
	IPC36bits.U2TXIS = 0b00;	//! Interrupt sub-priority of 0
	IPC36bits.U2RXIP = 0b111;	//! Interrupt priority of 7
	IPC36bits.U2RXIS = 0b00;	//! Interrupt sub-priority of 0

	IEC4SET = _IEC4_U2RXIE_MASK;	//! Rx INT Enable
	IFS4bits.U2TXIF = 0;		//! Clear Tx flag
	IFS4bits.U2RXIF = 0;		//! Clear Rx flag

	U2BRG = 24;			// 1MBaud @ 50MHz
	U2STA = 0;
	
	U2MODEbits.BRGH = 1;
	U2MODEbits.PDSEL = 0b00;
	U2MODEbits.STSEL = 0;
	U2MODEbits.UEN = 0b00;
	U2MODEbits.ON = 1;
	U2STASET = 0x9400;		// Enable Transmit and Receive

	irq_enable();
}


static inline
uint8_t	i2c2_start(void)
{
	I2C2CONbits.SEN = 1;		// Send Start
	while (I2C2CONbits.SEN);
	return I2C2STAT & 0xFF;
}

static inline
uint8_t	i2c3_start(void)
{
	I2C3CONbits.SEN = 1;		// Send Start
	while (I2C3CONbits.SEN);
	return I2C3STAT & 0xFF;
}

static inline
uint8_t	i2c2_restart(void)
{
	I2C2CONbits.RSEN = 1;		// Send Restart
	while (I2C2CONbits.RSEN);
	return I2C2STAT & 0xFF;
}

static inline
uint8_t	i2c3_restart(void)
{
	I2C3CONbits.RSEN = 1;		// Send Restart
	while (I2C3CONbits.RSEN);
	return I2C3STAT & 0xFF;
}


#if	0
static inline
uint8_t	i2c2_stop(void)
{
	if ((I2C2CON & 0x1F) == 0)
	    I2C2CONbits.PEN = 1;	// Send Stop
	return I2C2CON & 0x1F;
}
#else
#define i2c2_stop i2c2_bb_stop
#endif


#if	0
static inline
uint8_t	i2c3_stop(void)
{
	if ((I2C3CON & 0x1F) == 0)
	    I2C3CONbits.PEN = 1;	// Send Stop
	return I2C3CON & 0x1F;
}
#else
#define i2c3_stop i2c3_bb_stop
#endif

static inline
void	i2c_bb_delay(unsigned cnt)
{
	unsigned i;
	while (cnt--)
	    for (i=0; i<200; i++);
}

static inline
void	i2c2_bb_stop(void)
{
	i2c_bb_delay(1);
	I2C2CONbits.ON = 0;		// disable I2C
	i2c_bb_delay(1);
	ICSP_E_PDAT_T = 1;		// SDA to input
	i2c_bb_delay(2);
	I2C2CONbits.ON = 1;		// enable I2C
	ICSP_E_PDAT_O = 0;		// clear SDA
	ICSP_E_PDAT_T = 0;		// SDA to out
	ICSP_E_PCLK_O = 1;		// set SCL
	ICSP_E_PCLK_T = 1;		// SCL to in
}

static inline
void	i2c3_bb_stop(void)
{
	i2c_bb_delay(1);
	I2C3CONbits.ON = 0;		// disable I2C
	i2c_bb_delay(1);
	ICSP_W_PDAT_T = 1;		// SDA to input
	i2c_bb_delay(2);
	I2C3CONbits.ON = 1;		// enable I2C
	ICSP_W_PDAT_O = 0;		// clear SDA
	ICSP_W_PDAT_T = 0;		// SDA to out
	ICSP_W_PCLK_O = 1;		// set SCL
	ICSP_W_PCLK_T = 1;		// SCL to in
}


static inline
bool	i2c2_write(uint8_t byte)
{
	I2C2TRN = byte;
	while (I2C2STATbits.TRSTAT);
	return I2C2STATbits.ACKSTAT;
}

static inline
bool	i2c3_write(uint8_t byte)
{
	I2C3TRN = byte;
	while (I2C3STATbits.TRSTAT);
	return I2C3STATbits.ACKSTAT;
}

static inline
uint8_t	i2c2_read(bool ackdt)
{
	while (I2C2STATbits.RBF)
	    (void)I2C2RCV;
	if (I2C2CON & 0x1F)
	    return 0xFF;

	I2C2CONbits.RCEN = 1;
	while (!I2C2STATbits.RBF);

	I2C2CONbits.ACKDT = ackdt;
	I2C2CONbits.ACKEN = 1;
	while (I2C2CONbits.ACKEN);

	return I2C2RCV;
}

static inline
uint8_t	i2c3_read(bool ackdt)
{
	while (I2C3STATbits.RBF)
	    (void)I2C3RCV;
	while (I2C3CON & 0x1F);

	I2C3CONbits.RCEN = 1;
	while (!I2C3STATbits.RBF);

	I2C3CONbits.ACKDT = ackdt;
	I2C3CONbits.ACKEN = 1;
	while (I2C3CONbits.ACKEN);

	return I2C3RCV;
}



static inline
void	set_mclr_w(unsigned val)
{
	ICSP_W_MCLR_O = (val & 1) ? 1 : 0;
}


static inline
void	set_mclr_e(unsigned val)
{
	ICSP_E_MCLR_O = (val & 1) ? 1 : 0;
}



static inline
void	uart2_ch(char ch)
{
	while (U2STAbits.UTXBF);
	U2TXREG = ch;
}

static inline
void	uart2_hex(uint8_t hex)
{
	hex &= 0xF;
	if (hex > 9)
	    uart2_ch(hex + 'A' - 10);
	else
	    uart2_ch(hex + '0');
}

static inline
void	uart2_byte(uint8_t val)
{
	uart2_hex(val >> 4);
	uart2_hex(val);
}

static inline
void	uart2_word(uint16_t val)
{
	uart2_byte(val >> 8);
	uart2_byte(val);
}

static inline
void	uart2_long(uint32_t val)
{
	uart2_word(val >> 16);
	uart2_word(val);
}

static inline
void	uart2_str0(const char *str)
{
	while (*str)
	    uart2_ch(*str++);
}


static inline
void	delay_us(unsigned cnt)
{
	while (cnt--) {
	    for (unsigned i=0; i<49; i++)
		asm("nop");
	}
}

#define	delay_ms(t) delay_us(1000*(t))


void __attribute__((vector(_UART2_RX_VECTOR), interrupt(IPL7SRS), nomips16)) uart2_isr(void)
{
	while (U2STAbits.URXDA) {	// process buffer 
	    char ch = U2RXREG;

	    uart2_ch(ch);	// echo back
	}
       
	IFS4CLR = _IFS4_U2RXIF_MASK;	// clear UART2 Rx IRQ
}


uint8_t	i2c2_get(uint8_t idx)
{
	uint8_t val;

	i2c2_start();
	i2c2_write(0x20);
	i2c2_write(idx);
	i2c2_restart();
	i2c2_write(0x21);
	val = i2c2_read(1);
	i2c2_stop();	

	return val;
}

void	i2c2_set(uint8_t idx, uint8_t val)
{
	i2c2_start();
	i2c2_write(0x20);
	i2c2_write(idx);
	i2c2_write(val);
	i2c2_stop();	
}

void	i2c2_getn(uint8_t idx, uint8_t *data, uint8_t n)
{
	i2c2_start();
	i2c2_write(0x20);
	i2c2_write(idx);
	i2c2_restart();
	i2c2_write(0x21);
	while (n--)
	    *data++ = i2c2_read(n == 0);
	i2c2_stop();	
}

void	i2c2_setn(uint8_t idx, uint8_t *data, uint8_t n)
{
	i2c2_start();
	i2c2_write(0x20);
	i2c2_write(idx);
	while (n--)
	    i2c2_write(*data++);
	i2c2_stop();	
}


uint8_t	i2c3_get(uint8_t idx)
{
	uint8_t val;

	i2c3_start();
	i2c3_write(0x20);
	i2c3_write(idx);
	i2c3_restart();
	i2c3_write(0x21);
	val = i2c3_read(1);
	i2c3_stop();	

	return val;
}

void	i2c3_set(uint8_t idx, uint8_t val)
{
	i2c3_start();
	i2c3_write(0x20);
	i2c3_write(idx);
	i2c3_write(val);
	i2c3_stop();	
}

void	i2c3_getn(uint8_t idx, uint8_t *data, uint8_t n)
{
	i2c3_start();
	i2c3_write(0x20);
	i2c3_write(idx);
	i2c3_restart();
	i2c3_write(0x21);
	while (n--)
	    *data++ = i2c3_read(n == 0);
	i2c3_stop();	
}

void	i2c3_setn(uint8_t idx, uint8_t *data, uint8_t n)
{
	i2c3_start();
	i2c3_write(0x20);
	i2c3_write(idx);
	while (n--)
	    i2c3_write(*data++);
	i2c3_stop();	
}



uint8_t bitidx(uint8_t val)
{
	uint8_t i;

	for (i=0; i<8; i++)
	    if (val & 1)
		return i;
	    else
		val >>= 1;
	return 0xF;
}


static inline
void	lcd_pmp_wr(unsigned data)
{
	while (PMMODEbits.BUSY);
	PMDIN = data;
}

static inline
unsigned lcd_pmp_rd(void)
{
	while (PMMODEbits.BUSY);
	return PMDIN;
}

static inline
unsigned lcd_pmp_rdf(void)
{
	PMCONbits.PTRDEN = 0;
	unsigned res = lcd_pmp_rd();
	PMCONbits.PTRDEN = 1;
	return res;
}

static inline
void	lcd_pmp_cmd(uint8_t cmd)
{
	LCD_DCX_O = 0;
	lcd_pmp_wr(cmd);
	LCD_DCX_O = 1;
}



int	main(void)
{
	static uint8_t rgb[4];


	init_pbus();
	init_icsp_w();
	init_icsp_e();
	init_uart2();

	init_i2c_w();
	init_i2c_e();

	delay_ms(1);
#ifdef	MCU_16BIT
	uart2_str0("\f\n\rAXIOM Remote Demo v0.1 [MCU-16bit]\n\r");
#else
	uart2_str0("\f\n\rAXIOM Remote Demo v0.1 [MCU-8bit]\n\r");
#endif

	delay_ms(1);
	set_mclr_w(1);
	set_mclr_e(1);

	delay_ms(1);

	rgb[0] = 0x18;
	rgb[1] = 0x28;
	rgb[2] = 0x38;
	rgb[3] = 0x01;
	i2c3_setn(0x20, rgb, 4);


	CFGEBIAbits.EBIPINEN = 0;

	PMCON = 0;

	RPC1R = 0;
	RPC2R = 0;
	RPC3R = 0;
	RPC4R = 0;
	RPG6R = 0;
	RPG7R = 0;
	RPG8R = 0;

	LCD_TE_A = 0;
	LCD_CSX_A = 0;
	LCD_DCX_A = 0;
	LCD_WRX_A = 0;
	LCD_RDX_A = 0;
	LCD_SDI_A = 0;
	LCD_SDO_A = 0;

	LCD_TE_C = 0;
	LCD_CSX_C = 0;
	LCD_DCX_C = 0;
	LCD_WRX_C = 0;
	LCD_RDX_C = 0;
	LCD_SDI_C = 0;
	LCD_SDO_C = 0;

	LCD_TE_T = 1;
	LCD_CSX_T = 1;
	LCD_DCX_T = 0;
	LCD_WRX_T = 0;
	LCD_RDX_T = 0;
	LCD_SDI_T = 0;
	LCD_SDO_T = 1;

	LCD_TE_U = 0;
	LCD_CSX_U = 1;
	LCD_DCX_U = 1;
	LCD_WRX_U = 1;
	LCD_RDX_U = 1;
	LCD_SDI_U = 1;
	LCD_SDO_U = 1;

	LCD_RSX_T = 0;

	LCD_IM0_T = 1;
	LCD_IM1_T = 1;
	LCD_IM2_T = 1;
	LCD_IM3_T = 1;

	LCD_BLT_T = 0;

	LCD_D0_T = 0;
	LCD_D1_T = 0;
	LCD_D2_T = 0;
	LCD_D3_T = 0;
	LCD_D4_T = 0;
	LCD_D5_T = 0;
	LCD_D6_T = 0;
	LCD_D7_T = 0;
	LCD_D8_T = 0;
	LCD_D9_T = 0;
	LCD_D10_T = 0;
	LCD_D11_T = 0;
	LCD_D12_T = 0;
	LCD_D13_T = 0;
	LCD_D14_T = 0;
	LCD_D15_T = 0;

	LCD_D0_O = 0;
	LCD_D1_O = 0;
	LCD_D2_O = 0;
	LCD_D3_O = 0;
	LCD_D4_O = 0;
	LCD_D5_O = 0;
	LCD_D6_O = 0;
	LCD_D7_O = 0;
	LCD_D8_O = 0;
	LCD_D9_O = 0;
	LCD_D10_O = 0;
	LCD_D11_O = 0;
	LCD_D12_O = 0;
	LCD_D13_O = 0;
	LCD_D14_O = 0;
	LCD_D15_O = 0;

	LCD_RDX_O = 1;

	/* IMx is configured as input and pull up or pull 
	   down is used to select the desired output level
	   as a work around for Silicon Errata #24 */

#ifdef	MCU_16BIT
	/* 16-bit MCU interface I  D[15:0],CSX,DCX,WRX,RDX */
	LCD_IM0_U = 1;
#else
	/* 8-bit MCU interface I  D[7:0],CSX,DCX,WRX,RDX */
	LCD_IM0_D = 1;
#endif
	LCD_IM1_D = 1;
	LCD_IM2_D = 1;
	LCD_IM3_D = 1;
	LCD_IM0_D = 1;
	
	/* backlight off */
	LCD_BLT_O = 0;

	delay_ms(100);

	/* Reset is not required after changing IMx but it
	   doesn't hurt to get everything into a known state */

	LCD_RSX_O = 0;
	delay_ms(10);
	LCD_RSX_O = 1;

	delay_ms(100);


	/* configure PMP */

	PMMODEbits.MODE16 = 1;
	PMMODEbits.MODE = 0b10;
	PMMODEbits.INCM = 0b00;

	PMAENbits.PTEN = 0x8000;

	PMCONbits.ADRMUX = 0b00;
	PMCONbits.PTWREN = 1;
	PMCONbits.PTRDEN = 1;
	PMCONbits.CSF = 0b00;
	PMCONbits.ON = 1;


	static uint32_t res = 0;


	PMADDR = 0x0000;

	uart2_str0("\n\rSleep Out ... ");

	lcd_pmp_cmd(0x11);

	uart2_str0("\n\rTE Line ON ... ");

	lcd_pmp_cmd(0x35);
	lcd_pmp_wr(0x00);

	uart2_str0("\n\rRead Display Status ... ");

	lcd_pmp_cmd(0x09);
	lcd_pmp_rd();				// previous
	res = lcd_pmp_rd();			// 1p dummy
	res = (res << 8) | lcd_pmp_rd();	// 2p [31:24]
	res = (res << 8) | lcd_pmp_rd();	// 3p [23:16]
	res = (res << 8) | lcd_pmp_rd();	// 4p [15:8]
	res = (res << 8) | lcd_pmp_rdf();	// 5p [7:0]

	uart2_long(res);

	uart2_str0("\n\rDisplay ON ... ");

	lcd_pmp_cmd(0x29);

	uart2_str0("\n\rRead Display Status ... ");

	lcd_pmp_cmd(0x09);
	lcd_pmp_rd();				// previous
	res = lcd_pmp_rd();			// 1p dummy
	res = (res << 8) | lcd_pmp_rd();	// 2p [31:24]
	res = (res << 8) | lcd_pmp_rd();	// 3p [23:16]
	res = (res << 8) | lcd_pmp_rd();	// 4p [15:8]
	res = (res << 8) | lcd_pmp_rdf();	// 5p [7:0]

	uart2_long(res);

	uart2_str0("\n\rNormal Mode ON ... ");

	lcd_pmp_cmd(0x13);

	uart2_str0("\n\rMemory Access ... ");

	lcd_pmp_cmd(0x36);
	lcd_pmp_wr(0x08);

	uart2_str0("\n\rPixel Format Set ... ");

	lcd_pmp_cmd(0x3A);
	lcd_pmp_wr(0x55);

	uart2_str0("\n\rColumn Address Set ... ");

	lcd_pmp_cmd(0x2A);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0xEF);

	uart2_str0("\n\rPage Address Set ... ");

	lcd_pmp_cmd(0x2B);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x01);
	lcd_pmp_wr(0x3F);


	uart2_str0("\n\rAnimation ... ");

	static uint16_t r = 0;
	static uint16_t g = 0;
	static uint16_t b = 0;

	while (1) {
	    while (!LCD_TE_I);
	    while (LCD_TE_I);

	    LCD_RSX_O = 0;
	    LCD_RSX_O = 1;

	    lcd_pmp_cmd(0x2C);

	    for (int x=0; x<320; x++) {
		uint16_t v = r | g | b;
		for (int y=0; y<240; y++) {
	    #ifdef  MCU_16BIT
		    lcd_pmp_wr(v);
	    #else
		    lcd_pmp_wr(v >> 8);
		    lcd_pmp_wr(v & 0xFF);
	    #endif
		    v += (1 << 11) + (1 << 5);
		}

		g = (g + (1 << 5)) & 0x07E0; 
		b = (b + 1) & 0x1F;
	    }

	    r += (1 << 11);
	    g = (g - (1 << 5)) & 0x07E0; 
	    b = (b + 1) & 0x1F;

	    LCD_RSX_O = 0;
	    LCD_RSX_O = 1;

	    /* backlight on */
	    LCD_BLT_O = 1;
	}

	PMADDR = 0x8000;

	uart2_str0("\n\rall done.");

	while (1);

	return 0;
}

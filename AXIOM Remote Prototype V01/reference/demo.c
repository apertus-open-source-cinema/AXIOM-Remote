
/*	PIC32MZ Demo Code
**
**	Copyright (C) 2018 Herbert Poetzl
**
**	This program is free software; you can redistribute it and/or modify
**    	it under the terms of the GNU General Public License 2 as published 
**	by the Free Software Foundation.
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
void	delay(unsigned cnt)
{
	unsigned i;
	while (cnt--)
	    for (i=0; i<200; i++);
}




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



int	main(void)
{
	static uint8_t rgb[4];
	static uint8_t rgbt[4];
	static uint8_t data[16];
	static uint8_t qe[2];
	static uint32_t timeout = 0;


	init_pbus();
	init_icsp_w();
	init_icsp_e();
	init_uart2();

	init_i2c_w();
	init_i2c_e();

	delay(1000);
	uart2_str0("\f\n\rAXIOM Remote Demo v0.1\n\r");

	delay(1000);
	set_mclr_w(1);
	set_mclr_e(1);

	delay(1000);

	rgb[0] = 0x18;
	rgb[1] = 0x28;
	rgb[2] = 0x38;
	rgb[3] = 0x01;
	i2c3_setn(0x20, rgb, 4);

	qe[0] = 0x0;
	qe[1] = 0x0;


	while (1) {
	    // asm volatile("wait");

	    i2c2_getn(0x00, data, 3);

	    if (data[0] || data[1] || data[2]) {
		uart2_byte(data[0]);
		uart2_byte(data[1]);
		uart2_byte(data[2]);
		uart2_ch('.');

		rgbt[0] = bitidx(data[0]);
		rgbt[1] = bitidx(data[1]);
		rgbt[2] = bitidx(data[2]);

		rgbt[3] = 1;

		i2c3_setn(0x20, rgbt, 4);
		timeout = 1000;	
	    }

	    i2c3_getn(0x00, data, 3);

	    if ((data[0] & 0x3F) || data[1] || (data[2] & 0x1F)) {
		uart2_byte(data[0]);
		uart2_byte(data[1]);
		uart2_byte(data[2]);
		uart2_ch('.');

		rgbt[0] = bitidx(data[0]);
		rgbt[1] = bitidx(data[1]);
		rgbt[2] = bitidx(data[2]);

		rgbt[3] = 1;

		i2c3_setn(0x20, rgbt, 4);
		timeout = 1000;	
	    }

	    i2c3_getn(0x10, data, 2);

	    if (data[0] != qe[0]) {
		qe[0] = data[0];
		uart2_byte(qe[0]);
		uart2_ch('.');

		rgbt[0] = qe[0] & 0xF;
		rgbt[1] = qe[0] & 0xF;
		rgbt[2] = qe[0] & 0xF;
		rgbt[3] = 1;

		i2c3_setn(0x20, rgbt, 4);

		timeout = 3000;	
	    }
	    
	    if (data[1] != qe[1]) {
		qe[1] = data[1];
		uart2_byte(qe[1]);
		uart2_ch('.');

		rgbt[0] = qe[1] & 0xF;
		rgbt[1] = qe[1] & 0xF;
		rgbt[2] = qe[1] & 0xF;
		rgbt[3] = 1;

		i2c3_setn(0x20, rgbt, 4);

		timeout = 3000;	
	    }
	    


	    // data[0] = i2c3_get(0x10);
	    // data[1] = i2c3_get(0x11);


	    if (timeout > 1) {
		timeout--;
	    } else if (timeout == 1) {
		i2c3_setn(0x20, rgb, 4);
		timeout--;

		uart2_str0("idle\n");
	    }
	    delay(10);
	}

	return 0;
}

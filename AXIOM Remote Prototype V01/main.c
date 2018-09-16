
/*	AXIOM Remote
**
**	Copyright (C) 2018 Herbert Poetzl, Sebastian Pichelhofer
 * 
 * code partly based on Adafruit ILI9341 TFT Displays written by Limor "ladyada" Fried for Adafruit Industries.
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
#include "glcdfont.c"
#include "gfxfont.h"
#include "gradient.c"
#include <string.h>
#include <stdio.h>
#include "FreeSans9pt7b.h"
#include "axiom-logo.c"
#include "definitions.h"

// LCD text drawing cursor
uint16_t cursor_x = 0;
uint16_t cursor_y = 0;


// LCD text drawing font
GFXfont gfxFont;
typedef enum {left, center, right} textAlign; 


// AXIOM Remote buttons and knobs
bool btn_E1_pressed = false;
bool btn_E2_pressed = false;
bool btn_S1_pos = false;
bool btn_TS1_pos = false;
bool btn_TS2_pos = false;
uint8_t E1_pos = 0;
uint8_t E2_pos = 0;


// the central frame buffer that is written to the LCD
uint16_t framebuffer[_width][_height];


// Menu related stuff
uint8_t menuSelectionIndex = 0;
enum currentMenu {Main, Submenu1, Submenu2}; 
enum currentMenu CurrentMenu;
char menuItemLabels[10][20]; //Main Menu
char menuItemValuesText[10][10];
uint8_t menuItemValues[10];
uint8_t menu_offset = 0;
uint8_t parameter_menu_active;
uint8_t parameterSelectionIndex;
typedef struct drop_down_choice_t {
    char      label[32];
    uint8_t   value;
} drop_down_choice_t;
drop_down_choice_t mainMenuItem2Choices[2];
uint8_t mainMenuItem2;
drop_down_choice_t mainMenuItem3Choices[4];
uint8_t mainMenuItem3;
typedef enum {submenu, readonly, numeric, dropdown} menu_item_type_t; 
typedef struct menu_item_t {
    char              label[64];
    bool              disabled;
    bool              hidden;
    menu_item_type_t  type; 
    uint8_t           value;
    uint16_t          UID;
} menu_item_t;
menu_item_t _menu_main_item[10];
char menu_breadcrumbs[64];
char submenu1_item_labels[5][20];
char submenu2_item_labels[4][20];
char submenu3_item_labels[3][20];


// Color Definitions
uint16_t menu_item_color; 
uint16_t menu_dimmed_item_color; 
uint16_t _menu_disabled_item_color; 
uint16_t menu_selected_item_color; 
uint16_t _menu_hightlighted_item_color; 
uint16_t menu_background_color;
uint16_t menu_text_color;
uint16_t _menu_disabled_text_color; 
uint16_t _menu_selected_text_color;


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

int8_t LimitRange(int8_t in, int8_t min, int8_t max) {
    if (in >= max) {
        return max;
    } 
    if (in <= min) {
        return min;
    }
    return in;
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
        unsigned i;
	    for (i=0; i<49; i++)
		asm("nop");
	}
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


/**************************************************************************/
/*!
    @brief   Set the "address window" - the rectangle we will write to RAM with the next chunk of SPI data writes. The ILI9341 will automatically wrap the data as each row is filled
    @param   x  TFT memory 'x' origin
    @param   y  TFT memory 'y' origin
    @param   w  Width of rectangle
    @param   h  Height of rectangle
*/
/**************************************************************************/
void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    // to fix orientation and set origin in lower left corner
    /*int16_t _x = 240 - y;
    int16_t _y = x;
    */
    int16_t _x = x;
    int16_t _y = y;
    
    uint32_t xa = ((uint32_t)_x << 16) | (_x+h-1);
    uint32_t ya = ((uint32_t)_y << 16) | (_y+w-1);
    
    lcd_pmp_cmd(ILI9341_CASET); // Column addr set
    lcd_pmp_wr32(xa);
    
    lcd_pmp_cmd(ILI9341_PASET); // Row addr set
    lcd_pmp_wr32(ya);
    
    lcd_pmp_cmd(ILI9341_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
   @brief  Read 8 bits of data from ILI9341 configuration memory. NOT from RAM!
           This is highly undocumented/supported, it's really a hack but kinda works?
    @param    command  The command register to read data from
    @param    index  The byte index into the command to read from
    @return   Unsigned 8-bit data read from ILI9341 register
*/
/**************************************************************************/
uint8_t read_command8(uint8_t command, uint8_t index) {
	lcd_pmp_cmd(0xD9);
    lcd_pmp_wr(0x10 + index);
    lcd_pmp_cmd(command);
	uint8_t ret = lcd_pmp_rd();
    return ret;
}

/**************************************************************************/
/*!
   @brief    Draw a pixel to the canvas framebuffer
    @param   x   x coordinate
    @param   y   y coordinate
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void drawPixel(int16_t x, int16_t y, uint16_t color) {
    framebuffer[x][y] = color;
}

/**************************************************************************/
/*!
   @brief    Write a line.  Bresenham's algorithm - thx wikpedia
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            drawPixel(y0, x0, color);
        } else {
            drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly vertical line (this is often optimized in a subclass!)
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    drawLine(x, y, x, (int16_t)y+h-1, color);
}

/**************************************************************************/
/*!
   @brief  Fill a rectangle, includes code for SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    w  Width of rectangle
    @param    h  Height of rectangle
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    /*int16_t x2 = x + w - 1, y2 = y + h - 1;
    if((x2 < 0) || (y2 < 0)) return;

    // Clip left/top
    if(x < 0) {
        x = 0;
        w = x2 + 1;
    }
    if(y < 0) {
        y = 0;
        h = y2 + 1;
    }

    // Clip right/bottom
    if(x2 >= _width)  w = _width  - x;
    if(y2 >= _height) h = _height - y;

    int32_t len = (int32_t)w * h;
    setAddrWindow(x, y, w, h);
    writeColor(color, len);*/
    int16_t i;
    for (i=x; i<x+w; i++) {
        drawFastVLine(i, y, h, color);
    }
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly horizontal line (this is often optimized in a subclass!)
    @param    x   Left-most x coordinate
    @param    y   Left-most y coordinate
    @param    w   Width in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    drawLine(x, y, x+w-1, y, color);
}

/**************************************************************************/
/*!
   @brief    Draw a circle outline
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0  , y0+r, color);
    drawPixel(x0  , y0-r, color);
    drawPixel(x0+r, y0  , color);
    drawPixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

/**************************************************************************/
/*!
   @brief    Quarter-circle drawer, used to do circles and roundrects
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

/**************************************************************************/
/*!
   @brief    Quarter-circle drawer with fill, used to do circles and roundrects
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing
    @param    delta  Offset from center-point, used for round-rects
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) {
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;

        if (cornername & 0x1) {
            drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
            drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
        }
        if (cornername & 0x2) {
            drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
            drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
        }
    }
}

/**************************************************************************/
/*!
   @brief    Draw a circle with filled color
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    drawFastVLine(x0, y0-r, 2*r+1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

/**************************************************************************/
/*!
   @brief   Draw a rectangle with no fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y+h-1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x+w-1, y, h, color);
}

/**************************************************************************/
/*!
   @brief   Draw a rounded rectangle with no fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    r   Radius of corner rounding
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    drawFastHLine(x+r  , y    , w-2*r, color); // Top
    drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
    drawFastVLine(x    , y+r  , h-2*r, color); // Left
    drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
    
    // draw four corners
    drawCircleHelper(x+r    , y+r    , r, 1, color);
    drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
    drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

/**************************************************************************/
/*!
   @brief   Draw a rounded rectangle with fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    r   Radius of corner rounding
    @param    color 16-bit 5-6-5 Color to draw/fill with
*/
/**************************************************************************/
void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    fillRect(x+r, y, w-2*r, h, color);

    // draw four corners
    fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

/**************************************************************************/
/*!
    @brief  Pass 8-bit (each) R,G,B, get back 16-bit packed color
            This function converts 8-8-8 RGB data to 16-bit 5-6-5
    @param    red   Red 8 bit color
    @param    green Green 8 bit color
    @param    blue  Blue 8 bit color
    @return   Unsigned 16-bit down-sampled color in 5-6-5 format
*/
/**************************************************************************/
uint16_t color565(uint8_t red, uint8_t green, uint8_t blue) {
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}


//doesnt work, extended command?
void setLCDBacklight(uint8_t brightness) {
    lcd_pmp_cmd(0x51);
	lcd_pmp_wr(brightness);
}

/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color, no background)
    @param    size  Font magnification level, 1 is 'original' size
*/
/**************************************************************************/
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) {
    if(0) { // Classic built-in font
        if((x >= _width)            || // Clip right
           (y >= _height)           || // Clip bottom
           ((x + 6 * size - 1) < 0) || // Clip left
           ((y + 8 * size - 1) < 0)) {   // Clip top
            return;
        }
        //bool _cp437 = true;
        //if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

        // 5x7 characters
        int8_t i;
        for(i=0; i<5; i++) { // Char bitmap = 5 columns
            uint8_t line = pgm_read_byte(&font[c * 5 + i]);
            int8_t j;
            for(j=0; j<8; j++, line >>= 1) {
                if(line & 1) {
                    if(size == 1)
                        drawPixel(x+i, y+7-j, color);
                    else
                        fillRect(x+i*size, y+7-j*size, size, size, color);
                } else if(bg != color) {
                    if(size == 1)
                        drawPixel(x+i, y+7-j, bg);
                    else
                        fillRect(x+i*size, y+7-j*size, size, size, bg);
                }
            }
        }
        if(bg != color) { // If opaque, draw vertical line for last column
            if(size == 1) {
                drawFastVLine(x+5, y, 8, bg);
            }
            else {          
                fillRect(x+5*size, y, size, 8*size, bg);
            }
        } 
    } else { // Custom font

        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling
        // drawChar() directly with 'bad' characters of font may cause mayhem!

        c -= (uint8_t)pgm_read_byte(&gfxFont.first);
        GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont.glyph))[c]);
        uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont.bitmap);

        uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
        uint8_t  w  = pgm_read_byte(&glyph->width),
                 h  = pgm_read_byte(&glyph->height);
        int8_t   xo = pgm_read_byte(&glyph->xOffset),
                 yo = pgm_read_byte(&glyph->yOffset);
        uint8_t  xx, yy, bits = 0, bit = 0;
        int16_t  xo16 = 0, yo16 = 0;

        if(size > 1) {
            xo16 = xo;
            yo16 = yo;
        }

        // Todo: Add character clipping here

        // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
        // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
        // has typically been used with the 'classic' font to overwrite old
        // screen contents with new data.  This ONLY works because the
        // characters are a uniform size; it's not a sensible thing to do with
        // proportionally-spaced fonts with glyphs of varying sizes (and that
        // may overlap).  To replace previously-drawn text when using a custom
        // font, use the getTextBounds() function to determine the smallest
        // rectangle encompassing a string, erase the area with fillRect(),
        // then draw new text.  This WILL infortunately 'blink' the text, but
        // is unavoidable.  Drawing 'background' pixels will NOT fix this,
        // only creates a new set of problems.  Have an idea to work around
        // this (a canvas object type for MCUs that can afford the RAM and
        // displays supporting setAddrWindow() and pushColors()), but haven't
        // implemented this yet.

        for(yy=0; yy<h; yy++) {
            for(xx=0; xx<w; xx++) {
                if(!(bit++ & 7)) {
                    bits = pgm_read_byte(&bitmap[bo++]);
                }
                if(bits & 0x80) {
                    if(size == 1) {
                        drawPixel(x+xo+xx, y+yo+yy, color);
                    } else {
                        fillRect(x+(xo16+xx)*size, y+(yo16+yy)*size,
                          size, size, color);
                    }
                }
                bits <<= 1;
            }
        }
    }
}

/**************************************************************************/
/*!
   @brief  Draw RGB rectangle of data from RAM to a location on screen
   Adapted from https://github.com/PaulStoffregen/ILI9341_t3
   by Marc MERLIN. See examples/pictureEmbed to use this.
   5/6/2017: function name and arguments have changed for compatibility
   with current GFX library and to avoid naming problems in prior
   implementation.  Formerly drawBitmap() with arguments in different order.

    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    pcolors Pointer to 16-bit color data
    @param    w  Width of pcolors rectangle
    @param    h  Height of pcolors rectangle
*/
/**************************************************************************/
// Options for converting images to the format used here include:
//   http://www.rinkydinkelectronics.com/t_imageconverter565.php
// or
//  GIMP (https://www.gimp.org/) as follows:
//    1. File -> Export As
//    2. In Export Image dialog, use 'C source code (*.c)' as filetype.
//    3. Press export to get the export options dialog.
//    4. Type the desired variable name into the 'prefixed name' box.
//    5. Uncheck 'GLIB types (guint8*)'
//    6. Check 'Save as RGB565 (16-bit)'
//    7. Press export to save your image.
//  Assuming 'image_name' was typed in the 'prefixed name' box of step 4,
//  you can have to include the c file, then using the image can be done with:
//    tft.drawRGBBitmap(0, 0, image_name.pixel_data, image_name.width, image_name.height);
//  See also https://forum.pjrc.com/threads/35575-Export-for-ILI9341_t3-with-GIMP
void drawRGBBitmap(int16_t x, int16_t y, const uint16_t *pcolors, int16_t w, int16_t h) {
    //rotation fix
    int16_t temp = w;
    w = h;
    h = temp;
    
    int16_t x2, y2; // Lower-right coord
    if(( x             >= _width ) ||      // Off-edge right
       ( y             >= _height) ||      // " top
       ((x2 = (x+w-1)) <  0      ) ||      // " left
       ((y2 = (y+h-1)) <  0)     ) return; // " bottom

    int16_t bx1=0, by1=0, // Clipped top-left within bitmap
            saveW=w;      // Save original bitmap width value
    if(x < 0) { // Clip left
        w  +=  x;
        bx1 = -x;
        x   =  0;
    }
    if(y < 0) { // Clip top
        h  +=  y;
        by1 = -y;
        y   =  0;
    }
    if(x2 >= _width ) w = _width  - x; // Clip right
    if(y2 >= _height) h = _height - y; // Clip bottom

    uint16_t draw_x;
    uint16_t draw_y;
    
    for (draw_x=0; draw_x<w; draw_x++){
        for (draw_y=0; draw_y<h; draw_y++){
            //drawPixel(draw_y+y, _height-x+w+draw_x, *pcolors++);
            drawPixel(draw_y+x, y+draw_x, *pcolors++);
            //drawPixel(draw_y+x, _height-y+w+draw_x, *pcolors++);
        }
    }
    /*pcolors += by1 * saveW + bx1; // Offset bitmap ptr to clipped top-left
    setAddrWindow(x, y, w, h); // Clipped area
    while(h--) { // For each (clipped) scanline...
      drawPixel(pcolors, w); // Push one (clipped) row
      pcolors += saveW; // Advance pointer by one full (unclipped) line
    }*/
}

void setCursor(int16_t x, int16_t y) {
    cursor_x = x;
    cursor_y = y;
}

/**************************************************************************/
/*!
    @brief    Helper to determine size of a character with current font/size.
       Broke this out as it's used by both the PROGMEM- and RAM-resident getTextBounds() functions.
    @param    c     The ascii character in question
    @param    x     Pointer to x location of character
    @param    y     Pointer to y location of character
    @param    minx  Minimum clipping value for X
    @param    miny  Minimum clipping value for Y
    @param    maxx  Maximum clipping value for X
    @param    maxy  Maximum clipping value for Y
*/
/**************************************************************************/
void getCharBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy, uint8_t textsize) {
    bool wrap = false;
    if(1) {
        if(c == '\n') { // Newline?
            *x  = 0;    // Reset x to zero, advance y by one line
            *y += textsize * (uint8_t)pgm_read_byte(&gfxFont.yAdvance);
        } else if(c != '\r') { // Not a carriage return; is normal char
            uint8_t first = pgm_read_byte(&gfxFont.first),
                    last  = pgm_read_byte(&gfxFont.last);
            if((c >= first) && (c <= last)) { // Char present in this font?
                GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(
                  &gfxFont.glyph))[c - first]);
                uint8_t gw = pgm_read_byte(&glyph->width),
                        gh = pgm_read_byte(&glyph->height),
                        xa = pgm_read_byte(&glyph->xAdvance);
                int8_t  xo = pgm_read_byte(&glyph->xOffset),
                        yo = pgm_read_byte(&glyph->yOffset);
                if(wrap && ((*x+(((int16_t)xo+gw)*textsize)) > _width)) {
                    *x  = 0; // Reset x to zero, advance y by one line
                    *y += textsize * (uint8_t)pgm_read_byte(&gfxFont.yAdvance);
                }
                int16_t ts = (int16_t)textsize,
                        x1 = *x + xo * ts,
                        y1 = *y + yo * ts,
                        x2 = x1 + gw * ts - 1,
                        y2 = y1 + gh * ts - 1;
                if(x1 < *minx) *minx = x1;
                if(y1 < *miny) *miny = y1;
                if(x2 > *maxx) *maxx = x2;
                if(y2 > *maxy) *maxy = y2;
                *x += xa * ts;
            }
        }
    }
}

/**************************************************************************/
/*!
    @brief    Helper to determine size of a string with current font/size. Pass string and a cursor position, returns UL corner and W,H.
    @param    str     The ascii string to measure
    @param    x       The current cursor X
    @param    y       The current cursor Y
    @param    x1      The boundary X coordinate, set by function
    @param    y1      The boundary Y coordinate, set by function
    @param    w      The boundary width, set by function
    @param    h      The boundary height, set by function
*/
/**************************************************************************/
void getTextBounds(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    uint8_t c; // Current character

    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;

    while((c = *str++))
        getCharBounds(c, &x, &y, &minx, &miny, &maxx, &maxy,1);

    if(maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if(maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }
}

void drawString(int16_t x, int16_t y, char* text, uint16_t color, uint16_t bg, uint8_t size, textAlign align, uint16_t textwidth) {
    //bool wrap = false;
    if (0) {  //old font  
        uint8_t letterwidth = 6;

        uint8_t length = strlen(text);
        uint8_t i;
        for (i = 0; i<length; i++) {
            char c = text[i];
            drawChar(x+letterwidth*i*size, y, c, color, bg, size);
        }
    } else { // Custom font
        uint8_t first = pgm_read_byte(&gfxFont.first);
        uint8_t last = pgm_read_byte(&gfxFont.last);
        uint8_t length = strlen(text);
        
        cursor_x = x;
        cursor_y = y;
        
        uint16_t x1, y1, w1, h1;
        
        getTextBounds(text, x, y, &x1, &y1, &w1, &h1);
        //asdasd
        
        uint8_t i;
        for (i = 0; i<length; i++) {
            char c = text[i];
            
            if (c == 32) { // space " " character 
                cursor_x += 10;
            }
            
            if((c >= first) && (c <= last)) {
                GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont.glyph))[c - first]);
                uint8_t   w     = pgm_read_byte(&glyph->width),
                          h     = pgm_read_byte(&glyph->height);
                if((w > 0) && (h > 0)) { // Is there an associated bitmap?
                    int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
                    /*if(wrap && ((cursor_x + size * (xo + w)) > _width)) {
                        cursor_x  = 0;
                        cursor_y += (int16_t)size * (uint8_t)pgm_read_byte(&gfxFont.yAdvance);
                    }*/
                    if (align == left) {
                        drawChar(cursor_x+xo, cursor_y, c, color, bg, size);
                        cursor_x += (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)size; 
                    }
                    if (align == center) {
                       //TODO
                    }
                    if ((align == right) && (textwidth > 0)) {
                        drawChar(cursor_x+xo+textwidth-w1, cursor_y, c, color, bg, size);
                        cursor_x += (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)size; 
                    }
                    
                }
            }
        }
    }
    
    /*if(text == '\n') {                        // Newline?
        cursor_x  = 0;                     // Reset x to zero,
        cursor_y += size * 8;          // advance y one line
    } else if(c != '\r') {                 // Ignore carriage returns
        if(wrap && ((cursor_x + size * 6) > _width)) { // Off right?
            cursor_x  = 0;                 // Reset x to zero,
            cursor_y += size * 8;      // advance y one line
        }
        drawChar(cursor_x, cursor_y, c, color, bg, size);
        cursor_x += size * 6;          // Advance x one char
    }*/
}

void clearFramebuffer(uint16_t color) {
    uint16_t x;
    uint8_t y;
    for (x=0; x<_width; x++) {
        for (y=0; y<_height; y++) {
            framebuffer[x][y] = color;
        }
    }
}

void displayFramebuffer() {
    lcd_pmp_cmd(ILI9341_CASET);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0xEF);

	lcd_pmp_cmd(ILI9341_PASET);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x01);
	lcd_pmp_wr(0x3F);
    
    lcd_pmp_cmd(ILI9341_RAMWR);
            
    uint16_t x;
    uint8_t y;
    
    for (x=0; x<_width; x++) {
        for (y=0; y<_height; y++) {
        //for (y=_height; y>0; y--) { // we flip y axis so the origin in in the lower left corner
            lcd_pmp_wr(framebuffer[x][y]);
        }
    }
}

void lcd_init() {
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

    
    // Software Reset
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
    
    if (0) { // init commands from manufacturer datasheet don't seem to make any difference
        lcd_pmp_cmd(0x01); //software reset
        delay_ms(5);
        lcd_pmp_cmd(0x28); //display off
        
        lcd_pmp_cmd(0xEF);
        lcd_pmp_wr(0x03);
        lcd_pmp_wr(0x80);
        lcd_pmp_wr(0x02);

        lcd_pmp_cmd(0xCF);
        lcd_pmp_wr(0x00);
        lcd_pmp_wr(0XC1);
        lcd_pmp_wr(0X30);

        lcd_pmp_cmd(0xED);
        lcd_pmp_wr(0x64);
        lcd_pmp_wr(0x03);
        lcd_pmp_wr(0X12);
        lcd_pmp_wr(0X81);

        lcd_pmp_cmd(0xE8);
        lcd_pmp_wr(0x85);
        lcd_pmp_wr(0x00);
        lcd_pmp_wr(0x78);

        lcd_pmp_cmd(0xCB);
        lcd_pmp_wr(0x39);
        lcd_pmp_wr(0x2C);
        lcd_pmp_wr(0x00);
        lcd_pmp_wr(0x34);
        lcd_pmp_wr(0x02);

        lcd_pmp_cmd(0xF7);
        lcd_pmp_wr(0x20);

        lcd_pmp_cmd(0xEA);
        lcd_pmp_wr(0x00);
        lcd_pmp_wr(0x00);
    }

	uart2_str0("\n\rSleep Out ... ");

	lcd_pmp_cmd(0x11);

	uart2_str0("\n\rTE Line ON ... ");

	lcd_pmp_cmd(0x35);
	lcd_pmp_wr(0x00);

	uart2_str0("\n\rRead Display Status ... ");

	lcd_pmp_cmd(ILI9341_RDDST);
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

	lcd_pmp_cmd(ILI9341_RDDST);
	lcd_pmp_rd();				// previous
	res = lcd_pmp_rd();			// 1p dummy
	res = (res << 8) | lcd_pmp_rd();	// 2p [31:24]
	res = (res << 8) | lcd_pmp_rd();	// 3p [23:16]
	res = (res << 8) | lcd_pmp_rd();	// 4p [15:8]
	res = (res << 8) | lcd_pmp_rdf();	// 5p [7:0]

	uart2_long(res);

	uart2_str0("\n\rNormal Mode ON ... ");

	lcd_pmp_cmd(ILI9341_NORON);

	uart2_str0("\n\rMemory Access ... ");

	lcd_pmp_cmd(0x36);
	lcd_pmp_wr(0x08);

	uart2_str0("\n\rPixel Format Set ... ");

	lcd_pmp_cmd(ILI9341_PIXFMT);
	lcd_pmp_wr(0x55);

	uart2_str0("\n\rColumn Address Set ... ");

	lcd_pmp_cmd(0x2A);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0xEF);

	uart2_str0("\n\rPage Address Set ... ");

	lcd_pmp_cmd(ILI9341_PASET);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x00);
	lcd_pmp_wr(0x01);
	lcd_pmp_wr(0x3F);
    
    lcd_pmp_cmd(0xF2);    // 3Gamma Function Disable
    lcd_pmp_wr(0x00);

    lcd_pmp_cmd(ILI9341_GAMMASET);    //Gamma curve selected
    lcd_pmp_wr(0x01);

    lcd_pmp_wr(ILI9341_GMCTRP1);    //Set Gamma
    lcd_pmp_wr(0x0F);
    lcd_pmp_wr(0x31);
    lcd_pmp_wr(0x2B);
    lcd_pmp_wr(0x0C);
    lcd_pmp_wr(0x0E);
    lcd_pmp_wr(0x08);
    lcd_pmp_wr(0x4E);
    lcd_pmp_wr(0xF1);
    lcd_pmp_wr(0x37);
    lcd_pmp_wr(0x07);
    lcd_pmp_wr(0x10);
    lcd_pmp_wr(0x03);
    lcd_pmp_wr(0x0E);
    lcd_pmp_wr(0x09);
    lcd_pmp_wr(0x00);

    lcd_pmp_wr(ILI9341_GMCTRN1);    //Set Gamma
    lcd_pmp_wr(0x00);
    lcd_pmp_wr(0x0E);
    lcd_pmp_wr(0x14);
    lcd_pmp_wr(0x03);
    lcd_pmp_wr(0x11);
    lcd_pmp_wr(0x07);
    lcd_pmp_wr(0x31);
    lcd_pmp_wr(0xC1);
    lcd_pmp_wr(0x48);
    lcd_pmp_wr(0x08);
    lcd_pmp_wr(0x0F);
    lcd_pmp_wr(0x0C);
    lcd_pmp_wr(0x31);
    lcd_pmp_wr(0x36);
    lcd_pmp_wr(0x0F);
    
    gfxFont = FreeSans9pt7b;
    
    // Clear the image
    clearFramebuffer(ILI9341_WHITE);
}

uint8_t get_current_menu_item_count() {
    if (CurrentMenu == Main) {
        return sizeof (menuItemLabels) / sizeof *(menuItemLabels);
    }
    if (CurrentMenu == Submenu1) {
        return sizeof (submenu1_item_labels) / sizeof *(submenu1_item_labels);
    }
    if (CurrentMenu == Submenu2) {
        return sizeof (submenu2_item_labels) / sizeof *(submenu2_item_labels);
    }
}

uint8_t getCurrentParameterItemCount() {
    if (parameter_menu_active == 2)
        return 2;
    if (parameter_menu_active == 5)
        return 4;
}

/**************************************************************************/
/*!
   @brief  initialize our menu structure with default values and definitions
 * 
*/
/**************************************************************************/
void initMenu(){
    //Color Theme definitions
    menu_item_color = color565(255, 255, 255); 
    menu_dimmed_item_color = color565(247, 251, 247); 
    _menu_disabled_item_color  = color565(255, 255, 255); 
    menu_selected_item_color = color565(255, 128, 0); 
    menu_background_color = color565(180, 180, 180);
    menu_text_color = color565(0, 0, 0);
    _menu_selected_text_color = color565(255, 255, 255);
    _menu_hightlighted_item_color = color565(0, 128, 255);
    _menu_disabled_text_color = color565(230, 230, 230);

    
   /*
    //sprintf(menuItemValuesText[2], mainMenuItem2 ? "On" : "Off");
    //sprintf(menuItemValuesText[3], "%d", menuItemValues[3]);
    sprintf(menuItemValuesText[4], "%d", menuItemValues[4]);
    //sprintf(menuItemValuesText[5], "%d", menuItemValues[5]);
    sprintf(menuItemValuesText[6], "%d", menuItemValues[6]);
    sprintf(menuItemValuesText[7], "%d", menuItemValues[7]);
    sprintf(menuItemValuesText[8], "%d", menuItemValues[8]);
    sprintf(menuItemValuesText[9], "%d", menuItemValues[9]);*/
    
    
    strcpy(submenu1_item_labels[0], "<- back");
    strcpy(submenu1_item_labels[1], "Submenu1 Item 1");
    strcpy(submenu1_item_labels[2], "Submenu1 Item 2");
    strcpy(submenu1_item_labels[3], "Submenu1 Item 3");
    strcpy(submenu1_item_labels[4], "Submenu1 Item 4");
    
    
    strcpy(submenu2_item_labels[0], "<- back");
    strcpy(submenu2_item_labels[1], "Submenu2 Item 1");
    strcpy(submenu2_item_labels[2], "Submenu2 Item 2");
    strcpy(submenu2_item_labels[3], "Submenu2 Item 3");
    
    mainMenuItem2Choices[0].value = 0;
    strcpy(mainMenuItem2Choices[0].label, "Off");
    mainMenuItem2Choices[1].value = 1;
    strcpy(mainMenuItem2Choices[1].label, "On");
    mainMenuItem2 = 0;
    
    mainMenuItem3Choices[0].value = 0;
    strcpy(mainMenuItem3Choices[0].label, "Low");
    mainMenuItem3Choices[1].value = 1;
    strcpy(mainMenuItem3Choices[1].label, "Medium");
    mainMenuItem3Choices[2].value = 2;
    strcpy(mainMenuItem3Choices[2].label, "High");
    mainMenuItem3Choices[3].value = 3;
    strcpy(mainMenuItem3Choices[3].label, "Crazy");
    mainMenuItem3 = 0;
    
    
    // new menu structure definitions
    _menu_main_item[0].disabled = false;
    _menu_main_item[0].hidden = false;
    strcpy(_menu_main_item[0].label, "Submenu 1");
    _menu_main_item[0].type = submenu;
    
    _menu_main_item[1].disabled = false;
    _menu_main_item[1].hidden = false;
    strcpy(_menu_main_item[1].label, "Submenu 2");
    _menu_main_item[1].type = submenu;
    
    _menu_main_item[2].disabled = true;
    _menu_main_item[2].hidden = false;
    strcpy(_menu_main_item[2].label, "disabled sample");
    _menu_main_item[2].type = dropdown;
    
    _menu_main_item[3].disabled = false;
    _menu_main_item[3].hidden = false;
    strcpy(_menu_main_item[3].label, "Fun");
    _menu_main_item[3].type = dropdown;
    
    _menu_main_item[4].disabled = false;
    _menu_main_item[4].hidden = false;
    strcpy(_menu_main_item[4].label, "Fun Level");
    _menu_main_item[4].type = dropdown;
    
    CurrentMenu = Main;
    parameter_menu_active = 0;
    parameterSelectionIndex = 0;
    menu_offset = 0;
    
    strcpy(menu_breadcrumbs, "Menu");
}

/**************************************************************************/
/*!
   @brief  Draw a menu where the parameter of a certain option is displayed.
 * This typically looks like a kind of drop-down menu with several options to choose from
 * 

    @param    x  location (attention: top right corner)
    @param    y  location (attention: top right corner)
    @param    items[] the struct containing the different options to display
    @param    currentvalueindex which option (index) from the struct is the current setting
*/
/**************************************************************************/
void draw_parameter_menu(uint16_t x, uint16_t y, drop_down_choice_t items[], uint8_t currentvalueindex){
    uint8_t choice_number = getCurrentParameterItemCount(); // why doesn't this work?: sizeof (items) / sizeof (*items); //
    uint16_t height = 2 + 2 + choice_number * 30;
    
    //calculate the maximum width of the provided text options so we can define the menu width
    uint16_t max_width = 0;
    uint8_t i;
    for (i=0; i<choice_number; i++) {
        uint16_t x1,y1,w1,h1;
        getTextBounds(items[i].label, x, y, &x1, &y1, &w1, &h1);
        if (w1 > max_width) {
            max_width = w1;
        }
    }
    // add 25 pixel space to the widest label
    uint16_t width = max_width +25;
    
    // alter x now that we know the width of the menu
    // x and y define the top right corner here
    x -= width; 
    
    //don't draw over the LCD edge
    int16_t edge = (y+height) - _height;
    if ((edge < 4) && (edge > 0)){
        height = _height - y;
    }
    
    //if the menu would clip entire lines in the available height just shift it up as a whole
    if (edge > 4) {
        y -= edge-3;
        height = _height - y;
    }
    
    // todo
    // handle the case that there are more parameter options than fit in the height of the LCD
    
    // draw menu borders
    fillRect (x, y, width, height, menu_background_color);
    drawRect (x+1, y+1, width-2, height-2, menu_text_color);
    
    for (i=0; i<choice_number; i++){
        char draw_label[32];
        strcpy(draw_label, items[i].label);
        
        if (i == parameterSelectionIndex) {
            if (btn_E1_pressed){
                fillRect(x+2, y+2+i*30, width-4, 29, _menu_hightlighted_item_color);
                drawString(x+12, y+21+i*30, draw_label, _menu_selected_text_color, _menu_selected_text_color, 1, left, 0); 
            } else {
                fillRect(x+2, y+2+i*30, width-4, 29, menu_selected_item_color);
                drawString(x+12, y+21+i*30, draw_label, _menu_selected_text_color, _menu_selected_text_color, 1, left, 0); 
            }
        } else {
            fillRect(x+2, y+2+i*30, width-4, 29, menu_item_color);
            drawString(x+12, y+21+i*30, draw_label, menu_text_color, menu_text_color, 1, left, 0); 
        }
        
        // add a circle icon at beginning of the line and label of the current setting
        if (i == currentvalueindex) {
            if (i == parameterSelectionIndex) {
              fillCircle(x+6, y+15+i*30, 3, _menu_selected_text_color);
            } else {
                fillCircle(x+6, y+15+i*30, 3, menu_text_color);
            }
        }
    }
}

void btn_E1_released () {
    if (CurrentMenu == Main) {
        if (menuSelectionIndex == 0) {
            CurrentMenu = Submenu1;
            menuSelectionIndex = 0;
            strcpy(menu_breadcrumbs, "Menu > Submenu1");
        }
        if (menuSelectionIndex == 1) {
            CurrentMenu = Submenu2;
            menuSelectionIndex = 0;
            strcpy(menu_breadcrumbs, "Menu > Submenu2");
        }
        if ((menuSelectionIndex == 2) && (parameter_menu_active == 0)) {
            parameter_menu_active = 2;
        } else if ((menuSelectionIndex == 2) && (parameter_menu_active == 2)) {
            mainMenuItem2 = parameterSelectionIndex;
            parameter_menu_active = 0;
        }
        if ((menuSelectionIndex == 5) && (parameter_menu_active == 0)) {
            parameter_menu_active = 5;
        } else if ((menuSelectionIndex == 5) && (parameter_menu_active == 5)) {
            mainMenuItem3 = parameterSelectionIndex;
            parameter_menu_active = 0;
        }
    } else if (CurrentMenu == Submenu1) {
        if (menuSelectionIndex == 0) {
            CurrentMenu = Main;
            menuSelectionIndex = 0;
            strcpy(menu_breadcrumbs, "Menu");
        }
    } else if (CurrentMenu == Submenu2) {
        if (menuSelectionIndex == 0) {
            CurrentMenu = Main;
            menuSelectionIndex = 1;
            strcpy(menu_breadcrumbs, "Menu");
        }
    }
}
void btn_E2_released () {
    // to emulate the back button currently
    if (CurrentMenu == Submenu1) {
            CurrentMenu = Main;
            menuSelectionIndex = 0;
            strcpy(menu_breadcrumbs, "Menu");
    } else if (CurrentMenu == Submenu2) {
            CurrentMenu = Main;
            menuSelectionIndex = 1;
            strcpy(menu_breadcrumbs, "Menu");
    } else if (parameter_menu_active) {
        parameter_menu_active = 0;
    }
}

//void draw_menu_item (uint16_t x, uint16_t y, char* label, char* value, bool selected, bool highlighted){
void draw_menu_item (uint16_t x, uint16_t y, menu_item_t menu_main_item, bool selected, bool highlighted) {
    /*GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont.glyph))[65]);
    uint8_t h = pgm_read_byte(&glyph->height);*/
    
    char value[16];
    if (menu_main_item.type == submenu) {
        sprintf(value, ">");
    } else {
        sprintf(value, "%d", menu_main_item.value);
    }
        
    if (menu_main_item.disabled) {
        fillRect(x, y, _width, 29, _menu_disabled_item_color);
        drawString(x+5, y+19, menu_main_item.label, _menu_disabled_text_color, _menu_disabled_text_color, 1, left, 0); 
        drawString(x+210, y+19, value, _menu_disabled_text_color, _menu_disabled_text_color, 1, right, 80);
    } else if (highlighted) {
        fillRect(x, y, _width, 29, _menu_hightlighted_item_color);
        drawString(x+5, y+19, menu_main_item.label, _menu_selected_text_color, _menu_selected_text_color, 1, left, 0); 
        drawString(x+210, y+19, value, _menu_selected_text_color, _menu_selected_text_color, 1, right, 80); 
    } else if (!selected) {
        if (parameter_menu_active) {
            fillRect(x, y, _width, 29, menu_dimmed_item_color);
            drawString(x+5, y+19, menu_main_item.label, menu_text_color, menu_text_color, 1, left, 0); 
            drawString(x+210, y+19, value, menu_text_color, menu_text_color, 1, right, 80);
        } else {
            fillRect(x, y, _width, 29, menu_item_color);
            drawString(x+5, y+19, menu_main_item.label, menu_text_color, menu_text_color, 1, left, 0); 
            drawString(x+210, y+19, value, menu_text_color, menu_text_color, 1, right, 80); 
        }
    } else {
        fillRect(x, y, _width, 29, menu_selected_item_color);
        drawString(x+5, y+19, menu_main_item.label, _menu_selected_text_color, _menu_selected_text_color, 1, left, 0); 
        drawString(x+210, y+19, value, _menu_selected_text_color, _menu_selected_text_color, 1, right, 80);  
    }
}

void draw_scroll_indicator(uint8_t number, uint8_t CurrentMenuItemCount){
    uint8_t scrollbarheight = (_height-31) * (float)((float)number/(float)(CurrentMenuItemCount));
    uint8_t scrollbaroffset = 31 + menu_offset * (((_height-31) - scrollbarheight) / (CurrentMenuItemCount - number));
    
    //Background
    fillRect(_width-16, 31, 16, _height-31, menu_item_color);
    
    //Thin Line
    fillRect(_width-10, 31, 4, _height-31, menu_text_color);
    
    //Thick Line
    fillRect(_width-14, scrollbaroffset, 12, scrollbarheight, menu_text_color);
    //
}

void drawMenu(bool firstime) {
    if (firstime) {
        //clear the screen
        fillRect(0, 0, _width, _height, menu_background_color);
        
        //draw header
        fillRect(0, 0, _width, 28, menu_item_color);
        drawString(5, 19, menu_breadcrumbs, menu_text_color, menu_text_color, 1, left, 0); 
        drawLine(0, 29, _width, 29, menu_selected_item_color);
        drawLine(0, 30, _width, 30, menu_background_color);
    }
    
    /*strcpy(menuItemValuesText[0], ">");
    strcpy(menuItemValuesText[1], ">");
    sprintf(menuItemValuesText[2], mainMenuItem2Choices[mainMenuItem2].label);
    sprintf(menuItemValuesText[5], mainMenuItem3Choices[mainMenuItem3].label);
    sprintf(menuItemValuesText[4], "%d", menuItemValues[4]);
    //sprintf(menuItemValuesText[5], "%d", menuItemValues[5]);
    sprintf(menuItemValuesText[6], "%d", menuItemValues[6]);
    sprintf(menuItemValuesText[7], "%d", menuItemValues[7]);
    sprintf(menuItemValuesText[8], "%d", menuItemValues[8]);
    sprintf(menuItemValuesText[9], "%d", menuItemValues[9]);*/
    
    
    if (CurrentMenu == Main) {
        uint8_t i;
        int8_t displaySelectionIndex = menuSelectionIndex - menu_offset; 
        if (displaySelectionIndex >= 7) {
            menu_offset += 1;
        }
        if (displaySelectionIndex < 0) {
            menu_offset -= 1;
        }
        
        int number = 5;//sizeof (menuItemLabels) / sizeof *(menuItemLabels);
        
        number = LimitRange(number, 0, 7);
        
        for (i=0; i < number; i++){
            draw_menu_item(0, 31+i*30, _menu_main_item[i+menu_offset], i+menu_offset==menuSelectionIndex, ((i+menu_offset==menuSelectionIndex) && btn_E1_pressed));
            //draw_menu_item(0, 31+i*30, menuItemLabels[i+menu_offset], menuItemValuesText[i+menu_offset], i+menu_offset==menuSelectionIndex, ((i+menu_offset==menuSelectionIndex) && btn_E1_pressed));
        }
        if (number == 7) {
            draw_scroll_indicator(number, get_current_menu_item_count());
        }
        if (parameter_menu_active == 2) {
            uint16_t offset = 31 + (parameter_menu_active-menu_offset)*30 - 2;
            draw_parameter_menu(304, offset, mainMenuItem2Choices, mainMenuItem2);
        }
        if (parameter_menu_active == 5) {
            uint16_t offset = 31 + (parameter_menu_active-menu_offset)*30 - 2;
            draw_parameter_menu(304, offset, mainMenuItem3Choices, mainMenuItem3);
        }
    }
    
    if (CurrentMenu == Submenu1) {
        uint8_t i; 
        int number = sizeof (submenu1_item_labels) / sizeof *(submenu1_item_labels);
        for (i=0; i < number; i++){
           // draw_menu_item(0, 31+i*30, submenu1_item_labels[i], "", i==menuSelectionIndex, ((i==menuSelectionIndex) && btn_E1_pressed));
        }
    }
    
    if (CurrentMenu == Submenu2) {
        int i;
        int number = sizeof (submenu2_item_labels) / sizeof *(submenu2_item_labels);
        for (i=0; i < number; i++){
            //draw_menu_item(0, 31+i*30, submenu2_item_labels[i], "", i==menuSelectionIndex, ((i==menuSelectionIndex) && btn_E1_pressed));
        }
    }
    
    //drawRGBBitmap(10, 10, (uint16_t*)(gradient.pixel_data), gradient.width, gradient.height);
    //fillCircle(150, 5, 3, menuSelectedItemColor);
    //drawRGBBitmap(155, 0, (uint16_t*)(axiom_logo.pixel_data), axiom_logo.width, axiom_logo.height);
    
    //setLCDBacklight(menuItemValues[0]);
}

void updateFramebuffer(){
    drawMenu(true);
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

    // RGB LED
	rgb[0] = 0x18;
	rgb[1] = 0x28;
	rgb[2] = 0x38;
	rgb[3] = 0x01;
	i2c3_setn(0x20, rgb, 4);


    lcd_init();
    
    initMenu();
            
	static uint16_t r = 0;
	static uint16_t g = 0;
	static uint16_t b = 0;

    static uint8_t data[16];
    static uint8_t data_status[16];
    static uint8_t qe[2];
    
    drawMenu(true);

    
	while (1) {
        
        // Read Button Press Messages
        
        /*
         *  the first three registers (i2c3_getn(0x00, data, 3); data[0], data[1], data[2]) provide change information
         *  i.e. a bit set there shows that something on the respective input (port A,B or C) changed
         *  so, a button pressed or a button released will trigger a bit change there
         * 
         *  register 4 to 6 (i2c2_getn(0x04, data, 3) data[0], data[1], data[2]) contain the status registers of each button/knobs current state
         */
        
        i2c2_getn(0x00, data, 3);
	    if (data[0] || data[1] || data[2]) {
            //uart2_byte(data[0]);
            //uart2_byte(data[1]);
            //uart2_byte(data[2]);
            //uart2_str0("-1\n\r");
	    }

	    i2c3_getn(0x00, data, 3);
        i2c3_getn(0x04, data_status, 3);
        
	    if ((data[0] & 0x3F) || data[1] || (data[2] & 0x1F)) {
            //uart2_byte(data[0]);
            //uart2_byte(data[1]);
            //uart2_byte(data[2]);
            
            //uart2_str0(" -2- ");
            
            //uart2_byte(data_status[0]);
           // uart2_byte(data_status[1]);
            //uart2_byte(data_status[2]);
            
            //uart2_str0("\n\r");
            
            if (data[1] == 0x04) {
                btn_TS1_pos = true;
                //drawString(20, 200, "TS1: up  ", color565(0,0,0), color565(255,255,255), 1); 
            }
            if (data[1] == 0x08) {
                btn_TS1_pos = false;
                //drawString(20, 200, "TS1: down", color565(0,0,0), color565(255,255,255), 1); 
            }
            if (data[2] == 0x04) {
                btn_TS2_pos = true;
                lcd_pmp_wr(ILI9341_INVCTR);
                lcd_pmp_wr(0x07);
                drawString(20, 180, "TS2: up  ", color565(0,0,0), color565(255,255,255), 1, left, 0); 
                
                static uint32_t res = 0;
                uart2_str0("\n\rRead Display Status ... ");

                lcd_pmp_cmd(ILI9341_RDDST);
                lcd_pmp_rd();				// previous
                res = lcd_pmp_rd();			// 1p dummy
                res = (res << 8) | lcd_pmp_rd();	// 2p [31:24]
                res = (res << 8) | lcd_pmp_rd();	// 3p [23:16]
                res = (res << 8) | lcd_pmp_rd();	// 4p [15:8]
                res = (res << 8) | lcd_pmp_rdf();	// 5p [7:0]

                uart2_long(res);
    
            }
            if (data[2] == 0x08) {
                btn_TS2_pos = false;
                //testing display invesions
                //lcd_pmp_wr(ILI9341_INVCTR);
                //lcd_pmp_wr(0x02);
                drawString(20, 180, "TS2: down", color565(0,0,0), color565(255,255,255), 1, left, 0); 
                
                static uint32_t res = 0;
                uart2_str0("\n\rRead Display Status ... ");

                lcd_pmp_cmd(ILI9341_RDDST);
                lcd_pmp_rd();				// previous
                res = lcd_pmp_rd();			// 1p dummy
                res = (res << 8) | lcd_pmp_rd();	// 2p [31:24]
                res = (res << 8) | lcd_pmp_rd();	// 3p [23:16]
                res = (res << 8) | lcd_pmp_rd();	// 4p [15:8]
                res = (res << 8) | lcd_pmp_rdf();	// 5p [7:0]

                uart2_long(res);
    
            }
            if (data_status[0] == 0xEF) {
                btn_S1_pos = false;
                //drawString(20, 160, "S1: OFF", color565(0,0,0), color565(255,255,255), 1); 
            }
            if (data_status[0] == 0xF7) {
                btn_S1_pos = true;
                //drawString(20, 160, "S1: ON ", color565(0,0,0), color565(255,255,255), 1); 
            }
            
            if (data[0] == 0x20) {
                if (!btn_E1_pressed) {
                    //drawString(70, 120, "E1: down", color565(0,0,0), color565(255,255,255), 1); 
                    btn_E1_pressed = true;
                    drawMenu(false);
                } else {
                    //drawString(70, 120, "E1: up  ", color565(0,0,0), color565(255,255,255), 1); 
                    btn_E1_released();
                    btn_E1_pressed = false;
                    drawMenu(true);
                }
            }
            if (data[2] == 0x10) {
                if (!btn_E2_pressed) {
                    //drawString(70, 140, "E2: down", color565(0,0,0), color565(255,255,255), 1); 
                    btn_E2_pressed = true;
                    updateFramebuffer();
                } else {
                    //drawString(70, 140, "E2: up  ", color565(0,0,0), color565(255,255,255), 1); 
                    
                    btn_E2_released();
                    drawMenu(true);
                    
                    btn_E2_pressed = false;
                }
            }
	    }

	    i2c3_getn(0x10, data, 2);

	    if (data[0] != qe[0]) {
            
            uart2_byte(qe[0]);
            uart2_str0("-3\n\r");
            //E1_pos = qe[0];
            
            int8_t diff = data[0] - qe[0];
            
            if(parameter_menu_active) {
                parameterSelectionIndex += diff;
                parameterSelectionIndex = LimitRange(parameterSelectionIndex, 0, getCurrentParameterItemCount()-1);

            } else {        
                menuSelectionIndex += diff;
                menuSelectionIndex = LimitRange(menuSelectionIndex, 0, get_current_menu_item_count()-1);
            }
            
            //char encoder1[30] = "000000000";
           // sprintf(encoder1, "E1 (%d): %d", diff, menuSelectionIndex);
            //fillRect(10,180,100,20, color565(255,255,255));
            //drawString(10, 200, encoder1, color565(0,0,0), color565(255,255,255), 1); 
            
            qe[0] = data[0];
            
            drawMenu(false);

            
            //char encoder1[3] = "000";
            //sprintf(encoder1, "E1: %d", qe[0]);
            //drawString(20, 120, encoder1, color565(0,0,0), color565(255,255,255), 1); 
	    }
	    
	    if (data[1] != qe[1]) {
            
            uart2_byte(qe[1]);
            uart2_str0("-4\n\r");
            
           
            int8_t diff = data[1] - qe[1];
            qe[1] = data[1];
            E2_pos = qe[1];
            
            menuItemValues[menuSelectionIndex] += diff;
            drawMenu(false);
            
            //char encoder1[3] = "000";
            //sprintf(encoder1, "E2: %d", qe[1]);
            //drawString(20, 140, encoder1, color565(0,0,0), color565(255,255,255), 1); 
	    }
        /*fillRect(0, 0, _width, _height, menuBackgroundColor);
        drawLine(20, 140, 200, 140, color565(128, 128, 128));
        drawString(20, 140, "This is a long sentence.", color565(0,0,0), color565(255,255,255), 1); */
	    
	    // data[0] = i2c3_get(0x10);
	    // data[1] = i2c3_get(0x11);
        
	    while (!LCD_TE_I);
	    while (LCD_TE_I);

	    LCD_RSX_O = 0;
	    LCD_RSX_O = 1;
        
        displayFramebuffer();

	    /*lcd_pmp_cmd(ILI9341_RAMWR);
        int x;
	    for (x=0; x<320; x++) {
            uint16_t v = r | g | b;
            int y;
            for (y=0; y<240; y++) {
                #ifdef  MCU_16BIT
                    lcd_pmp_wr(v);
                #else
                    lcd_pmp_wr(v >> 8);
                    lcd_pmp_wr(v & 0xFF);
                #endif
                v += (1 << 11) + (1 << 5);
            }

            g = (g + (1 << 7)) & 0x07E0; 
            b = (b + 1) & 0x1F;
	    }

	    r += (1 << 11);
	    g = (g - (1 << 2)) & 0x07E0; 
	    b = (b + 1) & 0x1F;
        */
        
        //Testing 
        
        //drawPixel(200,50, ILI9341_RED);
        
        
        //fillRect(60, 10, 50, 50, ILI9341_BLUE);
        //fillRect(20, 150, 20, 60, ILI9341_BLUE);    
        //fillRect(150, 200, 60, 10, ILI9341_GREEN);
        /*
        drawCircle(100, 200, 40, ILI9341_YELLOW);
        drawCircle(100, 200, 30, ILI9341_YELLOW);
        
        drawCircleHelper(100, 200, 10, 1, ILI9341_YELLOW);
        drawCircleHelper(100, 200, 10, 4, ILI9341_YELLOW);
        
        
        fillCircle(160, 130, 10, ILI9341_YELLOW);
        
        fillCircleHelper(180, 40, 15, 1, 0, ILI9341_RED);
        
        drawRoundRect(20, 70, 30, 25, 4,ILI9341_GREEN);
        
        fillRoundRect(200, 270, 40, 15, 5, ILI9341_WHITE);
        */
        
        /*char count[13] = "Number: 00000";
        sprintf(count, "Number: %d", counter++);
        drawString(20, 20, count, color565(0,0,0), color565(255,255,255), 2);
        
        if (counter > 60000) {
            counter = 0;
        }*/
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

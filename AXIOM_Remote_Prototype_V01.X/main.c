
/*	AXIOM Remote
 **
 **	Copyright (C) 2018 Herbert Poetzl, Sebastian Pichelhofer
 * 
 * code partly based on Adafruit ILI9341 TFT Displays written by Limor "ladyada" Fried for Adafruit Industries.
 **
 **	This program is free software; you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License 2 as published 
 **	by the Free Software Foundation.
 **
 **	Compile with -O6 for best experience
 */

#ifndef MAIN_C
#define MAIN_C

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "definitions.h"
#include "globals.h"
#include "glcdfont.c"// TODO get rid of c file include
#include "gfxfont.h"
//#include "main.h"
//#include "axiom-logo.c"
//#include "home-icon.c"
#include "utility.h"
#include "main.h"
#include "draw.h"
#include "menu.h"

#define DEBUGBUTTONS TRUE


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


// LCD text drawing cursor
//uint16_t _cursor_x = 0;
//uint16_t _cursor_y = 0;


// LCD text drawing font
//GFXfont gfxFont;
//GFXfont _FreeSans9pt7b;
//GFXfont _FreeSans12pt7b;
//GFXfont _FreeSans18pt7b;
//GFXfont _FreeSans24pt7b;

// AXIOM Remote buttons and knobs
//bool btn_E1_pressed = false;
//bool btn_E2_pressed = false;
bool btn_P1_pressed = false;
bool btn_P2_pressed = false;
bool btn_P3_pressed = false;
bool btn_P4_pressed = false;
bool btn_P5_pressed = false;
bool btn_P6_pressed = false;
//bool btn_P7_pressed = false;
//bool btn_P8_pressed = false;
bool btn_P9_pressed = false;
bool btn_P10_pressed = false;
bool btn_P11_pressed = false;
bool btn_P12_pressed = false;
bool btn_S1_pos = false;
bool btn_TS1_pos = false;
bool btn_TS2_pos = false;
uint8_t E1_pos = 0;
uint8_t E2_pos = 0;


// Page related stuff
//_page_id_t _current_page;
//page_t main_page[3];

uint8_t page_count;

static inline
void unlock(void) {
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
}

static inline
void lock(void) {
    SYSKEY = 0x33333333;
}

static inline
void irq_disable(void) {
    asm volatile("di");
    asm volatile("ehb");
}

static inline
void irq_enable(void) {
    asm volatile("ei");
}

void init_pbus(void) {
    unlock();
    PB2DIVbits.PBDIV = 0b000001; // divide by 2
    PB2DIVbits.ON = 1;

    PB7DIVbits.PBDIV = 0b000000; // divide by 1
    PB7DIVbits.ON = 1;
    lock();
}

void init_icsp_w(void) {
    ICSP_W_MCLR_T = 0; // MCLR out
    ICSP_W_MCLR_O = 0; // MCLR low
}

void init_icsp_e(void) {
    ICSP_E_MCLR_T = 0; // MCLR out
    ICSP_E_MCLR_O = 0; // MCLR low
}

void init_i2c_e(void) {
    ICSP_E_PCLK_U = 1;
    ICSP_E_PDAT_U = 1;

    I2C2BRG = 128;

    I2C2ADD = 0xFF;
    I2C2MSK = 0xFF;

    ICSP_E_PDAT_O = 0; // clear SDA
    ICSP_E_PDAT_T = 0; // SDA to out
    ICSP_E_PCLK_O = 1; // set SCL
    ICSP_E_PCLK_T = 1; // SCL to in

    I2C2CONbits.ON = 1;
}

void init_i2c_w(void) {
    ICSP_W_PCLK_U = 1;
    ICSP_W_PDAT_U = 1;

    I2C3ADD = 0xFF;
    I2C3MSK = 0xFF;

    I2C3BRG = 128;

    ICSP_W_PDAT_O = 0; // clear SDA
    ICSP_W_PDAT_T = 0; // SDA to out
    ICSP_W_PCLK_O = 1; // set SCL
    ICSP_W_PCLK_T = 1; // SCL to in

    I2C3CONbits.ON = 1;
}

void init_uart2(void) {
    irq_disable();

    U2MODEbits.ON = 0;

    TRISEbits.TRISE8 = 0; // U2TX out
    TRISEbits.TRISE9 = 1; // U2RX in
    
    // Pull-down
    CNPUEbits.CNPUE9 = 1;
    
    ANSELEbits.ANSE8 = 0; // digital
    ANSELEbits.ANSE9 = 0; // digital

    CFGCONbits.IOLOCK = 0;
    RPE8Rbits.RPE8R = 0b0010; // U2TX
    U2RXRbits.U2RXR = 0b1101; // RPE9
    CFGCONbits.IOLOCK = 1;

    IPC36bits.U2TXIP = 0b000; //! Interrupt priority of 7
    IPC36bits.U2TXIS = 0b00; //! Interrupt sub-priority of 0
    IPC36bits.U2RXIP = 0b111; //! Interrupt priority of 7
    IPC36bits.U2RXIS = 0b00; //! Interrupt sub-priority of 0

    IEC4SET = _IEC4_U2RXIE_MASK; //! Rx INT Enable
    IFS4bits.U2TXIF = 0; //! Clear Tx flag
    IFS4bits.U2RXIF = 0; //! Clear Rx flag

    U2BRG = 24; // 1MBaud @ 50MHz
    U2STA = 0;

    U2MODEbits.BRGH = 1;
    U2MODEbits.PDSEL = 0b00;
    U2MODEbits.STSEL = 0;
    U2MODEbits.UEN = 0b00;
    U2MODEbits.ON = 1;
    U2STASET = 0x9400; // Enable Transmit and Receive

    irq_enable();
}

static inline
uint8_t i2c2_start(void) {
    I2C2CONbits.SEN = 1; // Send Start
    while (I2C2CONbits.SEN);
    return I2C2STAT & 0xFF;
}

static inline
uint8_t i2c3_start(void) {
    I2C3CONbits.SEN = 1; // Send Start
    while (I2C3CONbits.SEN);
    return I2C3STAT & 0xFF;
}

static inline
uint8_t i2c2_restart(void) {
    I2C2CONbits.RSEN = 1; // Send Restart
    while (I2C2CONbits.RSEN);
    return I2C2STAT & 0xFF;
}

static inline
uint8_t i2c3_restart(void) {
    I2C3CONbits.RSEN = 1; // Send Restart
    while (I2C3CONbits.RSEN);
    return I2C3STAT & 0xFF;
}


#if	0

static inline
uint8_t i2c2_stop(void) {
    if ((I2C2CON & 0x1F) == 0)
        I2C2CONbits.PEN = 1; // Send Stop
    return I2C2CON & 0x1F;
}
#else
#define i2c2_stop i2c2_bb_stop
#endif


#if	0

static inline
uint8_t i2c3_stop(void) {
    if ((I2C3CON & 0x1F) == 0)
        I2C3CONbits.PEN = 1; // Send Stop
    return I2C3CON & 0x1F;
}
#else
#define i2c3_stop i2c3_bb_stop
#endif

static inline
void i2c_bb_delay(unsigned cnt) {
    unsigned i;
    while (cnt--)
        for (i = 0; i < 200; i++);
}

static inline
void i2c2_bb_stop(void) {
    i2c_bb_delay(1);
    I2C2CONbits.ON = 0; // disable I2C
    i2c_bb_delay(1);
    ICSP_E_PDAT_T = 1; // SDA to input
    i2c_bb_delay(2);
    I2C2CONbits.ON = 1; // enable I2C
    ICSP_E_PDAT_O = 0; // clear SDA
    ICSP_E_PDAT_T = 0; // SDA to out
    ICSP_E_PCLK_O = 1; // set SCL
    ICSP_E_PCLK_T = 1; // SCL to in
}

static inline
void i2c3_bb_stop(void) {
    i2c_bb_delay(1);
    I2C3CONbits.ON = 0; // disable I2C
    i2c_bb_delay(1);
    ICSP_W_PDAT_T = 1; // SDA to input
    i2c_bb_delay(2);
    I2C3CONbits.ON = 1; // enable I2C
    ICSP_W_PDAT_O = 0; // clear SDA
    ICSP_W_PDAT_T = 0; // SDA to out
    ICSP_W_PCLK_O = 1; // set SCL
    ICSP_W_PCLK_T = 1; // SCL to in
}

static inline
bool i2c2_write(uint8_t byte) {
    I2C2TRN = byte;
    while (I2C2STATbits.TRSTAT);
    return I2C2STATbits.ACKSTAT;
}

static inline
bool i2c3_write(uint8_t byte) {
    I2C3TRN = byte;
    while (I2C3STATbits.TRSTAT);
    return I2C3STATbits.ACKSTAT;
}

static inline
uint8_t i2c2_read(bool ackdt) {
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
uint8_t i2c3_read(bool ackdt) {
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
void set_mclr_w(unsigned val) {
    ICSP_W_MCLR_O = (val & 1) ? 1 : 0;
}

static inline
void set_mclr_e(unsigned val) {
    ICSP_E_MCLR_O = (val & 1) ? 1 : 0;
}

static inline
void uart2_ch(char ch) {
    while (U2STAbits.UTXBF);
    U2TXREG = ch;
}

static inline
void uart2_hex(uint8_t hex) {
    hex &= 0xF;
    if (hex > 9)
        uart2_ch(hex + 'A' - 10);
    else
        uart2_ch(hex + '0');
}

static inline
void uart2_byte(uint8_t val) {
    uart2_hex(val >> 4);
    uart2_hex(val);
}

static inline
void uart2_word(uint16_t val) {
    uart2_byte(val >> 8);
    uart2_byte(val);
}

static inline
void uart2_long(uint32_t val) {
    uart2_word(val >> 16);
    uart2_word(val);
}

static inline
void uart2_str0(const char *str) {
    while (*str)
        uart2_ch(*str++);
}

void debug_uart(char *str) {
    while (*str) {
        uart2_ch(*str++);
    }
    uart2_str0("\n");
    uart2_str0("\r");
}

static inline
void delay_us(unsigned cnt) {
    while (cnt--) {
        unsigned i;
        for (i = 0; i < 49; i++)
            asm("nop");
    }
}

void __attribute__((vector(_UART2_RX_VECTOR), interrupt(IPL7SRS), nomips16)) uart2_rx_isr(void) {
    /*while (U2STAbits.URXDA) { // process buffer 
        char ch = U2RXREG;

        uart2_ch(ch); // echo back
    }*/

    IFS4CLR = _IFS4_U2RXIF_MASK; // clear UART2 Rx IRQ
}

void __attribute__((vector(_UART2_TX_VECTOR), interrupt(IPL7SRS), nomips16)) uart2_tx_isr(void) {

    IFS4CLR = _IFS4_U2TXIF_MASK; // clear UART2 TX IRQ
}

uint8_t i2c2_get(uint8_t idx) {
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

void i2c2_set(uint8_t idx, uint8_t val) {
    i2c2_start();
    i2c2_write(0x20);
    i2c2_write(idx);
    i2c2_write(val);
    i2c2_stop();
}

void i2c2_getn(uint8_t idx, uint8_t *data, uint8_t n) {
    i2c2_start();
    i2c2_write(0x20);
    i2c2_write(idx);
    i2c2_restart();
    i2c2_write(0x21);
    while (n--)
        *data++ = i2c2_read(n == 0);
    i2c2_stop();
}

void i2c2_setn(uint8_t idx, uint8_t *data, uint8_t n) {
    i2c2_start();
    i2c2_write(0x20);
    i2c2_write(idx);
    while (n--)
        i2c2_write(*data++);
    i2c2_stop();
}

uint8_t i2c3_get(uint8_t idx) {
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

void i2c3_set(uint8_t idx, uint8_t val) {
    i2c3_start();
    i2c3_write(0x20);
    i2c3_write(idx);
    i2c3_write(val);
    i2c3_stop();
}

void i2c3_getn(uint8_t idx, uint8_t *data, uint8_t n) {
    i2c3_start();
    i2c3_write(0x20);
    i2c3_write(idx);
    i2c3_restart();
    i2c3_write(0x21);
    while (n--)
        *data++ = i2c3_read(n == 0);
    i2c3_stop();
}

void i2c3_setn(uint8_t idx, uint8_t *data, uint8_t n) {
    i2c3_start();
    i2c3_write(0x20);
    i2c3_write(idx);
    while (n--)
        i2c3_write(*data++);
    i2c3_stop();
}

uint8_t bitidx(uint8_t val) {
    uint8_t i;

    for (i = 0; i < 8; i++)
        if (val & 1)
            return i;
        else
            val >>= 1;
    return 0xF;
}

static inline
void lcd_pmp_wr(unsigned data) {
    while (PMMODEbits.BUSY);
    PMDIN = data;
}

static inline
unsigned lcd_pmp_rd(void) {
    while (PMMODEbits.BUSY);
    return PMDIN;
}

static inline
unsigned lcd_pmp_rdf(void) {
    PMCONbits.PTRDEN = 0;
    unsigned res = lcd_pmp_rd();
    PMCONbits.PTRDEN = 1;
    return res;
}

static inline
void lcd_pmp_cmd(uint8_t cmd) {
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

    uint32_t xa = ((uint32_t) _x << 16) | (_x + h - 1);
    uint32_t ya = ((uint32_t) _y << 16) | (_y + w - 1);

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

void setLCDBacklight(uint8_t brightness) {
    //doesnt work, extended command not supported by LCD driver?

    lcd_pmp_cmd(0x51);
    lcd_pmp_wr(brightness);
}

void draw_lcd() {
    if (current_menu != MENU_NONE) {
        draw_menu();
    } else if (current_page == PAGE_HOME) {
        draw_page();
    } else if (current_page == PAGE_WB) {
        draw_wb_page();
    } else if (current_page == PAGE_WB_HELP) {
        draw_wb_help_page();
    }
}

void clearframebuffer(uint16_t color) {
    uint16_t x;
    uint8_t y;
    for (x = 0; x < FRAMEBUFFER_WIDTH; x++) {
        for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
            framebuffer[x][y] = color;
        }
    }
}

void clear_transitionframebuffer(uint16_t color) {
    uint16_t x;
    uint8_t y;
    for (x = 0; x < FRAMEBUFFER_WIDTH; x++) {
        for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
            transition_framebuffer[x][y] = color;
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

    // transition animations
    if (transition_active) {
        if (transition_counter <= transition_animation_speed - 1) {
            transition_active = false;
        }

        if (transition_animation_type == TRANSITION_WIPE_RIGHT) {

            float ratio = fabs(((float) (transition_counter) / 255) - 1);

            for (x = 0; x < FRAMEBUFFER_WIDTH; x++) {
                float horizontal_progress = (float) (x) / (float) (FRAMEBUFFER_WIDTH);

                if (horizontal_progress < ratio) {
                    for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
                        lcd_pmp_wr(framebuffer[x][y]);
                    }
                } else {
                    for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
                        lcd_pmp_wr(transition_framebuffer[x][y]);
                    }
                }
            }
        }
        if (transition_animation_type == TRANSITION_WIPE_LEFT) {

            float ratio = ((float) (transition_counter) / 255);

            for (x = 0; x < FRAMEBUFFER_WIDTH; x++) {
                float horizontal_progress = (float) (x) / (float) (FRAMEBUFFER_WIDTH);

                if (horizontal_progress > ratio) {
                    for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
                        lcd_pmp_wr(framebuffer[x][y]);
                    }
                } else {
                    for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
                        lcd_pmp_wr(transition_framebuffer[x][y]);
                    }
                }
            }
        }
        if (transition_animation_type == TRANSITION_PUSH_LEFT) {

            uint16_t offset = (float) (transition_counter) / (float) (255) * FRAMEBUFFER_WIDTH;

            for (x = 0; x < FRAMEBUFFER_WIDTH; x++) {
                if (x <= offset) {
                    for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
                        lcd_pmp_wr(transition_framebuffer[x + (FRAMEBUFFER_WIDTH - offset)][y]);
                    }
                } else {
                    for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
                        lcd_pmp_wr(framebuffer[x - offset][y]);
                    }
                }
            }
        }
        if (transition_animation_type == TRANSITION_PUSH_RIGHT) {

            uint16_t offset = fabs((float) (transition_counter) / (float) (255) - 1) * FRAMEBUFFER_WIDTH;

            for (x = 0; x < FRAMEBUFFER_WIDTH; x++) {
                //uint16_t horizontal_progress = (float) (x) / (float) (_width);

                if (x > offset) {
                    //if (((x - offset) < _width) && ((x - offset) >= 0)) {
                    for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
                        lcd_pmp_wr(transition_framebuffer[x - offset][y]);
                    }
                    //}
                } else {
                    // if ((((x + (_width - offset)) >= 0) && (x + (_width - offset)) < _width)) {
                    for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
                        lcd_pmp_wr(framebuffer[x + (FRAMEBUFFER_WIDTH - offset)][y]);
                    }
                    //}
                }
            }
        }
        if (transition_animation_type == TRANSITION_PUSH_UP) {

            uint16_t offset = (float) (transition_counter) / (float) (255) * FRAMEBUFFER_HEIGHT;

            for (x = 0; x < FRAMEBUFFER_WIDTH; x++) {
                for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
                    if (y <= offset) {
                        lcd_pmp_wr(transition_framebuffer[x][y + (FRAMEBUFFER_HEIGHT - offset)]);
                    } else {
                        lcd_pmp_wr(framebuffer[x][y - offset]);
                    }
                }
            }
        }
        if (transition_animation_type == TRANSITION_PUSH_DOWN) {
            uint16_t offset = fabs(((float) (transition_counter) / (float) (255) - 1) * FRAMEBUFFER_HEIGHT);

            for (x = 0; x < FRAMEBUFFER_WIDTH; x++) {
                for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
                    if (y > offset) {
                        lcd_pmp_wr(transition_framebuffer[x][y - offset]);
                    } else {
                        lcd_pmp_wr(framebuffer[x][y + (FRAMEBUFFER_HEIGHT - offset)]);
                    }
                }
            }
        }

        transition_counter -= transition_animation_speed;
    } else {
        // no transition animation
        for (x = 0; x < FRAMEBUFFER_WIDTH; x++) {
            for (y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
                //for (y=_height; y>0; y--) { // we flip y axis so the origin in in the lower left corner

                lcd_pmp_wr(framebuffer[x][y]);
            }
        }
    }
}

void startframebuffer_transition(enum transition_animation transition_animation_type, uint8_t speed) {
    //copy the current content of the framebuffer to the _transitionframebuffer - we take a snapshot so to say

    memcpy(transition_framebuffer, framebuffer, FRAMEBUFFER_HEIGHT * FRAMEBUFFER_WIDTH * sizeof (uint16_t));

    transition_active = true;
    transition_counter = 255;
    transition_animation_speed = speed;
    transition_animation_type = transition_animation_type;
}

void init_WB() {

    uart2_str0("\n\rWB Init ... ");
    uint8_t i = 0;

    strcpy(white_balance_parameter.white_balance_options[i].label, "Candles");
    white_balance_parameter.white_balance_options[i].Kelvin = 2000;
    white_balance_parameter.white_balance_options[i].ColorShift = 0;
    i++;
    strcpy(white_balance_parameter.white_balance_options[i].label, "Tungsten");
    white_balance_parameter.white_balance_options[i].Kelvin = 3200;
    white_balance_parameter.white_balance_options[i].ColorShift = 0;
    i++;
    strcpy(white_balance_parameter.white_balance_options[i].label, "Fluorescent");
    white_balance_parameter.white_balance_options[i].Kelvin = 4300;
    white_balance_parameter.white_balance_options[i].ColorShift = 0;
    i++;
    strcpy(white_balance_parameter.white_balance_options[i].label, "Daylight");
    white_balance_parameter.white_balance_options[i].Kelvin = 5600;
    white_balance_parameter.white_balance_options[i].ColorShift = 0;
    i++;
    strcpy(white_balance_parameter.white_balance_options[i].label, "Cloudy");
    white_balance_parameter.white_balance_options[i].Kelvin = 6500;
    white_balance_parameter.white_balance_options[i].ColorShift = 0;
    i++;
    strcpy(white_balance_parameter.white_balance_options[i].label, "Shade");
    white_balance_parameter.white_balance_options[i].Kelvin = 7500;
    white_balance_parameter.white_balance_options[i].ColorShift = 0;

    white_balance_parameter.white_balance_options_count = i + 1;
    white_balance_parameter.white_balance_selection_index = 0;
    uart2_str0("\n\rWB Init done ... ");
}

void init_lcd() {
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
    lcd_pmp_rd(); // previous
    res = lcd_pmp_rd(); // 1p dummy
    res = (res << 8) | lcd_pmp_rd(); // 2p [31:24]
    res = (res << 8) | lcd_pmp_rd(); // 3p [23:16]
    res = (res << 8) | lcd_pmp_rd(); // 4p [15:8]
    res = (res << 8) | lcd_pmp_rdf(); // 5p [7:0]

    uart2_long(res);

    uart2_str0("\n\rDisplay ON ... ");

    lcd_pmp_cmd(0x29);

    uart2_str0("\n\rRead Display Status ... ");

    lcd_pmp_cmd(ILI9341_RDDST);
    lcd_pmp_rd(); // previous
    res = lcd_pmp_rd(); // 1p dummy
    res = (res << 8) | lcd_pmp_rd(); // 2p [31:24]
    res = (res << 8) | lcd_pmp_rd(); // 3p [23:16]
    res = (res << 8) | lcd_pmp_rd(); // 4p [15:8]
    res = (res << 8) | lcd_pmp_rdf(); // 5p [7:0]

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

    lcd_pmp_cmd(0xF2); // 3Gamma Function Disable
    lcd_pmp_wr(0x00);

    lcd_pmp_cmd(ILI9341_GAMMASET); //Gamma curve selected
    lcd_pmp_wr(0x01);

    lcd_pmp_wr(ILI9341_GMCTRP1); //Set Gamma
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

    lcd_pmp_wr(ILI9341_GMCTRN1); //Set Gamma
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

    //gfxFont = FreeSans9pt7b;

    // Clear the image
    clearframebuffer(ILI9341_WHITE);

    // Clear the transition_frame buffer
    clear_transitionframebuffer(ILI9341_WHITE);
}

/*void btn_E1_released() {
    uint8_t a;
    for (a = 0; a < _main_menu_count; a++) {
        if (_main_menu[a].menu_id == _current_menu) {

            // if this menu item is disabled don't do anything
            if (_main_menu[a].menu_item[_menu_selection_index].disabled) {
                return;
            }

            // is the current item linking into a SUBMENU?
            if (_main_menu[a].menu_item[_menu_selection_index].type == SUBMENU) {
                // navigate into SUBMENU
                _current_menu = _main_menu[a].menu_item[_menu_selection_index].link_to_SUBMENU;

                // reset cursor to first item in list;
                _menu_selection_index = 0;

                //update bread crumbs
                strcpy(menu_breadcrumbs, "Menu > ");
                strcat(menu_breadcrumbs, _main_menu[a].menu_item[_menu_selection_index].label);
                return;
            }

            // is the current item supposed to show a drop-down menu?
            if ((_main_menu[a].menu_item[_menu_selection_index].type == DROPDOWN) && (_parameter_menu_active == 0)) {
                //open parameter menu
                _parameter_menu_active = _menu_selection_index;
                return;
            }

            // are we in a drop-down menu currently?
            if ((_main_menu[a].menu_item[_menu_selection_index].type == DROPDOWN) && (_parameter_menu_active != 0)) {
                // set new value
                _main_menu[a].menu_item[_menu_selection_index].value = _parameter_selection_index;

                //close parameter menu
                _parameter_menu_active = 0;
            }
            /*
                    if ((menuSelectionIndex == 2) && (_parameter_menu_active == 0)) {
                        _parameter_menu_active = 2;
                    } else if ((menuSelectionIndex == 2) && (_parameter_menu_active == 2)) {
                        mainMenuItem2 = parameterSelectionIndex;
                        _parameter_menu_active = 0;
                    }
                    if ((menuSelectionIndex == 5) && (_parameter_menu_active == 0)) {
                        _parameter_menu_active = 5;
                    } else if ((menuSelectionIndex == 5) && (_parameter_menu_active == 5)) {
                        mainMenuItem3 = parameterSelectionIndex;
                        _parameter_menu_active = 0;
                    }*/

/*} else if (_current_menu == SUBMENU1) {
    if (_menu_selection_index == 0) {
        _current_menu = Main;
        _menu_selection_index = 0;
        strcpy(menu_breadcrumbs, "Menu");
    }
} else if (_current_menu == SUBMENU2) {
    if (_menu_selection_index == 0) {
        _current_menu = Main;
        _menu_selection_index = 1;
        strcpy(menu_breadcrumbs, "Menu");
    }
}
}
}
}
 */

void btn_E2_released() {

}

//void draw_menu_item (uint16_t x, uint16_t y, char* label, char* value, bool selected, bool highlighted){

void updateFramebuffer() {

    draw_lcd();
}

void knob_event_handler(ButtonID button_event, int8_t value) {
    if (current_page == PAGE_WB) {
        wb_page_knob_handler(button_event, value);
    }
    if (current_page == PAGE_WB_HELP) {
        wb_help_page_knob_handler(button_event, value);
    }
    if (current_menu != MENU_NONE) {
        main_menu_knob_handler(button_event, value);
    }
    draw_lcd();
}

void button_event_handler(ButtonID button_event, bool pressed) {
    if (current_page == PAGE_HOME) {
        if (pressed) {
            main_page_button_press_handler(button_event);
        } else {
            main_page_button_release_handler(button_event);
        }
    } else if (current_page == PAGE_WB) {
        if (pressed) {
            wb_page_button_press_handler(button_event);
        } else {
            wb_page_button_release_handler(button_event);
        }
    } else if (current_page == PAGE_WB_HELP) {
        if (pressed) {
            wb_help_page_button_press_handler(button_event);
        } else {
            wb_help_page_button_release_handler(button_event);
        }
    } else if (current_menu != MENU_NONE) {
        if (pressed) {
            main_menu_button_press_handler(button_event);
        } else {

            main_menu_button_release_handler(button_event);
        }
    }

    draw_lcd();
}

int main(void) {
    static uint8_t rgb[4];

    INTCONbits.MVEC = 1;            // Multi Vector Interrupts
    PRISSbits.SS0 = 0;              // Normal Register Set
    PRISSbits.PRI7SS = 7;           // Assign Shadow Register Set

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


    init_lcd();

    init_WB();

    init_menus();
    init_pages();

    //subpages
    init_wb_page();
    init_wb_help_page();

    // start navigation
    current_menu = MENU_NONE;
    current_page = PAGE_HOME;

    static uint16_t r = 0;
    static uint16_t g = 0;
    static uint16_t b = 0;

    static uint8_t data[16];
    static uint8_t data_status[16];
    static uint8_t qe[2];

    draw_lcd();
    uart2_str0("\n\rfirst LCD draw done ... ");

    while (1) {

        // Read Button Press Messages

        /*
         *  the first three registers (i2c3_getn(0x00, data, 3); data[0], data[1], data[2]) provide change information
         *  i.e. a bit set there shows that something on the respective input (port A,B or C) changed
         *  so, a button pressed or a button released will trigger a bit change there
         * 
         *  register 4 to 6 (i2c2_getn(0x04, data, 3) data[0], data[1], data[2]) contain the status registers of each button/knobs current state
         */


        // each PIC8 handles a part of the buttons/switches
        i2c2_getn(0x00, data, 3);
        i2c2_getn(0x04, data_status, 3);

        if (data[0] || data[1] || data[2]) {
#ifdef DEBUGBUTTONS
            uart2_str0("change: ");
            uart2_byte(data[0]);
            uart2_byte(data[1]);
            uart2_byte(data[2]);
            uart2_str0("\n\r");

            uart2_str0("status: ");
            uart2_byte(data_status[0]);
            uart2_byte(data_status[1]);
            uart2_byte(data_status[2]);
            uart2_str0("\n\r");
#endif

            if (data[1] & 0x08) {
                if (data_status[1] & 0x08) {
                    button_event_handler(P1, false);
                    //btn_P1_pressed = false;
#ifdef DEBUGBUTTONS
                    uart2_str0("P1 up\n\r");
#endif
                } else {
                    button_event_handler(P1, true);
#ifdef DEBUGBUTTONS
                    uart2_str0("P1 down\n\r");
#endif
                    //btn_P1_pressed = true;
                }
            }

            if (data[1] & 0x10) {
                if (data_status[1] & 0x10) {
#ifdef DEBUGBUTTONS
                    uart2_str0("P2 up\n\r");
#endif
                    button_event_handler(P2, false);
                } else {
#ifdef DEBUGBUTTONS
                    uart2_str0("P2 down\n\r");
#endif
                    button_event_handler(P2, true);
                }
            }

            if (data[1] & 0x20) {
                if (data_status[1] & 0x20) {
#ifdef DEBUGBUTTONS
                    uart2_str0("P3 up\n\r");
#endif
                    button_event_handler(P3, false);
                } else {
#ifdef DEBUGBUTTONS
                    uart2_str0("P3 down\n\r");
#endif
                    button_event_handler(P3, true);
                }
            }

            if (data[2] & 0x80) {
                if (data_status[2] & 0x80) {
                    button_event_handler(P4, false);
#ifdef DEBUGBUTTONS
                    uart2_str0("P4 up\n\r");
#endif
                } else {
                    button_event_handler(P4, true);
#ifdef DEBUGBUTTONS
                    uart2_str0("P4 down\n\r");
#endif
                }
            }

            if (data[2] & 0x40) {
                if (data_status[2] & 0x40) {
#ifdef DEBUGBUTTONS
                    uart2_str0("change: ");
                    uart2_byte(data[0]);
                    uart2_byte(data[1]);
                    uart2_byte(data[2]);
                    uart2_str0("\n\r");

                    uart2_str0("status: ");
                    uart2_byte(data_status[0]);
                    uart2_byte(data_status[1]);
                    uart2_byte(data_status[2]);
                    uart2_str0("\n\r");
                    uart2_str0("P5 up\n\r");
#endif
                    button_event_handler(P5, false);
                } else {
#ifdef DEBUGBUTTONS
                    uart2_str0("P5 down\n\r");
#endif
                    button_event_handler(P5, true);
                }
            }

            if (data[2] & 0x20) {
                if (data_status[2] & 0x20) {
#ifdef DEBUGBUTTONS
                    uart2_str0("P6 up\n\r");
#endif
                    button_event_handler(P6, false);
                } else {
#ifdef DEBUGBUTTONS
                    uart2_str0("P6 down\n\r");
#endif
                    button_event_handler(P6, true);
                }
            }

            if (data[2] & 0x10) {
                if (data_status[2] & 0x10) {
#ifdef DEBUGBUTTONS
                    uart2_str0("P7 up\n\r");
#endif
                    button_event_handler(P7, false);
                } else {
#ifdef DEBUGBUTTONS
                    uart2_str0("P7 down\n\r");
#endif
                    button_event_handler(P7, true);
                }
            }

            if (data[2] & 0x08) {
                if (data_status[2] & 0x08) {
#ifdef DEBUGBUTTONS
                    uart2_str0("P8 up\n\r");
#endif
                    button_event_handler(P8, false);
                } else {
#ifdef DEBUGBUTTONS
                    uart2_str0("P8 down\n\r");
#endif
                    button_event_handler(P8, true);
                }
            }

            if (data[2] & 0x04) {
                if (data_status[2] & 0x04) {
#ifdef DEBUGBUTTONS
                    uart2_str0("P9 up\n\r");
#endif
                    button_event_handler(P9, false);
                } else {
#ifdef DEBUGBUTTONS
                    uart2_str0("P9 down\n\r");
#endif
                    button_event_handler(P9, true);
                }
            }

            if (data[1] & 0x04) {
                if (data_status[1] & 0x04) {
#ifdef DEBUGBUTTONS
                    uart2_str0("P10 up\n\r");
#endif
                    button_event_handler(P10, false);
                } else {
#ifdef DEBUGBUTTONS
                    uart2_str0("P10 down\n\r");
#endif
                    button_event_handler(P10, true);
                }
            }

            if (data[1] & 0x02) {
                if (data_status[1] & 0x02) {
#ifdef DEBUGBUTTONS
                    uart2_str0("P11 up\n\r");
#endif
                    button_event_handler(P11, false);
                } else {
#ifdef DEBUGBUTTONS
                    uart2_str0("P11 down\n\r");
#endif
                    button_event_handler(P11, true);
                }
            }

            if (data[1] & 0x01) {
                if (data_status[1] & 0x01) {
#ifdef DEBUGBUTTONS
                    uart2_str0("P12 up\n\r");
#endif
                    button_event_handler(P12, false);
                } else {
#ifdef DEBUGBUTTONS
                    uart2_str0("P12 down\n\r");
#endif
                    button_event_handler(P12, true);
                }
            }
        }

        // each PIC8 handles a part of the buttons/switches
        i2c3_getn(0x00, data, 3);
        i2c3_getn(0x04, data_status, 3);

        if ((data[0] & 0x3F) || data[1] || (data[2] & 0x1F)) {
#ifdef DEBUGBUTTONS
            uart2_str0("change: ");
            uart2_byte(data[0]);
            uart2_byte(data[1]);
            uart2_byte(data[2]);
            uart2_str0("\n\r");

            uart2_str0("status: ");
            uart2_byte(data_status[0]);
            uart2_byte(data_status[1]);
            uart2_byte(data_status[2]);
            uart2_str0("\n\r");
#endif

            if (data[1] == 0x04) {
                //btn_TS1_pos = true;
                //drawString(20, 200, "TS1: up  ", color565(0,0,0), color565(255,255,255), 1); 
            }
            if (data[1] == 0x08) {
                //btn_TS1_pos = false;
                //drawString(20, 200, "TS1: down", color565(0,0,0), color565(255,255,255), 1); 
            }
            if (data[2] == 0x04) {
                //btn_TS2_pos = true;

                //drawString(20, 180, "TS2: up  ", color565(0, 0, 0), color565(255, 255, 255), 1, ALIGN_LEFT, 0);
            }
            if (data[2] == 0x08) {
                //btn_TS2_pos = false;
                //drawString(20, 180, "TS2: down", color565(0, 0, 0), color565(255, 255, 255), 1, ALIGN_LEFT, 0);
            }
            if (data_status[0] == 0xEF) {
                //btn_S1_pos = false;
                //drawString(20, 160, "S1: OFF", color565(0,0,0), color565(255,255,255), 1); 
            }
            if (data_status[0] == 0xF7) {
                //btn_S1_pos = true;
                //drawString(20, 160, "S1: ON ", color565(0,0,0), color565(255,255,255), 1); 
            }

            if (data[0] & 0x20) {
                if (data_status[0] & 0x20) {
                    button_event_handler(E1, false);
#ifdef DEBUGBUTTONS
                    uart2_str0("E1 up\n\r");
#endif
                } else {
                    button_event_handler(E1, true);
#ifdef DEBUGBUTTONS
                    uart2_str0("E1 down\n\r");
#endif
                }
            }
            if (data[2] & 0x10) {
                if (data_status[2] & 0x10) {
                    button_event_handler(E2, false);
#ifdef DEBUGBUTTONS
                    uart2_str0("E2 up\n\r");
#endif

                } else {
                    button_event_handler(E2, true);
#ifdef DEBUGBUTTONS
                    uart2_str0("E2 down\n\r");
#endif
                }
            }
        }

        i2c3_getn(0x10, data, 2);

        if (data[0] != qe[0]) {

            uart2_byte(qe[0]);
            uart2_str0("-3\n\r");
            //E1_pos = qe[0];

            int8_t diff = data[0] - qe[0];
            knob_event_handler(E1_ROT, diff);

            /*else {
                _menu_selection_index += diff;
                _menu_selection_index = limit_range(_menu_selection_index, 0, get_current_menu_item_count() - 1);
            }*/

            //char encoder1[30] = "000000000";
            // sprintf(encoder1, "E1 (%d): %d", diff, menuSelectionIndex);
            //fillRect(10,180,100,20, color565(255,255,255));
            //drawString(10, 200, encoder1, color565(0,0,0), color565(255,255,255), 1); 

            qe[0] = data[0];

            //draw_lcd();


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

            //menuItemValues[_menu_selection_index] += diff;
            draw_lcd();

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

        //uart2_str0("\n\rfirst loop done ... ");
    }

    PMADDR = 0x8000;

    uart2_str0("\n\rall done.");

    while (1);

    return 0;
}

#endif /* MAIN_C */
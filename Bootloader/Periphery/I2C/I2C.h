#ifndef I2C_H
#define	I2C_H

#include "I2CDefinitions.h"

// TODO: Review #if/#endif part, if still required or can be reworked
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

#endif	/* I2C_H */

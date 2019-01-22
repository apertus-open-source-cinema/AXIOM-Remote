/*	AXIOM Remote
 **
 **	Copyright (C) 2018 Herbert Poetzl, Sebastian Pichelhofer
 * 
 **
 **	This program is free software; you can redistribute it and/or modify
 **    	it under the terms of the GNU General Public License 2 as published 
 **	by the Free Software Foundation.
 **
 **	Compile with -O6 for best experience
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <xc.h>

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

#define ILI9341_TFTWIDTH   240       ///< ILI9341 max TFT width
#define ILI9341_TFTHEIGHT  320       ///< ILI9341 max TFT height

#define ILI9341_NOP        0x00      ///< No-op register
#define ILI9341_SWRESET    0x01      ///< Software reset register
#define ILI9341_RDDID      0x04      ///< Read display identification information
#define ILI9341_RDDST      0x09      ///< Read Display Status 

#define ILI9341_SLPIN      0x10      ///< Enter Sleep Mode
#define ILI9341_SLPOUT     0x11      ///< Sleep Out
#define ILI9341_PTLON      0x12      ///< Partial Mode ON
#define ILI9341_NORON      0x13      ///< Normal Display Mode ON

#define ILI9341_RDMODE     0x0A      ///< Read Display Power Mode 
#define ILI9341_RDMADCTL   0x0B      ///< Read Display MADCTL
#define ILI9341_RDPIXFMT   0x0C      ///< Read Display Pixel Format
#define ILI9341_RDIMGFMT   0x0D      ///< Read Display Image Format 
#define ILI9341_RDSELFDIAG 0x0F      ///< Read Display Self-Diagnostic Result

#define ILI9341_INVOFF     0x20      ///< Display Inversion OFF
#define ILI9341_INVON      0x21      ///< Display Inversion ON 
#define ILI9341_GAMMASET   0x26      ///< Gamma Set 
#define ILI9341_DISPOFF    0x28      ///< Display OFF 
#define ILI9341_DISPON     0x29      ///< Display ON

#define ILI9341_CASET      0x2A      ///< Column Address Set 
#define ILI9341_PASET      0x2B      ///< Page Address Set
#define ILI9341_RAMWR      0x2C      ///< Memory Write
#define ILI9341_RAMRD      0x2E      ///< Memory Read

#define ILI9341_PTLAR      0x30      ///< Partial Area
#define ILI9341_MADCTL     0x36      ///< Memory Access Control
#define ILI9341_VSCRSADD   0x37      ///< Vertical Scrolling Start Address
#define ILI9341_PIXFMT     0x3A      ///< COLMOD: Pixel Format Set

#define ILI9341_FRMCTR1    0xB1      ///< Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR2    0xB2      ///< Frame Rate Control (In Idle Mode/8 colors)
#define ILI9341_FRMCTR3    0xB3      ///< Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341_INVCTR     0xB4      ///< Display Inversion Control
#define ILI9341_DFUNCTR    0xB6      ///< Display Function Control

#define ILI9341_PWCTR1     0xC0      ///< Power Control 1
#define ILI9341_PWCTR2     0xC1      ///< Power Control 2
#define ILI9341_PWCTR3     0xC2      ///< Power Control 3
#define ILI9341_PWCTR4     0xC3      ///< Power Control 4
#define ILI9341_PWCTR5     0xC4      ///< Power Control 5
#define ILI9341_VMCTR1     0xC5      ///< VCOM Control 1
#define ILI9341_VMCTR2     0xC7      ///< VCOM Control 2

#define ILI9341_RDID1      0xDA      ///< Read ID 1
#define ILI9341_RDID2      0xDB      ///< Read ID 2
#define ILI9341_RDID3      0xDC      ///< Read ID 3
#define ILI9341_RDID4      0xDD      ///< Read ID 4

#define ILI9341_GMCTRP1    0xE0      ///< Positive Gamma Correction
#define ILI9341_GMCTRN1    0xE1      ///< Negative Gamma Correction

#define ILI9341_BCKLGHT    0xBE      ///< Set Backlight Intensity
//#define ILI9341_PWCTR6     0xFC


// Color definitions
#define ILI9341_BLACK       0x0000      ///<   0,   0,   0
#define ILI9341_NAVY        0x000F      ///<   0,   0, 128
#define ILI9341_DARKGREEN   0x03E0      ///<   0, 128,   0
#define ILI9341_DARKCYAN    0x03EF      ///<   0, 128, 128
#define ILI9341_MAROON      0x7800      ///< 128,   0,   0
#define ILI9341_PURPLE      0x780F      ///< 128,   0, 128
#define ILI9341_OLIVE       0x7BE0      ///< 128, 128,   0
#define ILI9341_LIGHTGREY   0xC618      ///< 192, 192, 192
#define ILI9341_DARKGREY    0x7BEF      ///< 128, 128, 128
#define ILI9341_BLUE        0x001F      ///<   0,   0, 255
#define ILI9341_GREEN       0x07E0      ///<   0, 255,   0
#define ILI9341_CYAN        0x07FF      ///<   0, 255, 255
#define ILI9341_RED         0xF800      ///< 255,   0,   0
#define ILI9341_MAGENTA     0xF81F      ///< 255,   0, 255
#define ILI9341_YELLOW      0xFFE0      ///< 255, 255,   0
#define ILI9341_WHITE       0xFFFF      ///< 255, 255, 255
#define ILI9341_ORANGE      0xFD20      ///< 255, 165,   0
#define ILI9341_GREENYELLOW 0xAFE5      ///< 173, 255,  47
#define ILI9341_PINK        0xFC18      ///< 255, 128, 192

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

#define lcd_pmp_wr16(s)         lcd_pmp_wr((s) >> 8); lcd_pmp_wr(s)
#define lcd_pmp_wr32(l)         lcd_pmp_wr((l) >> 24); lcd_pmp_wr((l) >> 16); lcd_pmp_wr((l) >> 8); lcd_pmp_wr(l)

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_16bit(addr) (*(uint16_t)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))

#define array_len(a)    (sizeof(a)/sizeof(a[0]))

#define framebuffer_width 320
#define _height 240
#define _right 319
#define _top 239

#define	delay_ms(t) delay_us(1000*(t))

#endif /* DEFINITIONS_H */

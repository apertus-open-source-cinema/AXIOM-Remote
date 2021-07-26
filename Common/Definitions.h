#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define XTAL_FREQ 24000000      // 24MHz MEMS oscillator
#define CPU_FREQ 192000000      // 192MHz, see DEVCFG2 section for explanations
#define PBCLK_FREQ CPU_FREQ / 2 // 96MHz, division by 2 is configured

#endif /* DEFINITIONS_H */

#ifndef TEST_2_BIT_LINE_OUTPUT_H
#define TEST_2_BIT_LINE_OUTPUT_H

#include <stdint.h>
#include <array>
#include <map>
#include "../../Firmware/Utils.h"

constexpr uint16_t Test2BitBGColor = 0x22;
constexpr uint16_t Test2BitFGColor = 0x111;
constexpr uint16_t Test2BitTransparentColor = 0x0;

constexpr float lowLerpThres = 0.3;
constexpr float highLerpThres = 0.7;

const std::map<uint8_t, uint16_t> noTransmapping{{0x0, Test2BitBGColor},
                                                 {0x1, AlphaBlend(Test2BitBGColor, Test2BitFGColor, 178)},
                                                 {0x2, AlphaBlend(Test2BitBGColor, Test2BitFGColor, 76)},
                                                 {0x3, Test2BitFGColor}};

const std::map<uint8_t, uint16_t> withTransMapping{{0x0, Test2BitTransparentColor},
                                                   {0x1, AlphaBlend(Test2BitTransparentColor, Test2BitFGColor, 178)},
                                                   {0x2, AlphaBlend(Test2BitTransparentColor, Test2BitFGColor, 76)},
                                                   {0x3, Test2BitFGColor}};

// The Test2Icon, with each pixel stored as an element.
// Makes it easier to check the results.
uint8_t Test2BitFramebufferRepr[] = {
    0x3, 0x3, 0x3, 0x3, 0x2, 0x0, 0x0, 0x2, 0x3, 0x3, 0x3, 0x3, 0x1, 0x0, 0x2, 0x3, // 2 rows here
    0x3, 0x3, 0x3, 0x3, 0x2, 0x0, 0x0, 0x2, 0x3, 0x3, 0x3, 0x3, 0x1, 0x0, 0x2, 0x3,
    0x3, 0x3, 0x3, 0x3, 0x2, 0x0, 0x0, 0x2, 0x3, 0x3, 0x3, 0x3, 0x1, 0x0, 0x2, 0x3,
    0x3, 0x3, 0x3, 0x3, 0x2, 0x0, 0x0, 0x2, 0x3, 0x3, 0x3, 0x3, 0x1, 0x0, 0x2, 0x3,
};

#endif // TEST_2_BIT_LINE_OUTPUT_H

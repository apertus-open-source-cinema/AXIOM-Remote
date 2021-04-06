#include "Utils.h"

/* fg: The foreground color
 * bg: The background color
 * alpha: how much of the foreground color that should be used: 0-255
 * returns: a blended color between foreground and background
 */
uint16_t utils::AlphaBlend(uint16_t fg, uint16_t bg, uint8_t alpha)
{
    // Source: https://stackoverflow.com/a/19068028
    constexpr uint16_t maskRB = 63519U;      // 0b1111100000011111, the red and blue bits
    constexpr uint16_t maskG = 2016U;        // 0b0000011111100000, the green bits
    constexpr uint32_t maskMulRB = 4065216U; // 0b1111100000011111000000, the red and blue bits after multiplication
    constexpr uint32_t maskMulG = 129024U;   // 0b0000011111100000000000, the green bits after multiplication
    constexpr uint16_t maxAlpha = 64U;       // 6bits+1 with rounding: [0, 64]
    /* Explanation of "6 bits + 1":
     * If alpha > 6 bits, the blue bits get shifted into the red part
     * This is only fine if all of the red bits have been shifted further,
     * ie. only when alpha = 0b1_000_000
     * Hence 6 bits + 1.
     */

    alpha = (alpha + 2) >> 2; // convert from 8bit to (6bit+1) with rounding
    const uint8_t beta = maxAlpha - alpha;

    // The trick here is to perform the computations on the red and blue channels
    // together, since there is no risk of bit overlap.
    const uint32_t foreRB = static_cast<uint32_t>(fg & maskRB);
    const uint32_t backRB = static_cast<uint32_t>(bg & maskRB);
    const uint32_t redBlue = (alpha * foreRB + beta * backRB) & maskMulRB;
    const uint32_t foreG = fg & maskG;
    const uint32_t backG = bg & maskG;
    const uint32_t green = (alpha * foreG + beta * backG) & maskMulG;

    return (redBlue | green) >> 6; // divide by maxAlpha
}

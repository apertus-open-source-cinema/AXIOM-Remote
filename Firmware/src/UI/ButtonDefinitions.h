#ifndef BUTTONDEFINITIONS_H
#define BUTTONDEFINITIONS_H

#include <cstdint>

enum class ButtonID
{
    BUTTON_NONE = -1,

    // Display top
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,

    // Display Bottom
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,

    // Display left
    BUTTON_7,
    BUTTON_8,
    BUTTON_9,
 
    // Display right
    BUTTON_10,
    BUTTON_11,
    BUTTON_12,

    BUTTON_A,
    BUTTON_B,
    
    BUTTON_PHOTO,
    BUTTON_RECORD,
    
    // Rotary Knobs
    KNOB,
    // E_1,
    // E_2,
    // E1_DOWN,
    // E_2_DOWN,
    // E_1_UP,
    // E_2_UP,
};

enum class ButtonState
{
    Pressed,
    Released
};

#endif // BUTTONDEFINITIONS_H

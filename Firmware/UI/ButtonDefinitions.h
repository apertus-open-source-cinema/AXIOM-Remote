#ifndef BUTTONDEFINITIONS_H
#define BUTTONDEFINITIONS_H

// TODO: Find proper name as Button gives the impression of a base class, but is only containing definitions
enum class ButtonID
{
    BUTTON_NONE,

    // Display top
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_1_DOWN,
    BUTTON_2_DOWN,
    BUTTON_3_DOWN,
    BUTTON_1_UP,
    BUTTON_2_UP,
    BUTTON_3_UP,

    // Display Bottom
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_4_DOWN,
    BUTTON_5_DOWN,
    BUTTON_6_DOWN,
    BUTTON_4_UP,
    BUTTON_5_UP,
    BUTTON_6_UP,

    // Display left
    BUTTON_7,
    BUTTON_8,
    BUTTON_9,
    BUTTON_7_DOWN,
    BUTTON_8_DOWN,
    BUTTON_9_DOWN,
    BUTTON_7_UP,
    BUTTON_8_UP,
    BUTTON_9_UP,

    // Display right
    BUTTON_10,
    BUTTON_11,
    BUTTON_12,
    BUTTON_10_DOWN,
    BUTTON_11_DOWN,
    BUTTON_12_DOWN,
    BUTTON_10_UP,
    BUTTON_11_UP,
    BUTTON_12_UP,

    // Rotary Knobs
    E_1,
    E_2,
    E_1_DOWN,
    E_2_DOWN,
    E_1_UP,
    E_2_UP,
};

#endif // BUTTONDEFINITIONS_H

#pragma once

#include "ListParameterScreen.h"

class WhiteBalanceScreen : public ListParameterScreen
{
public:
    WhiteBalanceScreen(AppContext* context)
        : ListParameterScreen(context, "White Balance", "WB")
    {
        SetOptions({
            "Auto",
            "Daylight",
            "Cloudy",
            "Shade",
            "Tungsten",
            "Fluorescent",
            "Flash",
            "Custom"
        });
        SetWrapAround(false);

        // Set back button to go directly to main screen
        SetBackPressedCallback([context]() {
            context->ProcessActionWithAnimation(UIAction::GO_TO_PREVIOUS_SCREEN, AnimationDirection::DOWN);
        });
    }
};
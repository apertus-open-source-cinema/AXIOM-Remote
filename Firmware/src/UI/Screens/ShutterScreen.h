#pragma once

#include "ListParameterScreen.h"
#include <vector>
#include <string>

class ShutterScreen : public ListParameterScreen
{
public:
    ShutterScreen(AppContext* context)
        : ListParameterScreen(context, "Shutter Speed", "Shutter")
    {
        const std::vector<std::string> shutterSpeedLabels = {
            "1/8000s", "1/4000s", "1/2000s", "1/1000s", "1/500s", "1/250s", "1/125s",
            "1/60s", "1/30s", "1/15s", "1/8s", "1/4s", "1/2s",
            "1s", "2s", "4s", "8s", "15s", "30s"
        };
        SetOptions(shutterSpeedLabels);
        SetWrapAround(false);

        // Set back button to go directly to main screen
        SetBackPressedCallback([context]() {
            context->ProcessActionWithAnimation(UIAction::GO_TO_PREVIOUS_SCREEN, AnimationDirection::DOWN);
        });
    }
};

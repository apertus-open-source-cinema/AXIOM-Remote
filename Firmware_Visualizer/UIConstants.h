#pragma once

namespace UIConstants {
const float FullCircleRad = 2 * M_PI;
const float Tick = FullCircleRad / 12;
const float HalfPi = static_cast<float>(M_PI) / 2.0f;

// Knob dimensions (diameter and offsets can be ints for pixel placement)
const int KnobDiameter = 200;
const float KnobRadius =
    KnobDiameter * 0.5f; // Radius might be used in float calculations
const int KnobPressableDiameter = static_cast<int>(0.3f * KnobDiameter);
const float KnobPressableRadiusSquared =
    0.25f * KnobPressableDiameter * KnobPressableDiameter;
const ImVec2 KnobPressableSize =
    ImVec2(static_cast<float>(KnobPressableDiameter),
           static_cast<float>(KnobPressableDiameter));
const float KnobPressableOffset =
    0.50f * (KnobDiameter - KnobPressableDiameter);
const int KnobValueTextOffset = 200;

// Button dimensions
const int SmallButtonWidth = 46;
const int SmallButtonHeight = 30;
const int RoundButtonDiameter = 30;
const ImVec2 RoundButtonSize = ImVec2(static_cast<float>(RoundButtonDiameter),
                                      static_cast<float>(RoundButtonDiameter));
const int PhotoButtonWidth = 46;
const int PhotoButtonHeight = 46;
const int RecordButtonWidth = 68;
const int RecordButtonHeight = 68;

// LED dimensions and offsets
const ImVec2 LEDSize =
    ImVec2(30.0f, 29.0f); // Size could remain float if used with ImGui::Image
const ImVec2 LEDGlowSize = ImVec2(40.0f, 40.0f); // Size could remain float
const ImVec2 LEDGlowOffset = ImVec2(5.0f, 5.0f); // Offset could be int

// Virtual camera FBO dimensions
const int CameraFBOWidth = 800;
const int CameraFBOHeight = 480;
const int DisplayFBOWidth = 320;
const int DisplayFBOHeight = 240;

// Overlay dimensions and positioning
const ImVec2 OverlaySize = ImVec2(120.0f, 400.0f); // Size could remain float
const ImVec2 OverlayPosition =
    ImVec2(650.0f, 30.0f); // Position needs to be float
const int OverlayTextOffsetX = 20;
const int OverlayTextOffsetY = 20;
const int OverlayTextSpacingY = 100;

// UI element positions (these need to be ImVec2 with float components)
const ImVec2 KnobPosition = ImVec2(40.0f, 140.0f);
const ImVec2 SpecialButton1Position = ImVec2(55.0f, 22.0f);
const ImVec2 SpecialButton2Position = ImVec2(148.0f, 22.0f);
const ImVec2 PhotoButtonPosition = ImVec2(236.0f, 367.0f);
const ImVec2 RecordButtonPosition = ImVec2(225.0f, 56.0f);
const int ButtonSpacingX = 103;
const int RoundButtonSpacingY = 55;
const ImVec2 LEDTopPosition = ImVec2(63.0f, 70.0f);
const ImVec2 LEDBottomPosition = ImVec2(63.0f, 109.0f);
const ImVec2 CameraPreviewPosition = ImVec2(338.0f, 120.0f);
const ImVec2 DebugTogglePosition = ImVec2(50.0f, 375.0f);
const ImVec2 ContrastTogglePosition = ImVec2(50.0f, 410.0f);
const ImVec2 ScreenshotButtonPosition = ImVec2(50.0f, 445.0f);

// These are offsets/spacing
const int Buttons1_3_OffsetY = 55;
const int Buttons4_6_OffsetY = 395;
const int Buttons7_9_OffsetX = 249;
const int Buttons10_12_OffsetX = 717;

const char *buttonIDs[12] = {"1", "2", "3", "4",  "5",  "6",
                             "7", "8", "9", "10", "11", "12"};

// Magic number constants for button grid positions
const float BUTTONS_1_3_X = 372.0f;
const float BUTTONS_7_9_GRID_Y_START = 170.0f;
const float BUTTONS_GRID_Y_START = 170.0f;
const float CAMERA_PREVIEW_ZOOM = 4.0f;
const float CAMERA_PREVIEW_ZOOM_REGION = 48.0f;
const float CAMERA_BRIGHTNESS_FACTOR = 0.1f;
const float LCD_BRIGHTNESS_FACTOR = 1.0f / 100.0f;
const uint8_t DEFAULT_BRIGHTNESS_LEVEL = 16;
const uint8_t DEFAULT_LCD_BRIGHTNESS = 100;
const float DEFAULT_LCD_CONTRAST = 1.0f;
const float DEFAULT_LCD_CONTRAST_REDUCED = 0.7f;
} // namespace UIConstants

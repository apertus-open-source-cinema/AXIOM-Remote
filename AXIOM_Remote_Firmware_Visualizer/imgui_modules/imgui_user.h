#ifndef IMGUI_USER_H
#define IMGUI_USER_H

#include "SDL2/SDL.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui {
static float fullCircleRad = 2 * M_PI;
static float tick = fullCircleRad / 12; // 30°, 12 -> possible detents of real rotary encoder
static float halfP = static_cast<float>(M_PI) / 2.0f;
static float oldAngle = 0;

void HandleActiveKnob(uint8_t* value, const ImVec2& center)
{
    ImVec2 mp = ImGui::GetIO().MousePos;

    float newAngle = atan2f(mp.y - center.y, center.x - mp.x);
    newAngle += halfP;
    if (oldAngle < 0)
    {
        oldAngle += fullCircleRad;
    }
    if (oldAngle > fullCircleRad)
    {
        int fullRotations = oldAngle / fullCircleRad;
        oldAngle -= fullRotations * fullCircleRad;
    }

    float delta = newAngle + oldAngle;
    if (delta < 0)
    {
        // Underflow, convert to full circle
        delta += fullCircleRad;
    }
    if (delta > M_PI)
    {
        // Convert from full circle (0..360) to half circle (-180..180) to get rotation direction
        delta -= fullCircleRad;
    }

    if (delta < -tick)
    {
        value[0]++;
        oldAngle += tick;
    } else if (delta > tick)
    {
        value[0]--;
        oldAngle -= tick;
    }
}

// Reference: https://github.com/denilsonsa/html5-knob/blob/master/xknob.js
IMGUI_API bool Knob(const char* label, uint8_t* value, ImTextureID tex)
{

    ImVec2 p = ImGui::GetCursorScreenPos();
    float knobSize = 200.0f;
    float radius = knobSize * 0.5f;
    ImVec2 center = ImVec2(p.x + radius, p.y + radius);
    char textval[256];

    ImVec2 textpos = p;
    // Slight offset of value text to prevent overlap
    textpos.y -= 20;

    const ImGuiStyle& style = ImGui::GetStyle();
    float lineHeight = ImGui::GetTextLineHeight();
    ImGui::InvisibleButton(label, ImVec2(knobSize, knobSize + lineHeight + style.ItemInnerSpacing.y));

    bool is_active = ImGui::IsItemActive();
    // TODO: Add button in the middle to simulate knob press
    //bool is_hovered = ImGui::IsItemHovered();
    bool touched = false;

    if (is_active)
    {
        touched = true;
        HandleActiveKnob(value, center);
    }

    ImU32 col32text = ImGui::GetColorU32(ImGuiCol_Text);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 size = ImVec2(knobSize, knobSize);

    float cos_a = cosf(oldAngle);
    float sin_a = sinf(oldAngle);
    ImVec2 pos[4] = {center + ImRotate(ImVec2(-size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
                     center + ImRotate(ImVec2(+size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
                     center + ImRotate(ImVec2(+size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a),
                     center + ImRotate(ImVec2(-size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a)};

    ImVec2 uvs[4] = {ImVec2(0.0f, 0.0f), ImVec2(1.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec2(0.0f, 1.0f)};

    draw_list->AddImageQuad(tex, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], IM_COL32_WHITE);

    ImFormatString(textval, IM_ARRAYSIZE(textval), "Value: %d", *value);
    draw_list->AddText(textpos, col32text, textval);

    return touched;
}
} // namespace ImGui
#endif // IMGUI_USER_H

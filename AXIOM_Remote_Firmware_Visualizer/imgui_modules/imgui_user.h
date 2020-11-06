#ifndef IMGUI_USER_H
#define IMGUI_USER_H

#include "SDL2/SDL.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui {
static const float fullCircleRad = 2 * M_PI;
static const float tick = fullCircleRad / 12; // 30°, 12 -> possible detents of real rotary encoder
static const float halfP = static_cast<float>(M_PI) / 2.0f;

// Knob dimensions
static const float knobDiameter = 200.0f;
static const float knobRadius = knobDiameter * 0.5f;
static const float knobPressableDiameter = 0.3 * knobDiameter;
static const float knobPressableRadiusSquared = 0.25 * knobPressableDiameter * knobPressableDiameter;
static const ImVec2 knobPressableSize = ImVec2(knobPressableDiameter, knobPressableDiameter);
static const float knobPressableOffset = 0.50 * (knobDiameter - knobPressableDiameter);
static const float knobValueTextOffset = 200.0f; // slight offset to prevent overlap with knob

static float oldAngle = 0;

void UpdateKnobValue(uint8_t& value, const ImVec2& center)
{
    ImVec2 mousePos = ImGui::GetIO().MousePos;

    float newAngle = atan2f(mousePos.y - center.y, center.x - mousePos.x);
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
        value++;
        oldAngle += tick;
    } else if (delta > tick)
    {
        value--;
        oldAngle -= tick;
    }
}

// Reference: https://github.com/denilsonsa/html5-knob/blob/master/xknob.js
IMGUI_API bool Knob(const char* label, uint8_t& value, bool& pressed, ImTextureID texture)
{
    const ImVec2 knobOrigin = ImGui::GetCursorScreenPos();
    const ImVec2 knobCenter = ImVec2(knobOrigin.x + knobRadius, knobOrigin.y + knobRadius);
    const ImVec2 knobValueTextPosition = ImVec2(knobOrigin.x, knobOrigin.y + knobValueTextOffset);
    static char knobValueText[64];

    ImGui::InvisibleButton(label, ImVec2(knobDiameter, knobDiameter));

    bool isKnobActive = ImGui::IsItemActive();
    bool touched = false;

    if (isKnobActive)
    {
        touched = true;
        UpdateKnobValue(value, knobCenter);
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    float cosine = cosf(oldAngle);
    float sine = sinf(oldAngle);
    ImVec2 pos[4] = {knobCenter + ImRotate(ImVec2(-knobRadius, -knobRadius), cosine, sine),
                     knobCenter + ImRotate(ImVec2(+knobRadius, -knobRadius), cosine, sine),
                     knobCenter + ImRotate(ImVec2(+knobRadius, +knobRadius), cosine, sine),
                     knobCenter + ImRotate(ImVec2(-knobRadius, +knobRadius), cosine, sine)};

    ImVec2 uvs[4] = {ImVec2(0.0f, 0.0f), ImVec2(1.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec2(0.0f, 1.0f)};

    drawList->AddImageQuad(texture, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], IM_COL32_WHITE);

    ImFormatString(knobValueText, IM_ARRAYSIZE(knobValueText), "Value: %d", value);
    drawList->AddText(knobValueTextPosition, ImGui::GetColorU32(ImGuiCol_Text), knobValueText);

    // As Dear ImGui only allows rectangular clickable areas (e.g. in the form of a button), having a large such area to
    // detect knob presses comes at the price of the corners of that area interfering with the knob rotation. To work
    // around this, check that the mouse position lies within the specified knob-press circular area before attempting
    // to detect a knob press.
    ImVec2 mousePos = ImGui::GetIO().MousePos;
    float diffX{mousePos.x - knobCenter.x}, diffY{mousePos.y - knobCenter.y};
    if (diffX * diffX + diffY * diffY < knobPressableRadiusSquared)
    {
        ImVec2 knobPressableOrigin = ImVec2(knobOrigin.x + knobPressableOffset, knobOrigin.y + knobPressableOffset);
        ImGui::SetCursorPos(knobPressableOrigin);
        ImGui::BeginChild("pressableKnobOverlay", knobPressableSize, false);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, knobPressableDiameter / 2);
        if (ImGui::Button("Click", knobPressableSize))
        {
            pressed = true;
        }

        ImGui::PopStyleVar();
        ImGui::EndChild();
    }

    return touched;
}

void ToggleButton(const char* str_id, const char* label, bool* v)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked())
    {
        *v = !*v;
    }

    float t = *v ? 1.0f : 0.0f;

    ImGuiContext& g = *GImGui;
    float ANIM_SPEED = 0.08f;
    if (g.LastActiveId == g.CurrentWindow->GetID(str_id))
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = *v ? (t_anim) : (1.0f - t_anim);
    }

    ImU32 col_bg;
    if (ImGui::IsItemHovered())
    {
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
    } else
    {
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));
    }

    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f,
                               IM_COL32(255, 255, 255, 255));

    // Draw label
    ImVec2 textpos = p;
    textpos.x += 40;
    textpos.y += 2;
    ImU32 col32text = ImGui::GetColorU32(ImGuiCol_Text);
    draw_list->AddText(textpos, col32text, label);
}

bool CustomImageButton(const char* id, ImTextureID texture_id, ImTextureID pressed_texture_id, const ImVec2& size,
                       const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1))
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, GetID(id)))
    {
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, GetID(id), &hovered, &held, ImGuiButtonFlags_PressedOnClickRelease);

    ImU32 tint = IM_COL32(200, 200, 200, 255);
    if (hovered)
    {
        tint = IM_COL32(255, 255, 255, 255);
    }
    if (hovered && held)
    {
        texture_id = pressed_texture_id;
    }

    window->DrawList->AddImage(texture_id, bb.Min, bb.Max, uv0, uv1, tint);

    return pressed;
}

} // namespace ImGui
#endif // IMGUI_USER_H

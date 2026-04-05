#pragma once

#include "SDL3/SDL.h"

#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>
#include <cstdio>

namespace ImGui {
static const float fullCircleRad = 2 * M_PI;
static const float tick = fullCircleRad / 12;
static const float halfP = static_cast<float>(M_PI) / 2.0f;

// Knob dimensions
// Based on: https://github.com/ocornut/imgui/issues/942
int KnobEncoder(const char* label, float* p_angle_rad, bool* p_pressed, const char* button_label, const ImVec2& size, ImTextureID texture, ImU32 tint)
{
    ImGuiIO& io = GetIO();
    ImGuiStyle& style = GetStyle();
    ImDrawList* draw_list = GetWindowDrawList();
    ImGuiID id = GetID(label);

    ImVec2 knob_size = size;
    if (knob_size.x == 0) knob_size.x = GetItemRectSize().x;
    if (knob_size.y == 0) knob_size.y = GetItemRectSize().y;

    float radius = ImMin(knob_size.x, knob_size.y) / 2.0f;
    ImVec2 center = ImVec2(GetCursorScreenPos().x + radius, GetCursorScreenPos().y + radius);

    // Main invisible button behavior
    float line_height = GetTextLineHeight();
    InvisibleButton(label, ImVec2(radius * 2, radius * 2 + line_height + style.ItemInnerSpacing.y));
    bool is_active = IsItemActive();
    bool is_clicked = IsItemClicked();
    bool is_hovered = IsItemHovered();

    if (p_pressed) *p_pressed = false;

    int tick_delta = 0;
    const float tick_angle = (2 * M_PI) / 12.0f; // 12 detents

    // Check for interaction with the center button area
    float pressable_radius = radius * 0.4f;
    ImVec2 mouse_from_center = io.MousePos - center;
    float dist_sq = mouse_from_center.x * mouse_from_center.x + mouse_from_center.y * mouse_from_center.y;
    bool is_over_center = dist_sq < pressable_radius * pressable_radius;
    bool is_held = false;
    
    if (is_active && is_over_center) {
        if (is_clicked) {
            if (p_pressed) *p_pressed = true;
        }
        if (!IsMouseDragging(ImGuiMouseButton_Left)) {
            is_held = true;
        }
    }
    
    // Handle dragging the knob (only if not considered a button hold/click)
    if (is_active && !is_over_center && IsMouseDragging(ImGuiMouseButton_Left)) {
        ImGuiStorage* storage = GetStateStorage();
        float drag_start_offset = storage->GetFloat(id, 0.0f);
        
        float mouse_angle_raw = atan2f(io.MousePos.y - center.y, io.MousePos.x - center.x);
        float mouse_angle_mapped = fmodf(mouse_angle_raw + 2.5f * M_PI, 2.0f * M_PI);

        if (IsItemActivated()) {
            drag_start_offset = mouse_angle_mapped - *p_angle_rad;
            if (drag_start_offset > M_PI) drag_start_offset -= 2 * M_PI;
            if (drag_start_offset < -M_PI) drag_start_offset += 2 * M_PI;
            storage->SetFloat(id, drag_start_offset);
        }

        float new_angle = mouse_angle_mapped - drag_start_offset;
        float angle_diff = new_angle - *p_angle_rad;
        
        while (angle_diff >= M_PI) angle_diff -= 2 * M_PI;
        while (angle_diff < -M_PI) angle_diff += 2 * M_PI;

        if (fabsf(angle_diff) >= tick_angle) {
            int ticks = roundf(angle_diff / tick_angle);
            tick_delta = ticks;
            *p_angle_rad += ticks * tick_angle;
        }
    }

    // --- Rendering ---
    
    // Ensure angle stays in [0, 2*PI] range for consistent drawing
    *p_angle_rad = fmodf(*p_angle_rad, 2 * M_PI);
    if (*p_angle_rad < 0.0f) {
        *p_angle_rad += 2 * M_PI;
    }
    
    // Draw the main knob image
    float cos_a = cosf(*p_angle_rad);
    float sin_a = sinf(*p_angle_rad);
    ImVec2 knob_quad_size = ImVec2(radius * 2, radius * 2);

    ImVec2 pos[4] = {
        center + ImRotate(ImVec2(-knob_quad_size.x * 0.5f, -knob_quad_size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(+knob_quad_size.x * 0.5f, -knob_quad_size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(+knob_quad_size.x * 0.5f, +knob_quad_size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(-knob_quad_size.x * 0.5f, +knob_quad_size.y * 0.5f), cos_a, sin_a)
    };
    ImVec2 uvs[4] = {
        ImVec2(0.0f, 0.0f), ImVec2(1.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec2(0.0f, 1.0f)
    };
    draw_list->AddImageQuad(texture, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], tint);

    // Draw visual feedback for the center button
    if (is_held) {
        draw_list->AddCircleFilled(center, pressable_radius, IM_COL32(0, 0, 0, 128)); // Darker circle for held state
    } else if (is_hovered && is_over_center) {
        draw_list->AddCircleFilled(center, pressable_radius, IM_COL32(255, 255, 255, 50)); // Lighter circle for hover
    }

    // Draw the button label
    if (button_label && *button_label)
    {
        ImVec2 label_size = CalcTextSize(button_label, NULL, true);
        draw_list->AddText(ImVec2(center.x - label_size.x * 0.5f, center.y - label_size.y * 0.5f), GetColorU32(ImGuiCol_Text), button_label);
    }

    return tick_delta;
}


void ToggleButton(const char *id, const char *label, bool *state) {
    ImVec2 pos = GetCursorScreenPos();
    float height = GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.5f;

    InvisibleButton(id, {width, height});
    if (IsItemClicked())
        *state = !*state;

    ImDrawList *drawList = GetWindowDrawList();
    ImU32 bgColor = GetColorU32(*state ? ImVec4(0.56f, 0.83f, 0.26f, 1.0f) : ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
    drawList->AddRectFilled(pos, {pos.x + width, pos.y + height}, bgColor, radius);

    float circlePosX = pos.x + radius + (*state ? (width - 2 * radius) : 0.0f);
    drawList->AddCircleFilled({circlePosX, pos.y + radius}, radius - 1.5f, IM_COL32_WHITE);

    drawList->AddText({pos.x + 40, pos.y + 2}, GetColorU32(ImGuiCol_Text), label);
}

bool CustomImageButton(const char *id, ImTextureID texture, ImTextureID pressedTexture, const ImVec2 &size, ImU32 tint = IM_COL32_WHITE) {
    ImGuiWindow *window = GetCurrentWindow();
    ImGuiID buttonId = GetID(id);
    ImRect rect(window->DC.CursorPos, window->DC.CursorPos + size);

    ItemSize(size);
    if (!ItemAdd(rect, buttonId))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(rect, buttonId, &hovered, &held, ImGuiButtonFlags_PressedOnClick);

    ImTextureID displayTex = (held && pressedTexture) ? pressedTexture : texture;
    window->DrawList->AddImage(displayTex, rect.Min, rect.Max, {0.0f, 0.0f}, {1.0f, 1.0f}, tint);

    return pressed;
}
} // namespace ImGui
#ifndef IMGUI_USER_H
#define IMGUI_USER_H

#include "SDL2/SDL.h"

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

namespace ImGui
{

float old_value = 0;
IMGUI_API bool Knob(const char* label, float* value_p, ImTextureID tex)
{
    ImGuiStyle& style = ImGui::GetStyle();
    float line_height = ImGui::GetTextLineHeight();

    ImVec2 p = ImGui::GetCursorScreenPos();
    float sz = 200.0f;
    float radius =  sz * 0.5f;
    ImVec2 center = ImVec2(p.x + radius, p.y + radius);
    float val1 = *value_p;//value_p[0]; //(]value_p[0] - minv) / (maxv - minv);
    char textval[32];
    ImFormatString(textval, IM_ARRAYSIZE(textval), "%04.1f", value_p[0]);

    ImVec2 textpos = p;
    //float gamma = (float)M_PI / 4.0f; //0 value in knob
    //float alpha = ((float)M_PI - gamma) * val1 * 2.0f + gamma;

    float alpha = val1 * (float)M_PI / 180;// * 2.0f;

    float x2 = -sinf(alpha) * radius + center.x;
    float y2 = cosf(alpha) * radius + center.y;

    ImGui::InvisibleButton(label,ImVec2(sz, sz + line_height + style.ItemInnerSpacing.y));

    bool is_active = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemHovered();
    bool touched = false;

    if (is_active)
    {
        touched = true;
        ImVec2 mp = ImGui::GetIO().MousePos;
        ImVec2 value_raw = ImGui::GetMouseDragDelta(0, 0.0f);
        alpha = atan2f(value_raw.x - center.x, center.y - value_raw.y);// + (float)M_PI;
        //alpha = ImMax(gamma,ImMin<float>(2.0f*(float)M_PI-gamma,alpha));
        //float value = 0.5 * alpha;//0.5f*(alpha-gamma)/((float)M_PI-gamma);
        value_p[0] = alpha / (float)M_PI * 180;//value*(maxv - minv) + minv;
    }

    ImU32 col32 = ImGui::GetColorU32(is_active ? ImGuiCol_FrameBgActive : is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    ImU32 col32line = ImGui::GetColorU32(ImGuiCol_SliderGrabActive);
    ImU32 col32text = ImGui::GetColorU32(ImGuiCol_Text);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 size = ImVec2(200, 200);

    float cos_a = cosf(alpha);
    float sin_a = sinf(alpha);
    ImVec2 pos[4] =
    {
        center + ImRotate(ImVec2(-size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(+size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(+size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(-size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a)
    };

    ImVec2 uvs[4] =
    {
        ImVec2(0.0f, 0.0f),
        ImVec2(1.0f, 0.0f),
        ImVec2(1.0f, 1.0f),
        ImVec2(0.0f, 1.0f)
    };

    draw_list->AddImageQuad(tex,pos[0], pos[1], pos[2], pos[3],uvs[0], uvs[1], uvs[2], uvs[3], IM_COL32_WHITE);

    //draw_list->AddCircleFilled(center, radius, col32, 32);
    //draw_list->AddLine(center, ImVec2(x2, y2), col32line, 2);
    draw_list->AddText(textpos, col32text, textval);
    //draw_list->AddText(ImVec2(p.x, p.y + sz + style.ItemInnerSpacing.y), col32text, label);

    old_value = *value_p;

    return touched;
}
}
#endif // IMGUI_USER_H

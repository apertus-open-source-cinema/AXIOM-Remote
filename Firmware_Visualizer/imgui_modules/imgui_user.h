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
static const float knobDiameter = 200.0f;
static const float knobRadius = knobDiameter * 0.5f;
static const float knobPressableDiameter = 0.3f * knobDiameter;
static const float knobPressableRadiusSquared = 0.25f * knobPressableDiameter * knobPressableDiameter;
static const ImVec2 knobPressableSize = ImVec2(knobPressableDiameter, knobPressableDiameter);
static const float knobPressableOffset = 0.50f * (knobDiameter - knobPressableDiameter);
static const float knobValueTextOffset = 200.0f;

static float oldAngle = 0.0f;
static int oldTickValue = 0; // To track crossing of ticks

void UpdateKnobValue(uint8_t &value, const ImVec2 &center) {
  ImVec2 mousePos = ImGui::GetIO().MousePos;
  // Calculate the angle from the knob center to the mouse position.
  // Adding halfP (+ M_PI / 2) is often done to align the zero angle upwards.
  float newAngleRaw = atan2f(mousePos.y - center.y, center.x - mousePos.x) + halfP;
  if (newAngleRaw < 0.0f)
      newAngleRaw += fullCircleRad;

  // Calculate the difference between the new angle and the old angle.
  float deltaRaw = newAngleRaw - oldAngle;

  // Normalize the delta to be within -PI to PI for correct direction detection.
  if (deltaRaw > M_PI)
      deltaRaw -= fullCircleRad;
  else if (deltaRaw < -M_PI)
      deltaRaw += fullCircleRad;

  if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
      float angleThreshold = tick * 0.5f; // Adjust sensitivity

      if (deltaRaw > angleThreshold) {
          // Mouse moved clockwise (positive delta), should increase value
          value = (value < 255) ? value + 1 : 0;
          oldAngle += tick;
          if (oldAngle > M_PI) oldAngle -= fullCircleRad;
      } else if (deltaRaw < -angleThreshold) {
          // Mouse moved counter-clockwise (negative delta), should decrease value
          value = (value > 0) ? value - 1 : 255;
          oldAngle -= tick;
          if (oldAngle < -M_PI) oldAngle += fullCircleRad;
      }
  }
}

ImVec2 GetKnobUV(float angle) {
  angle = fmodf(angle, fullCircleRad);
  if (angle < 0.0f) angle += fullCircleRad;

  // Convert angle to a vector on the unit circle
  float cos_a = cosf(angle);
  float sin_a = sinf(angle);

  // Map this vector to UV coordinates (center of the circle is at 0.5, 0.5)
  float u = 0.5f + 0.5f * cos_a;
  float v = 0.5f + 0.5f * sin_a;

  return {u, v};
}

bool Knob(const char *label, uint8_t &value, bool &pressed, ImTextureID texture) {
  ImVec2 knobOrigin = ImGui::GetCursorScreenPos();
  ImVec2 knobCenter = {knobOrigin.x + knobRadius, knobOrigin.y + knobRadius};

  ImGui::InvisibleButton(label, {knobDiameter, knobDiameter});
  bool isActive = ImGui::IsItemActive();

  if (isActive) {
      UpdateKnobValue(value, knobCenter);
  }

  ImDrawList *drawList = ImGui::GetWindowDrawList();
  float angle = oldAngle;

  // UV coordinates corresponding to the corners of the *unrotated* quad
  // These will sample the texture based on the rotation
  ImVec2 uv_tl = GetKnobUV(angle + fullCircleRad * (3.0f / 8.0f)); // Top-Left (adjust offset as needed)
  ImVec2 uv_tr = GetKnobUV(angle + fullCircleRad * (1.0f / 8.0f)); // Top-Right
  ImVec2 uv_br = GetKnobUV(angle + fullCircleRad * (7.0f / 8.0f)); // Bottom-Right
  ImVec2 uv_bl = GetKnobUV(angle + fullCircleRad * (5.0f / 8.0f)); // Bottom-Left

  ImVec2 corners[4] = {
      knobOrigin,
      {knobOrigin.x + knobDiameter, knobOrigin.y},
      {knobOrigin.x + knobDiameter, knobOrigin.y + knobDiameter},
      {knobOrigin.x, knobOrigin.y + knobDiameter}
  };

  drawList->AddImageQuad(texture, corners[0], corners[1], corners[2], corners[3], uv_tl, uv_tr, uv_br, uv_bl);

  char buf[32];
  snprintf(buf, sizeof(buf), "Value: %d", value);
  drawList->AddText({knobOrigin.x, knobOrigin.y + knobValueTextOffset}, ImGui::GetColorU32(ImGuiCol_Text), buf);

  float distSq = powf(ImGui::GetIO().MousePos.x - knobCenter.x, 2) + powf(ImGui::GetIO().MousePos.y - knobCenter.y, 2);
  if (distSq < knobPressableRadiusSquared) {
      ImGui::SetCursorScreenPos({knobOrigin.x + knobPressableOffset, knobOrigin.y + knobPressableOffset});
      ImGui::BeginChild("KnobPressable", knobPressableSize, false);
      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, knobPressableDiameter * 0.5f);
      if (ImGui::Button("Click", knobPressableSize))
          pressed = true;
      ImGui::PopStyleVar();
      ImGui::EndChild();
  }

  return isActive;
}

void ToggleButton(const char *id, const char *label, bool *state) {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.5f;

    ImGui::InvisibleButton(id, {width, height});
    if (ImGui::IsItemClicked())
        *state = !*state;

    ImDrawList *drawList = ImGui::GetWindowDrawList();
    ImU32 bgColor = ImGui::GetColorU32(*state ? ImVec4(0.56f, 0.83f, 0.26f, 1.0f) : ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
    drawList->AddRectFilled(pos, {pos.x + width, pos.y + height}, bgColor, radius);

    float circlePosX = pos.x + radius + (*state ? (width - 2 * radius) : 0.0f);
    drawList->AddCircleFilled({circlePosX, pos.y + radius}, radius - 1.5f, IM_COL32_WHITE);

    drawList->AddText({pos.x + 40, pos.y + 2}, ImGui::GetColorU32(ImGuiCol_Text), label);
}

bool CustomImageButton(const char *id, ImTextureID texture, ImTextureID pressedTexture, const ImVec2 &size, ImU32 tint = IM_COL32_WHITE) {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImGuiID buttonId = ImGui::GetID(id);
    ImRect rect(window->DC.CursorPos, window->DC.CursorPos + size);

    ImGui::ItemSize(size);
    if (!ImGui::ItemAdd(rect, buttonId))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, buttonId, &hovered, &held, ImGuiButtonFlags_PressedOnClick);

    ImTextureID displayTex = (held && pressedTexture) ? pressedTexture : texture;
    window->DrawList->AddImage(displayTex, rect.Min, rect.Max, {0.0f, 0.0f}, {1.0f, 1.0f}, tint);

    return pressed;
}
} // namespace ImGui
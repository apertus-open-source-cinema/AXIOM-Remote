#ifndef VIRTUALUI_H
#define VIRTUALUI_H

#include <stdint.h>

#include "imgui.h"

#include "imgui_modules/imgui_impl_opengl3.h"
#include "imgui_modules/imgui_impl_sdl.h"

enum class Button;

void RenderUI(SDL_Window* window, const ImGuiIO& io, ImTextureID knoTextureID, ImTextureID displayTextureID, Button& button, int8_t& knobValue);

#endif // VIRTUALUI_H


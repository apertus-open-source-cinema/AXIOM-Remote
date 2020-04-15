#ifndef VIRTUALUI_H
#define VIRTUALUI_H

#include "imgui.h"

#include "imgui_modules/imgui_impl_opengl3.h"
#include "imgui_modules/imgui_impl_sdl.h"

enum class Button;

void RenderUI(SDL_Window* window, const ImGuiIO& io, ImTextureID knoTextureID, ImTextureID displayTextureID, Button& button);

#endif // VIRTUALUI_H

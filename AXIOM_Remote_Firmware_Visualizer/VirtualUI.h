#ifndef VIRTUALUI_H
#define VIRTUALUI_H

#include "imgui.h"

#include "imgui_modules/imgui_impl_opengl3.h"
#include "imgui_modules/imgui_impl_sdl.h"

// #include "AXIOM_Remote_Prototype_V01.X/globals.h"

// extern "C" {
// #include "AXIOM_Remote_Prototype_V01.X/menu.h"
// #include "AXIOM_Remote_Prototype_V01.X/page.h"

// #include "AXIOM_Remote_Prototype_V01.X/page_wb.h"
// #include "AXIOM_Remote_Prototype_V01.X/page_wb_help.h"
// }

void RenderUI(SDL_Window *window, ImTextureID textureID);

#endif //VIRTUALUI_H

#ifndef VIRTUALUI_H
#define VIRTUALUI_H

#include <iostream>

#include "3rdParty/imgui/imgui.h"
#include "imgui_modules/imgui_impl_sdl.h"
#include "imgui_modules/imgui_impl_opengl2.h"

// Additional controls, like knob
#include "imgui_modules/imgui_user.h"

#include "AXIOM_Remote_Prototype_V01.X/globals.h"

extern "C"
{
#include "AXIOM_Remote_Prototype_V01.X/page.h"
#include "AXIOM_Remote_Prototype_V01.X/menu.h"
}

float value = 0;

void RenderUI(SDL_Window* window, ImTextureID textureID)
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    //ImGui::ShowStyleEditor();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(800, 480));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImU32)ImColor(0, 64, 96, 255));
    ImGui::Begin("Image", nullptr, ImVec2(0, 0), -1, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::SetCursorPos(ImVec2(450, 70));
    ImGui::Button("1", ImVec2(40, 30));
    ImGui::SetCursorPos(ImVec2(530, 70));
    ImGui::Button("2", ImVec2(40, 30));
    ImGui::SetCursorPos(ImVec2(610, 70));
    ImGui::Button("3", ImVec2(40, 30));

    ImGui::SetCursorPos(ImVec2(450, 380));
    ImGui::Button("4", ImVec2(40, 30));
    ImGui::SetCursorPos(ImVec2(530, 380));
    ImGui::Button("5", ImVec2(40, 30));
    ImGui::SetCursorPos(ImVec2(610, 380));
    ImGui::Button("6", ImVec2(40, 30));

    ImGui::SetCursorPos(ImVec2(740, 170));
    ImGui::Button("10", ImVec2(40, 30));
    ImGui::SetCursorPos(ImVec2(740, 220));
    ImGui::Button("11", ImVec2(40, 30));
    ImGui::SetCursorPos(ImVec2(740, 270));
    ImGui::Button("12", ImVec2(40, 30));

    ImGui::SetCursorPos(ImVec2(340, 170));
    if(ImGui::Button("7", ImVec2(40, 30)))
    {
        //main_page_button_press_handler(ButtonID::P7);
        main_menu_button_press_handler(ButtonID::P7);
        btn_P7_pressed = true;
    }
    else
    {
        //main_page_button_release_handler(ButtonID::P7);
        if(btn_P7_pressed)
        {
            main_menu_button_release_handler(ButtonID::P7);
            btn_P7_pressed = false;
        }
    }
    ImGui::SetCursorPos(ImVec2(340, 220));
    if(ImGui::Button("8", ImVec2(40, 30)))
    {
        main_menu_button_press_handler(ButtonID::P8);
        btn_P8_pressed = true;
    }
    else
    {
        if(btn_P8_pressed)
        {
            main_menu_button_release_handler(ButtonID::P8);
            btn_P8_pressed = false;
        }
    }

    ImGui::SetCursorPos(ImVec2(340, 270));
    ImGui::Button("9", ImVec2(40, 30));

    ImGui::SetCursorPos(ImVec2(60, 140));
    ImGui::Knob("Test123", &value, (ImTextureID)textureID);
    ImGui::PopStyleColor();
    ImGui::End();
    ImGui::EndFrame();

    ImGui::Render();
}

#endif // VIRTUALUI_H

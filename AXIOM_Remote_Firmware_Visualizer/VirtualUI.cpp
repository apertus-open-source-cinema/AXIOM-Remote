#include "VirtualUI.h"

#include <iostream>

// Additional controls, like knob
#include "imgui_modules/imgui_user.h"

uint8_t value = 0;
uint8_t lastValue = 0;

void RenderUI(SDL_Window *window, ImTextureID textureID)
{
          std::cout << "Debug point UI 0" << std::endl;

    ImGui_ImplOpenGL3_NewFrame();
        std::cout << "Debug point UI 1" << std::endl;

    ImGui_ImplSDL2_NewFrame(window);
        std::cout << "Debug point UI 2" << std::endl;

    ImGui::NewFrame();

    //ImGui::ShowStyleEditor();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(800, 480));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImU32)ImColor(96, 96, 96, 255));
    ImGui::Begin("Image", nullptr, ImVec2(0, 0), -1,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse);

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
    if (ImGui::Button("7", ImVec2(40, 30)))
    {
        //     // main_page_button_press_handler(ButtonID::P7);
        //     main_menu_button_press_handler(ButtonID::P7);
        //     btn_P7_pressed = true;
        //   } else {
        //     // main_page_button_release_handler(ButtonID::P7);
        //     if (btn_P7_pressed) {
        //       main_menu_button_release_handler(ButtonID::P7);
        //       btn_P7_pressed = false;
        //     }
        //   }
        //   ImGui::SetCursorPos(ImVec2(340, 220));
        //   if (ImGui::Button("8", ImVec2(40, 30))) {
        //     main_menu_button_press_handler(ButtonID::P8);
        //     btn_P8_pressed = true;
        //   } else {
        //     if (btn_P8_pressed) {
        //       main_menu_button_release_handler(ButtonID::P8);
        //       btn_P8_pressed = false;
        //     }
    }

    ImGui::SetCursorPos(ImVec2(340, 270));
    ImGui::Button("9", ImVec2(40, 30));

    ImGui::SetCursorPos(ImVec2(60, 140));
    if (ImGui::Knob("Test123", &value, (ImTextureID)textureID))
    {
        float diff = value - lastValue;
        lastValue = value;

        /*if (current_page == PAGE_WB) {
      wb_page_knob_handler(E1_ROT, diff);
    }
    if (current_page == PAGE_WB_HELP) {
      wb_help_page_knob_handler(E1_ROT, diff);
    }
    if (current_menu != MENU_NONE) {
      main_menu_knob_handler(E1_ROT, diff);
    }*/
    }
    ImGui::PopStyleColor();
    ImGui::End();
    ImGui::EndFrame();

    ImGui::Render();
}
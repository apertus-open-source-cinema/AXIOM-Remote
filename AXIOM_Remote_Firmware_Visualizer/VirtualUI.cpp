#include "VirtualUI.h"

#include <iostream>

// Additional controls, like knob
#include "imgui_modules/imgui_user.h"

#include "UI/ButtonDefinitions.h"

uint8_t value = 0;
uint8_t lastValue = 0;

bool RenderButton(std::string name, uint16_t x, uint16_t y, uint16_t width = 40, uint16_t height = 30)
{
    ImGui::SetCursorPos(ImVec2(x, y));
    return ImGui::Button(name.c_str(), ImVec2(width, height));
}

// Grabbed from the ImGui examples
void ShowZoomTooltip(const ImGuiIO& io, ImTextureID displayTextureID)
{
    ImVec2 pos = ImGui::GetCursorScreenPos();
    int16_t textureWidth = 320;
    int16_t textureHeight = 240;
    ImGui::Image(displayTextureID, ImVec2(textureWidth, textureHeight), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                 ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        float region_sz = 48.0f;
        float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
        if (region_x < 0.0f)
            region_x = 0.0f;
        else if (region_x > textureWidth - region_sz)
            region_x = textureWidth - region_sz;
        float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
        if (region_y < 0.0f)
            region_y = 0.0f;
        else if (region_y > textureHeight - region_sz)
            region_y = textureHeight - region_sz;
        float zoom = 4.0f;
        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
        ImVec2 uv0 = ImVec2((region_x) / textureWidth, (region_y) / textureHeight);
        ImVec2 uv1 = ImVec2((region_x + region_sz) / textureWidth, (region_y + region_sz) / textureHeight);
        ImGui::Image(displayTextureID, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1,
                     ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
        ImGui::EndTooltip();
    }
}

void RenderUI(SDL_Window* window, const ImGuiIO& io, ImTextureID knobTextureID, ImTextureID displayTextureID, Button& button)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);

    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(800, 480));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImU32)ImColor(96, 96, 96, 255));
    ImGui::Begin("Image", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    if (RenderButton("1", 434, 70))
    {
        button = Button::BUTTON_1_UP;
    }
    if (RenderButton("2", 540, 70))
    {
        button = Button::BUTTON_2_UP;
    }
    if (RenderButton("3", 650, 70))
    {
        button = Button::BUTTON_3_UP;
    }

    if (RenderButton("4", 435, 380))
    {
        button = Button::BUTTON_4_UP;
    }
    if (RenderButton("5", 540, 380))
    {
        button = Button::BUTTON_5_UP;
    }
    if (RenderButton("6", 650, 380))
    {
        button = Button::BUTTON_6_UP;
    }

    if (RenderButton("7", 340, 170))
    {
        button = Button::BUTTON_7_UP;
    }
    if (RenderButton("8", 340, 220))
    {
        button = Button::BUTTON_8_UP;
    }
    if (RenderButton("9", 340, 270))
    {
        button = Button::BUTTON_9_UP;
    }

    if (RenderButton("10", 740, 170))
    {
        button = Button::BUTTON_10_UP;
    }
    if (RenderButton("11", 740, 220))
    {
        button = Button::BUTTON_11_UP;
    }
    if (RenderButton("12", 740, 270))
    {
        button = Button::BUTTON_12_UP;
    }

    // ImGui::SetCursorPos(ImVec2(450, 70));
    // ImGui::Button("1", ImVec2(40, 30));
    // ImGui::SetCursorPos(ImVec2(530, 70));
    // ImGui::Button("2", ImVec2(40, 30));
    // ImGui::SetCursorPos(ImVec2(610, 70));
    // ImGui::Button("3", ImVec2(40, 30));

    // ImGui::SetCursorPos(ImVec2(450, 380));
    // ImGui::Button("4", ImVec2(40, 30));
    // ImGui::SetCursorPos(ImVec2(530, 380));
    // ImGui::Button("5", ImVec2(40, 30));
    // ImGui::SetCursorPos(ImVec2(610, 380));
    // ImGui::Button("6", ImVec2(40, 30));

    /*ImGui::SetCursorPos(ImVec2(740, 170));
    ImGui::Button("10", ImVec2(40, 30));
    ImGui::SetCursorPos(ImVec2(740, 220));
    ImGui::Button("11", ImVec2(40, 30));
    ImGui::SetCursorPos(ImVec2(740, 270));
    ImGui::Button("12", ImVec2(40, 30));*/

    /*ImGui::SetCursorPos(ImVec2(340, 170));
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
    ImGui::Button("9", ImVec2(40, 30));*/

    ImGui::SetCursorPos(ImVec2(60, 140));
    if (ImGui::Knob("Test123", &value, (ImTextureID)knobTextureID))
    {
        /*float diff = value - lastValue;
        lastValue = value;*/

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

    ImGui::SetCursorPos(ImVec2(400, 120));
    ShowZoomTooltip(io, displayTextureID);

    ImGui::PopStyleColor();
    ImGui::End();
    ImGui::EndFrame();

    ImGui::Render();
}
#ifndef VIRTUALUI_H
#define VIRTUALUI_H

#include <stdint.h>
#include <string>

#include "imgui.h"

#include "imgui_modules/imgui_impl_opengl3.h"
#include "imgui_modules/imgui_impl_sdl.h"

enum class Button;

class VirtualUI {
    SDL_Window* _window;
    const ImGuiIO& _io;

    uint32_t _knobTextureID;
    ImTextureID _displayTextureID;
    ImTextureID _backgroundTextureID;
    
    ImTextureID _buttonTextureID;
    ImTextureID _buttonPressedTextureID;
    ImTextureID _buttonRoundTextureID;
    ImTextureID _buttonRoundPressedTextureID;

    uint32_t _cameraPreviewTextureID;

    uint32_t _programID;

    void ShowZoomTooltip();

    uint32_t _vertexbuffer;
    uint32_t _cameraFBO;
    uint32_t _fboTextureID;

    uint32_t _cameraPreviewTexture;

    float _analogGainShader;

    // Shader helper
    uint32_t LoadShader(std::string shaderFilePath, uint32_t shaderID);
    void CompileShader();
    void ShowShaderLog(uint32_t shaderID);

    void CreateFBO();

    void RenderCameraPreviewToFBO();
    void RenderVirtualCamera();

  public:
    VirtualUI(SDL_Window* window, uint32_t displayTextureID, uint32_t backgroundTextureID);

    void RenderUI(Button& button, int8_t& knobValue, bool& debugOverlayEnabled);
};

#endif // VIRTUALUI_H

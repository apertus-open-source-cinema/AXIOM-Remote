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

    ImTextureID _knobTextureID;
    ImTextureID _displayTextureID;
    ImTextureID _backgroundTextureID;

    ImTextureID _buttonTextureID;
    ImTextureID _buttonPressedTextureID;
    ImTextureID _buttonRoundTextureID;
    ImTextureID _buttonRoundPressedTextureID;
    ImTextureID _buttonPhotoTextureID;
    ImTextureID _buttonPhotoPressedTextureID;
    ImTextureID _buttonRecordTextureID;
    ImTextureID _buttonRecordPressedTextureID;
    ImTextureID _ledTextureID;
    ImTextureID _ledGlowTextureID;

    uint32_t _cameraPreviewTextureID;

    uint32_t _programID;

    void ShowZoomTooltip();

    uint32_t _vertexbuffer;
    uint32_t _cameraFBO;
    uint32_t _fboTextureID;
    uint32_t _fboDisplayTextureID;

    uint32_t _cameraPreviewTexture;

    float _analogGainShader;

    // Shader helper
    uint32_t LoadShader(std::string shaderFilePath, uint32_t shaderID);
    void CompileShader();
    void ShowShaderLog(uint32_t shaderID);

    void LoadTextures();

    void CreateFBO();

    void RenderCameraPreviewToFBO() const;
    void RenderVirtualCamera();

    void RenderKnob(int8_t& knobValue, Button& button);
    void RenderButtons(Button& button);
    void RenderLED(int8_t glowValue);

  public:
    VirtualUI(SDL_Window* window, uint32_t displayTextureID);

    void RenderUI(Button& button, int8_t& knobValue, bool& debugOverlayEnabled);
    void RenderDisplayToFBO() const;
};

#endif // VIRTUALUI_H

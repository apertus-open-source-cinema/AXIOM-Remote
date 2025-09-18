#pragma once
#include "ButtonDefinitions.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include <glad/gl.h>

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl3.h"

#include <lvgl.h>
// #include <CentralDB/CentralDBObserver.h>

// #include <UI/ButtonDefinitions.h>

enum class ButtonID;

class VirtualUI {
  SDL_Window *_window;
  const ImGuiIO &_io;

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
  std::function<void()> _screenshotHandler;

  uint32_t _vertexBuffer;
  uint32_t _cameraFBO;
  uint32_t _fboTextureID;
  uint32_t _fboDisplayTextureID;

  uint32_t _cameraPreviewTexture;

  float _analogGainShader;
  float _brightnessFactor;
  float _contrastFactor;

  // CentralDB* _db;
  // std::shared_ptr<CentralDBObserver> lcdObserver;

  // Shader helper
  uint32_t LoadShader(std::string shaderFilePath, uint32_t shaderID);
  void CompileShader();
  void ShowShaderLog(uint32_t shaderID);

  void LoadTextures();
  ImTextureID LoadTexture(const std::string &filePath);

  void CreateFBO();

  void RenderCameraPreviewToFBO() const;

  void RenderKnob(int8_t &knobValue, ButtonID &button);
  void RenderButtons(ButtonID &button);
  void RenderLED(int8_t glowValue);

  void RenderOverlay() const;
  void RenderVirtualCamera() const;
  void CompileShaders();
  uint32_t LoadSingleShader(const std::string &shaderFilePath,
                            GLenum shaderType);

  using ButtonClickHandler = std::function<void(ButtonID, ButtonState)>;
  ButtonClickHandler _buttonClickHandler;
  void RenderAndHandleSimButton(const char *label_id, ButtonID id);

public:
  VirtualUI(
        SDL_Window* window, uint32_t displayTextureID/*,
        std::function<void()> screenshotHandler = []() {}*/);

  void Render(ButtonID &button, int8_t &knobValue, bool &debugOverlayEnabled);

  void RenderDisplayToFBO() const;
  // void RenderDisplay() const;

  void ToggleLCDContrast(bool toggleContrastEnabled) const;
  void SetupVBO();

  void SetButtonClickHandler(ButtonClickHandler handler);
};

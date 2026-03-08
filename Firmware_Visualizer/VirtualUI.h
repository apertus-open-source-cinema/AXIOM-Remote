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

  // Shader uniform locations (GLint, not float)
  GLint _cameraPreviewTextureUniform;
  GLint _analogGainUniform;
  GLint _brightnessFactorUniform;
  GLint _contrastFactorUniform;

  // UI state (moved from global variables)
  uint8_t _brightnessLevel = 16;
  uint8_t _lcdBrightness = 100;
  float _lcdContrast = 1.0f;
  bool _toggleContrast = false;
  int8_t _glowValue = 0;

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

  // Helper methods for rendering optimization
  void RenderButtonGrid(int startIdx, int count, int offsetX, int offsetY,
                        const ImVec2& size, bool isRound = false);
  void SetShaderUniforms(GLint textureHandle, float brightness, float contrast) const;
  void RenderFullScreenQuad() const;
  void RenderToFBO(uint32_t fboHandle, uint32_t textureHandle, 
                   const ImVec2& viewportSize, float brightness, float contrast) const;

  using ButtonClickHandler = std::function<void(ButtonID, ButtonState)>;
  ButtonClickHandler _buttonClickHandler;
  void RenderAndHandleSimButton(const char *label_id, ButtonID id, bool isSpecial = false);

public:
  VirtualUI(
        SDL_Window* window, uint32_t displayTextureID/*,
        std::function<void()> screenshotHandler = []() {}*/);

  void Render(ButtonID &button, int8_t &knobValue, bool &debugOverlayEnabled);

  void RenderDisplayToFBO() const;

  void ToggleLCDContrast(bool toggleContrastEnabled);
  void SetupVBO();

  void SetButtonClickHandler(ButtonClickHandler handler);
};

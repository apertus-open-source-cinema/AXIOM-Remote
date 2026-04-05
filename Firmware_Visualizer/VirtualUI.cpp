#include "VirtualUI.h"

#include <cmath> // For M_PI
#include <fstream>
#include <iostream>
#include <vector>

#include <SDL3_image/SDL_image.h>

#include "HelpersGL.h"
#include "imgui.h"
#include "imgui_modules/imgui_user.h"

#include <ButtonDefinitions.h>
#include "UIConstants.h"

VirtualUI::VirtualUI(SDL_Window *window, uint32_t displayTextureID)
    : _window(window), _io(ImGui::GetIO()),
      _displayTextureID(static_cast<ImTextureID>(displayTextureID)) {
  LoadTextures();
  CreateFBO();
  SetupVBO();
  CompileShaders(); // Renamed to be plural as we compile two
}

void VirtualUI::SetupVBO() {
  uint32_t vertexArrayID;
  glGenVertexArrays(1, &vertexArrayID);
  glBindVertexArray(vertexArrayID);
  static const float vertexBufferData[] = {
      -1.0f, -1.0f, 0.0f, //
      1.0f,  -1.0f, 0.0f, //
      -1.0f, 1.0f,  0.0f, //
      -1.0f, 1.0f,  0.0f, //
      1.0f,  -1.0f, 0.0f, //
      1.0f,  1.0f,  0.0f, //
  };
  glGenBuffers(1, &_vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData,
               GL_STATIC_DRAW);
}

ImTextureID VirtualUI::LoadTexture(const std::string &filePath) {
  SDL_Surface *surface = IMG_Load(filePath.c_str());
  if (!surface) {
    std::cerr << "Failed to load texture: " << filePath << " - "
              << SDL_GetError() << std::endl;
    exit(1); // Or handle error as needed
  }
  ImTextureID textureID = CreateGLTextureFromSurface(surface);
  SDL_DestroySurface(surface);
  return textureID;
}

void VirtualUI::LoadTextures() {
  _backgroundTextureID = LoadTexture("./images/enclosure.png");
  _knobTextureID = LoadTexture("images/knob.png");
  _buttonTextureID = LoadTexture("images/button_normal.png");
  _buttonPressedTextureID = LoadTexture("images/button_pressed.png");
  _buttonRoundTextureID = LoadTexture("images/button_round_normal.png");
  _buttonRoundPressedTextureID = LoadTexture("images/button_round_pressed.png");
  _buttonPhotoTextureID = LoadTexture("images/photo_button_normal.png");
  _buttonPhotoPressedTextureID = LoadTexture("images/photo_button_pressed.png");
  _buttonRecordTextureID = LoadTexture("images/record_button_normal.png");
  _buttonRecordPressedTextureID =
      LoadTexture("images/record_button_pressed.png");
  _ledTextureID = LoadTexture("images/LED_off.png");
  _ledGlowTextureID = LoadTexture("images/LED_glow.png");
  _cameraPreviewTextureID = LoadTexture("images/camera_preview.png");
}

void VirtualUI::CreateFBO() {
  glGenFramebuffers(1, &_cameraFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, _cameraFBO);

  _fboTextureID = CreateGLTexture(UIConstants::CameraFBOWidth,
                                  UIConstants::CameraFBOHeight);
  _fboDisplayTextureID = CreateGLTexture(UIConstants::DisplayFBOWidth,
                                         UIConstants::DisplayFBOHeight, nullptr,
                                         GL_RGB, GL_NEAREST);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _fboTextureID, 0);

  uint32_t drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawBuffers);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "FBO initialization failed" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind FBO after setup
}

void VirtualUI::SetButtonClickHandler(ButtonClickHandler buttonClickHandler) {
  _buttonClickHandler = buttonClickHandler;
}

void VirtualUI::SetKnobHandler(KnobHandler knobHandler) {
  _knobHandler = knobHandler;
}

void VirtualUI::SetDebugToggleHandler(DebugToggleHandler debugToggleHandler) {
  _debugToggleHandler = debugToggleHandler;
}

uint32_t VirtualUI::LoadSingleShader(const std::string &shaderFilePath,
                                     GLenum shaderType) {
  uint32_t shaderID = glCreateShader(shaderType);
  std::ifstream shaderStream(shaderFilePath, std::ios::in);
  std::string shaderCode((std::istreambuf_iterator<char>(shaderStream)),
                         std::istreambuf_iterator<char>());

  const char *sourcePointer = shaderCode.c_str();
  glShaderSource(shaderID, 1, &sourcePointer, nullptr);
  glCompileShader(shaderID);

  ShowShaderLog(shaderID);
  return shaderID;
}

void VirtualUI::CompileShaders() {
  uint32_t vertexShaderID =
      LoadSingleShader("shaders/camera_preview.vert", GL_VERTEX_SHADER);
  uint32_t fragmentShaderID =
      LoadSingleShader("shaders/camera_preview.frag", GL_FRAGMENT_SHADER);

  _programID = glCreateProgram();
  glAttachShader(_programID, vertexShaderID);
  glAttachShader(_programID, fragmentShaderID);
  glLinkProgram(_programID);

  GLint success = GL_TRUE;
  glValidateProgram(_programID);
  glGetProgramiv(_programID, GL_VALIDATE_STATUS, &success);
  if (success == GL_FALSE) {
    GLchar errorLog[1024] = {0};
    glGetProgramInfoLog(_programID, 1024, nullptr, errorLog);
    std::cerr << "Error validating shader program: " << errorLog << std::endl;
  }

  _cameraPreviewTextureUniform =
      glGetUniformLocation(_programID, "cameraPreviewTexture");
  _analogGainUniform = glGetUniformLocation(_programID, "analogGain");
  _brightnessFactorUniform =
      glGetUniformLocation(_programID, "brightnessFactor");
  _contrastFactorUniform = glGetUniformLocation(_programID, "contrastFactor");

  // Clean up shader objects after linking
  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);
}

void VirtualUI::ShowShaderLog(uint32_t shaderID) {
  GLint result = GL_FALSE;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
  GLint infoLogLength = 0;
  glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
  if (infoLogLength > 0) {
    std::vector<char> shaderErrorMessage(infoLogLength + 1);
    glGetShaderInfoLog(shaderID, infoLogLength, nullptr,
                       shaderErrorMessage.data());
    std::cerr << "Shader compilation log:\n"
              << std::string(begin(shaderErrorMessage), end(shaderErrorMessage))
              << std::endl;
  }
}

void VirtualUI::ToggleLCDContrast(bool toggleContrastEnabled) {
  _lcdContrast = toggleContrastEnabled
                     ? UIConstants::DEFAULT_LCD_CONTRAST_REDUCED
                     : UIConstants::DEFAULT_LCD_CONTRAST;
}

void VirtualUI::SetShaderUniforms(GLint textureHandle, float brightness,
                                  float contrast) const {
  glUniform1i(_cameraPreviewTextureUniform, 0);
  glUniform1f(_contrastFactorUniform, contrast);
  glUniform1f(_brightnessFactorUniform, brightness);
}

void VirtualUI::RenderFullScreenQuad() const {
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(0);
}

void VirtualUI::RenderToFBO(uint32_t fboHandle, uint32_t textureHandle,
                            const ImVec2 &viewportSize, float brightness,
                            float contrast) const {
  glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureHandle, 0);
  glViewport(0, 0, static_cast<GLsizei>(viewportSize.x),
             static_cast<GLsizei>(viewportSize.y));
  glUseProgram(_programID);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureHandle);
  SetShaderUniforms(0, brightness, contrast);

  RenderFullScreenQuad();

  glUseProgram(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VirtualUI::RenderDisplayToFBO() const {
  glBindFramebuffer(GL_FRAMEBUFFER, _cameraFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                       (GLuint)(intptr_t)_displayTextureID, 0);
  glViewport(0, 0, UIConstants::DisplayFBOWidth, UIConstants::DisplayFBOHeight);
  glUseProgram(_programID);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)_displayTextureID);

  float brightness = UIConstants::LCD_BRIGHTNESS_FACTOR * _lcdBrightness;
  SetShaderUniforms(0, brightness, _lcdContrast);

  RenderFullScreenQuad();

  glUseProgram(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VirtualUI::ShowZoomTooltip()const {
  ImVec2 pos = ImGui::GetCursorScreenPos();
  int16_t textureWidth = UIConstants::DisplayFBOWidth;
  int16_t textureHeight = UIConstants::DisplayFBOHeight;

  ImGui::Image(static_cast<ImTextureID>(_displayTextureID),
               ImVec2(textureWidth, textureHeight), ImVec2(0, 0), ImVec2(1, 1));

  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    float region_sz = UIConstants::CAMERA_PREVIEW_ZOOM_REGION;
    float region_x = _io.MousePos.x - pos.x - region_sz * 0.5f;
    region_x = ImClamp(region_x, 0.0f, (float)textureWidth - region_sz);
    float region_y = _io.MousePos.y - pos.y - region_sz * 0.5f;
    region_y = ImClamp(region_y, 0.0f, (float)textureHeight - region_sz);
    float zoom = UIConstants::CAMERA_PREVIEW_ZOOM;
    ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
    ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz,
                region_y + region_sz);
    ImVec2 uv0 = ImVec2(region_x / textureWidth, region_y / textureHeight);
    ImVec2 uv1 = ImVec2((region_x + region_sz) / textureWidth,
                        (region_y + region_sz) / textureHeight);
    ImGui::Image(static_cast<ImTextureID>(_displayTextureID),
                 ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1);
    ImGui::EndTooltip();
  }
}

void VirtualUI::RenderCameraPreviewToFBO() const {
  float brightness = UIConstants::CAMERA_BRIGHTNESS_FACTOR * _brightnessLevel;
  RenderToFBO(_cameraFBO, _fboTextureID,
              ImVec2(UIConstants::CameraFBOWidth, UIConstants::CameraFBOHeight),
              brightness, 1.0f);
}

void VirtualUI::RenderOverlay()  {
  ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImU32)ImColor(16, 16, 16, 196));
  ImGui::BeginChild("Parameters", UIConstants::OverlaySize, true);
  ImGui::SetWindowFontScale(1.5f);

  ImGui::SetCursorPos(ImVec2(UIConstants::OverlayTextOffsetX,
                             UIConstants::OverlayTextOffsetY +
                                 UIConstants::OverlayTextSpacingY));
  ImGui::Text("Analog\nGain\nN/A");

  ImGui::SetCursorPos(ImVec2(UIConstants::OverlayTextOffsetX,
                             UIConstants::OverlayTextOffsetY +
                                 2 * UIConstants::OverlayTextSpacingY));
  ImGui::Text("Digital\nGain\nN/A");

  ImGui::PopStyleColor();
  ImGui::EndChild();
}

void VirtualUI::RenderVirtualCamera() const {
  RenderCameraPreviewToFBO();

  ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
  ImGui::SetNextWindowPos(ImVec2(0, UIConstants::CameraFBOHeight));
  ImGui::SetNextWindowSize(
      ImVec2(UIConstants::CameraFBOWidth, UIConstants::CameraFBOHeight));

  ImGui::Begin("Image2", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

  ImGui::Image(
      static_cast<ImTextureID>(_fboTextureID),
      ImVec2(UIConstants::CameraFBOWidth, UIConstants::CameraFBOHeight),
      ImVec2(0, 0), ImVec2(1, 1));

  ImGui::SetCursorPos(UIConstants::OverlayPosition);
  RenderOverlay();

  ImGui::PopStyleVar();
  ImGui::End();
}

void VirtualUI::RenderKnob(int8_t &knobValue, ButtonID &button) {
  ImGui::SetCursorPos(UIConstants::KnobPosition);

  static float knob_angle = 0.0f;
  static bool prev_knob_pressed = false;
  bool knob_pressed = false;

  int tick_delta = ImGui::KnobEncoder(
      "Knob", &knob_angle, &knob_pressed, "Press",
      ImVec2(UIConstants::KnobDiameter, UIConstants::KnobDiameter),
      _knobTextureID, IM_COL32_WHITE);

  // Check for mouse wheel over the knob
  if (ImGui::IsItemHovered()) {
    float wheel = ImGui::GetIO().MouseWheel;
    if (wheel != 0.0f) {
      tick_delta += static_cast<int>(wheel);
    }
  }

  if (tick_delta != 0) {
    knobValue = tick_delta;
    _brightnessLevel += tick_delta;
    if (_knobHandler) {
      _knobHandler(tick_delta);
    }
  } else {
    knobValue = 0;
  }

  // Handle knob press state transitions with ButtonState
  if (knob_pressed && !prev_knob_pressed) {
    if (_buttonClickHandler) {
      _buttonClickHandler(ButtonID::KNOB, ButtonState::Pressed);
    }
    button = ButtonID::KNOB;
  } else if (!knob_pressed && prev_knob_pressed) {
    if (_buttonClickHandler) {
      _buttonClickHandler(ButtonID::KNOB, ButtonState::Released);
    }
  }

  prev_knob_pressed = knob_pressed;
}

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

void VirtualUI::RenderAndHandleSimButton(const char *label_id, ButtonID id,
                                         bool isSpecial) {
  if (isSpecial) {
    ImGui::CustomImageButton(
        label_id, _buttonTextureID, _buttonPressedTextureID,
        ImVec2(UIConstants::SmallButtonWidth, UIConstants::SmallButtonHeight));
  } else {
    ImGui::CustomImageButton(
        label_id, _buttonTextureID, _buttonPressedTextureID,
        ImVec2(UIConstants::SmallButtonWidth, UIConstants::SmallButtonHeight));
  }

  if (ImGui::IsItemActivated()) {
    if (_buttonClickHandler) {
      _buttonClickHandler(id, ButtonState::Pressed);
    }
  }
  if (ImGui::IsItemDeactivated()) {
    if (_buttonClickHandler) {
      _buttonClickHandler(id, ButtonState::Released);
    }
  }
}

void VirtualUI::RenderButtonGrid(int startIdx, int count, int offsetX,
                                 int offsetY, const ImVec2 &size,
                                 bool isRound) {
  const ImTextureID normalTex =
      isRound ? _buttonRoundTextureID : _buttonTextureID;
  const ImTextureID pressedTex =
      isRound ? _buttonRoundPressedTextureID : _buttonPressedTextureID;
  const ImVec2 buttonSize = isRound ? ImVec2(size)
                                    : ImVec2(UIConstants::SmallButtonWidth,
                                             UIConstants::SmallButtonHeight);
  const int spacingX = isRound ? 0 : UIConstants::ButtonSpacingX;
  const int spacingY = isRound ? UIConstants::RoundButtonSpacingY : 0;

  for (int i = 0; i < count; ++i) {
    int x = offsetX + (spacingX > 0 ? i * spacingX : 0);
    int y = offsetY + (spacingY > 0 ? i * spacingY : 0);
    ImGui::SetCursorPos(ImVec2(static_cast<float>(x), static_cast<float>(y)));

    const char *buttonLabel = UIConstants::buttonIDs[startIdx + i];
    ButtonID buttonID =
        static_cast<ButtonID>(to_underlying(ButtonID::BUTTON_1) + startIdx + i);

    ImGui::CustomImageButton(buttonLabel, normalTex, pressedTex, buttonSize);

    if (ImGui::IsItemActivated()) {
      if (_buttonClickHandler) {
        _buttonClickHandler(buttonID, ButtonState::Pressed);
      }
    }
    if (ImGui::IsItemDeactivated()) {
      if (_buttonClickHandler) {
        _buttonClickHandler(buttonID, ButtonState::Released);
      }
    }
  }
}

void VirtualUI::RenderButtons(ButtonID &button) {
  // Special buttons (top-left and top-center)
  ImGui::SetCursorPos(UIConstants::SpecialButton1Position);
  RenderAndHandleSimButton("SPECIAL_1", ButtonID::BUTTON_A, true);

  ImGui::SetCursorPos(UIConstants::SpecialButton2Position);
  RenderAndHandleSimButton("SPECIAL_2", ButtonID::BUTTON_B, true);

  // Photo button
  ImGui::SetCursorPos(UIConstants::PhotoButtonPosition);
  ImGui::CustomImageButton(
      "PHOTO", _buttonPhotoTextureID, _buttonPhotoPressedTextureID,
      ImVec2(UIConstants::PhotoButtonWidth, UIConstants::PhotoButtonHeight));
  if (ImGui::IsItemActivated()) {
    if (_buttonClickHandler) {
      _buttonClickHandler(ButtonID::BUTTON_PHOTO, ButtonState::Pressed);
    }
  }
  if (ImGui::IsItemDeactivated()) {
    if (_buttonClickHandler) {
      _buttonClickHandler(ButtonID::BUTTON_PHOTO, ButtonState::Released);
    }
  }

  // Record button
  ImGui::SetCursorPos(UIConstants::RecordButtonPosition);
  ImGui::CustomImageButton(
      "RECORD", _buttonRecordTextureID, _buttonRecordPressedTextureID,
      ImVec2(UIConstants::RecordButtonWidth, UIConstants::RecordButtonHeight));
  if (ImGui::IsItemActivated()) {
    if (_buttonClickHandler) {
      _buttonClickHandler(ButtonID::BUTTON_RECORD, ButtonState::Pressed);
    }
  }
  if (ImGui::IsItemDeactivated()) {
    if (_buttonClickHandler) {
      _buttonClickHandler(ButtonID::BUTTON_RECORD, ButtonState::Released);
    }
  }

  // Buttons 1-3 (rectangular buttons, top row)
  RenderButtonGrid(
      0, 3, UIConstants::BUTTONS_1_3_X, UIConstants::Buttons1_3_OffsetY,
      ImVec2(UIConstants::SmallButtonWidth, UIConstants::SmallButtonHeight),
      false);

  // Buttons 4-6 (rectangular buttons, bottom row)
  RenderButtonGrid(
      3, 3, UIConstants::BUTTONS_1_3_X, UIConstants::Buttons4_6_OffsetY,
      ImVec2(UIConstants::SmallButtonWidth, UIConstants::SmallButtonHeight),
      false);

  // Buttons 7-9 (round buttons, left column)
  RenderButtonGrid(6, 3, UIConstants::Buttons7_9_OffsetX,
                   UIConstants::BUTTONS_7_9_GRID_Y_START,
                   UIConstants::RoundButtonSize, true);

  // Buttons 10-12 (round buttons, right column)
  RenderButtonGrid(9, 3, UIConstants::Buttons10_12_OffsetX,
                   UIConstants::BUTTONS_7_9_GRID_Y_START,
                   UIConstants::RoundButtonSize, true);
}

void EnableBlending(const ImDrawList *parent_list, const ImDrawCmd *cmd) {
  glBlendFunc(GL_ONE, GL_ONE);
}

void DisableBlending(const ImDrawList *parent_list, const ImDrawCmd *cmd) {
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Render RGB LEDs
void VirtualUI::RenderLED(int8_t glowValue) {
  // Render off state
  ImGui::SetCursorPos(UIConstants::LEDTopPosition);
  ImGui::Image(_ledTextureID, UIConstants::LEDSize);

  ImGui::SetCursorPos(UIConstants::LEDBottomPosition);
  ImGui::Image(_ledTextureID, UIConstants::LEDSize);

  // Render LED light
  ImGui::GetWindowDrawList()->ImDrawList::AddCallback(EnableBlending, nullptr);

  ImGui::SetCursorPos(UIConstants::LEDTopPosition - UIConstants::LEDGlowOffset * 0.5f);
  ImGui::ImageWithBg(_ledGlowTextureID, UIConstants::LEDGlowSize, ImVec2(0, 0),
                     ImVec2(1, 1), ImColor(0, 0, 0, 0),
                     ImColor(255, 32, 32, 255 - glowValue));

  ImGui::SetCursorPos(UIConstants::LEDBottomPosition - UIConstants::LEDGlowOffset * 0.5f);
  ImGui::ImageWithBg(_ledGlowTextureID, UIConstants::LEDGlowSize, ImVec2(0, 0),
                     ImVec2(1, 1), ImColor(0, 0, 0, 0),
                     ImColor(64, 64, 255, 255 - glowValue));

  ImGui::GetWindowDrawList()->ImDrawList::AddCallback(DisableBlending, nullptr);
}

void VirtualUI::Render(ButtonID &button, int8_t &knobValue,
                       bool &debugOverlayEnabled) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();

  ImGui::NewFrame();

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(
      ImVec2(UIConstants::CameraFBOWidth, UIConstants::CameraFBOHeight));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImU32)ImColor(0, 0, 0, 255));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Image", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

  ImGui::SetCursorPos(ImVec2(0, 0));
  ImGui::Image(_backgroundTextureID, ImVec2(UIConstants::CameraFBOWidth,
                                            UIConstants::CameraFBOHeight));

  RenderKnob(knobValue, button);
  RenderButtons(button);

  _glowValue += knobValue;
  RenderLED(_glowValue);

  ImGui::SetCursorPos(UIConstants::DebugTogglePosition);
  bool oldDebugState = debugOverlayEnabled;
  ImGui::ToggleButton("debug_overlay_switch", "Debug overlay", &debugOverlayEnabled);

  if (oldDebugState != debugOverlayEnabled && _debugToggleHandler) {
      _debugToggleHandler(debugOverlayEnabled);
  }

  ImGui::SetCursorPos(UIConstants::ContrastTogglePosition);
  ImGui::ToggleButton("toggle_contrast_switch", "Simulate LCD", &_toggleContrast);
  ToggleLCDContrast(_toggleContrast);

  ImGui::SetCursorPos(UIConstants::CameraPreviewPosition);
  ShowZoomTooltip();

  ImGui::SetCursorPos(UIConstants::ScreenshotButtonPosition);
  if (ImGui::Button("Take Screenshot") && _screenshotHandler) {
    _screenshotHandler();
  }

  ImGui::PopStyleVar();
  ImGui::PopStyleColor();

  ImGui::End();
  RenderVirtualCamera();

  ImGui::EndFrame();

  ImGui::Render();
}

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

namespace UIConstants {
const float FullCircleRad = 2 * M_PI;
const float Tick = FullCircleRad / 12;
const float HalfPi = static_cast<float>(M_PI) / 2.0f;

// Knob dimensions (diameter and offsets can be ints for pixel placement)
const int KnobDiameter = 200;
const float KnobRadius =
    KnobDiameter * 0.5f; // Radius might be used in float calculations
const int KnobPressableDiameter = static_cast<int>(0.3f * KnobDiameter);
const float KnobPressableRadiusSquared =
    0.25f * KnobPressableDiameter * KnobPressableDiameter;
const ImVec2 KnobPressableSize =
    ImVec2(static_cast<float>(KnobPressableDiameter),
           static_cast<float>(KnobPressableDiameter));
const float KnobPressableOffset =
    0.50f * (KnobDiameter - KnobPressableDiameter);
const int KnobValueTextOffset = 200;

// Button dimensions
const int SmallButtonWidth = 46;
const int SmallButtonHeight = 30;
const int RoundButtonDiameter = 30;
const ImVec2 RoundButtonSize = ImVec2(static_cast<float>(RoundButtonDiameter),
                                      static_cast<float>(RoundButtonDiameter));
const int PhotoButtonWidth = 46;
const int PhotoButtonHeight = 46;
const int RecordButtonWidth = 68;
const int RecordButtonHeight = 68;

// LED dimensions and offsets
const ImVec2 LEDSize =
    ImVec2(30.0f, 29.0f); // Size could remain float if used with ImGui::Image
const ImVec2 LEDGlowSize = ImVec2(40.0f, 40.0f); // Size could remain float
const ImVec2 LEDGlowOffset = ImVec2(5.0f, 5.0f); // Offset could be int

// Virtual camera FBO dimensions
const int CameraFBOWidth = 800;
const int CameraFBOHeight = 480;
const int DisplayFBOWidth = 320;
const int DisplayFBOHeight = 240;

// Overlay dimensions and positioning
const ImVec2 OverlaySize = ImVec2(120.0f, 400.0f); // Size could remain float
const ImVec2 OverlayPosition =
    ImVec2(650.0f, 30.0f); // Position needs to be float
const int OverlayTextOffsetX = 20;
const int OverlayTextOffsetY = 20;
const int OverlayTextSpacingY = 100;

// UI element positions (these need to be ImVec2 with float components)
const ImVec2 KnobPosition = ImVec2(40.0f, 140.0f);
const ImVec2 SpecialButton1Position = ImVec2(55.0f, 22.0f);
const ImVec2 SpecialButton2Position = ImVec2(148.0f, 22.0f);
const ImVec2 PhotoButtonPosition = ImVec2(236.0f, 367.0f);
const ImVec2 RecordButtonPosition = ImVec2(225.0f, 56.0f);
const int ButtonSpacingX = 103;
const int RoundButtonSpacingY = 55;
const ImVec2 LEDTopPosition = ImVec2(63.0f, 70.0f);
const ImVec2 LEDBottomPosition = ImVec2(63.0f, 109.0f);
const ImVec2 CameraPreviewPosition = ImVec2(338.0f, 120.0f);
const ImVec2 DebugTogglePosition = ImVec2(50.0f, 375.0f);
const ImVec2 ContrastTogglePosition = ImVec2(50.0f, 410.0f);
const ImVec2 ScreenshotButtonPosition = ImVec2(50.0f, 445.0f);

// These are offsets/spacing
const int Buttons1_3_OffsetY = 55;
const int Buttons4_6_OffsetY = 395;
const int Buttons7_9_OffsetX = 249;
const int Buttons10_12_OffsetX = 717;

const char *buttonIDs[12] = {"1", "2", "3", "4",  "5",  "6",
                             "7", "8", "9", "10", "11", "12"};
} // namespace UIConstants

uint8_t brightnessLevel = 16;
uint8_t lcdBrightness = 100;
float lcdContrast = 1.0;
bool toggleContrast = false;

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

  _cameraPreviewTexture =
      glGetUniformLocation(_programID, "cameraPreviewTexture");
  _analogGainShader = glGetUniformLocation(_programID, "analogGain");
  _brightnessFactor = glGetUniformLocation(_programID, "brightnessFactor");
  _contrastFactor = glGetUniformLocation(_programID, "contrastFactor");

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

void VirtualUI::ToggleLCDContrast(bool toggleContrastEnabled) const {
  lcdContrast = toggleContrastEnabled ? 0.7f : 1.0f;
}

void VirtualUI::RenderDisplayToFBO() const {
  glBindFramebuffer(GL_FRAMEBUFFER, _cameraFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _displayTextureID,
                       0);
  glViewport(0, 0, UIConstants::DisplayFBOWidth, UIConstants::DisplayFBOHeight);
  glUseProgram(_programID);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)_displayTextureID);
  glUniform1i(_cameraPreviewTexture, 0);

  float brightness = 1.0f / 100.0f * lcdBrightness;
  glUniform1f(_contrastFactor, lcdContrast);
  glUniform1f(_brightnessFactor, brightness);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(0);

  glUseProgram(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// void VirtualUI::RenderDisplay() const {
//   // glBindFramebuffer(GL_FRAMEBUFFER, _cameraFBO);
//   // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
//   // _displayTextureID,
//   //                      0);
//   // glViewport(0, 0, UIConstants::DisplayFBOWidth,
//   // UIConstants::DisplayFBOHeight);
//   glUseProgram(3);

//   glActiveTexture(GL_TEXTURE0);
//   glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)_displayTextureID);
//   glUniform1i(_cameraPreviewTexture, 0);

//   float brightness = 1.0f / 100.0f * lcdBrightness;
//   glUniform1f(_contrastFactor, lcdContrast);
//   glUniform1f(_brightnessFactor, brightness);

//   ImGui::Image(static_cast<ImTextureID>(_displayTextureID), ImVec2(320, 240),
//                ImVec2(0, 0), ImVec2(1, 1));

//   glUseProgram(0);
//   // glBindFramebuffer(GL_FRAMEBUFFER, 0);
// }

void VirtualUI::ShowZoomTooltip() {
  ImVec2 pos = ImGui::GetCursorScreenPos();
  int16_t textureWidth = UIConstants::DisplayFBOWidth;
  int16_t textureHeight = UIConstants::DisplayFBOHeight;

  // RenderDisplay();

  ImGui::Image(static_cast<ImTextureID>(_displayTextureID),
               ImVec2(textureWidth, textureHeight), ImVec2(0, 0), ImVec2(1, 1));

  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    float region_sz = 48.0f;
    float region_x = _io.MousePos.x - pos.x - region_sz * 0.5f;
    region_x = ImClamp(region_x, 0.0f, (float)textureWidth - region_sz);
    float region_y = _io.MousePos.y - pos.y - region_sz * 0.5f;
    region_y = ImClamp(region_y, 0.0f, (float)textureHeight - region_sz);
    float zoom = 4.0f;
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
  glBindFramebuffer(GL_FRAMEBUFFER, _cameraFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _fboTextureID, 0);
  glViewport(0, 0, UIConstants::CameraFBOWidth, UIConstants::CameraFBOHeight);
  glUseProgram(_programID);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _cameraPreviewTextureID);
  glUniform1i(_cameraPreviewTexture, 0);

  float brightness = 0.1f * brightnessLevel;
  glUniform1f(_brightnessFactor, brightness);
  glUniform1f(_contrastFactor, 1.0f);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(0);

  glUseProgram(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VirtualUI::RenderOverlay() const {
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

  int tick_delta = ImGui::KnobEncoder("Knob", &knob_angle, &knob_pressed, "Press",
                                      ImVec2(UIConstants::KnobDiameter, UIConstants::KnobDiameter), 
                                      _knobTextureID, IM_COL32_WHITE);

  if (tick_delta != 0) {
      knobValue = tick_delta;
      brightnessLevel += tick_delta; 
  } else {
      knobValue = 0;
  }

  // Handle knob press state transitions with ButtonState
  if (knob_pressed && !prev_knob_pressed) {
      // Knob pressed this frame
      if (_buttonClickHandler) {
          _buttonClickHandler(ButtonID::KNOB, ButtonState::Pressed);
      }
      button = ButtonID::KNOB;
  } else if (!knob_pressed && prev_knob_pressed) {
      // Knob released this frame
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

void VirtualUI::RenderAndHandleSimButton(const char *label_id, ButtonID id) {
  // Call the function that draws the button and sets up interaction
  // We don't necessarily need its bool return value here
  ImGui::CustomImageButton(
      label_id, _buttonTextureID, _buttonPressedTextureID,
      ImVec2(UIConstants::SmallButtonWidth, UIConstants::SmallButtonHeight));

  // Check status *immediately after* the call for the last item
  if (ImGui::IsItemActivated()) { // PRESSED this frame
    if (_buttonClickHandler) {
      _buttonClickHandler(id, ButtonState::Pressed);
    }
  }
  // Check for Deactivation (Release)
  if (ImGui::IsItemDeactivated()) { // RELEASED this frame
    if (_buttonClickHandler) {
      _buttonClickHandler(id, ButtonState::Released);
    }
  }
}

void VirtualUI::RenderButtons(ButtonID &button) {
  // Special buttons (top-left)
  ImGui::SetCursorPos(UIConstants::SpecialButton1Position);
  // if (ImGui::CustomImageButton("SPECIAL_1", _buttonTextureID,
  //                              _buttonPressedTextureID,
  //                              ImVec2(UIConstants::SmallButtonWidth,
  //                                     UIConstants::SmallButtonHeight))) {
  // }

  RenderAndHandleSimButton("SPECIAL_1", ButtonID::BUTTON_1);

  ImGui::SetCursorPos(UIConstants::SpecialButton2Position);
  if (ImGui::CustomImageButton("SPECIAL_2", _buttonTextureID,
                               _buttonPressedTextureID,
                               ImVec2(UIConstants::SmallButtonWidth,
                                      UIConstants::SmallButtonHeight))) {
  }

  // Button PHOTO
  ImGui::SetCursorPos(UIConstants::PhotoButtonPosition);
  if (ImGui::CustomImageButton("PHOTO", _buttonPhotoTextureID,
                               _buttonPhotoPressedTextureID,
                               ImVec2(UIConstants::PhotoButtonWidth,
                                      UIConstants::PhotoButtonHeight))) {
  }

  // Button RECORD
  ImGui::SetCursorPos(UIConstants::RecordButtonPosition);
  if (ImGui::CustomImageButton("RECORD", _buttonRecordTextureID,
                               _buttonRecordPressedTextureID,
                               ImVec2(UIConstants::RecordButtonWidth,
                                      UIConstants::RecordButtonHeight))) {
  }

  // Buttons 1-3
  for (int i = 0; i < 3; ++i) {
    ImGui::SetCursorPos(ImVec2((int)372.0f + i * UIConstants::ButtonSpacingX,
                               UIConstants::Buttons1_3_OffsetY));
    RenderAndHandleSimButton(
        UIConstants::buttonIDs[i],
        static_cast<ButtonID>(to_underlying(ButtonID::BUTTON_1) + i));
  }

  // Button 4-6
  for (int i = 0; i < 3; ++i) {
    ImGui::SetCursorPos(ImVec2(372.0f + i * UIConstants::ButtonSpacingX,
                               UIConstants::Buttons4_6_OffsetY));
    RenderAndHandleSimButton(
        UIConstants::buttonIDs[i + 3],
        static_cast<ButtonID>(to_underlying(ButtonID::BUTTON_4) + i));
  }

  // Button 7-9
  for (int i = 0; i < 3; ++i) {
    ImGui::SetCursorPos(ImVec2(UIConstants::Buttons7_9_OffsetX,
                               170.0f + i * UIConstants::RoundButtonSpacingY));
    if (ImGui::CustomImageButton(
            UIConstants::buttonIDs[i + 6], _buttonRoundTextureID,
            _buttonRoundPressedTextureID, UIConstants::RoundButtonSize)) {
      // button = static_cast<ButtonID>(ButtonID::BUTTON_7_UP + i);
      std::cout << "Button " << UIConstants::buttonIDs[i + 6] << std::endl;
    }
  }

  // Button 10-12
  for (int i = 0; i < 3; ++i) {
    ImGui::SetCursorPos(ImVec2(UIConstants::Buttons10_12_OffsetX,
                               170.0f + i * UIConstants::RoundButtonSpacingY));
    if (ImGui::CustomImageButton(
            UIConstants::buttonIDs[i + 9], _buttonRoundTextureID,
            _buttonRoundPressedTextureID, UIConstants::RoundButtonSize)) {
      // button = static_cast<ButtonID>(ButtonID::BUTTON_10_UP + i);
      std::cout << "Button " << UIConstants::buttonIDs[i + 9] << std::endl;
    }
  }
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

  ImGui::SetCursorPos(UIConstants::LEDTopPosition -
                      UIConstants::LEDGlowOffset * 0.5f);
  ImGui::Image(
      _ledGlowTextureID,
      UIConstants::LEDGlowSize); //, ImColor(255, 32, 32, 255 - glowValue)

  ImGui::SetCursorPos(UIConstants::LEDBottomPosition -
                      UIConstants::LEDGlowOffset * 0.5f);
  ImGui::Image(_ledGlowTextureID,
               UIConstants::LEDGlowSize); //, ImColor(64, 64, 255, glowValue)

  ImGui::GetWindowDrawList()->ImDrawList::AddCallback(DisableBlending, nullptr);
}

int glowValue = 0;

void VirtualUI::Render(ButtonID &button, int8_t &knobValue,
                       bool &debugOverlayEnabled
                       /*std::function<void()> screenshotHandler*/) {
  // _screenshotHandler = screenshotHandler; // Store the handler

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();

  ImGui::NewFrame();

  // RenderDisplayToFBO();

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

  glowValue += knobValue;
  RenderLED(glowValue);

  ImGui::SetCursorPos(UIConstants::DebugTogglePosition);
  ImGui::ToggleButton("debug_overlay_switch", "Debug overlay",
                      &debugOverlayEnabled);

  ToggleLCDContrast(toggleContrast);
  ImGui::SetCursorPos(UIConstants::ContrastTogglePosition);
  ImGui::ToggleButton("toggle_contrast_switch", "Simulate LCD",
                      &toggleContrast);

  // RenderDisplayToFBO();
  ImGui::SetCursorPos(UIConstants::CameraPreviewPosition);
  ShowZoomTooltip();

  ImGui::SetCursorPos(UIConstants::ScreenshotButtonPosition);
  if (ImGui::Button("Take Screenshot") && _screenshotHandler) {
    _screenshotHandler();
  }

  ImGui::PopStyleVar();
  ImGui::PopStyleColor();

  ImGui::End();

  // RenderVirtualCamera();

  ImGui::EndFrame();

  ImGui::Render();
}
#include "VirtualUI.h"

#include <fstream>
#include <iostream>
#include <vector>

#include <GL/gl3w.h>

#include <SDL2/SDL_image.h>

// Additional controls, like knob
#include "imgui_modules/imgui_user.h"

#include "UI/ButtonDefinitions.h"

#include <CentralDB/CentralDB.h>

#include "HelpersGL.h"

uint8_t value = 0;
uint8_t lastValue = 0;
uint8_t brightnessLevel = 16;
uint8_t lcdBrightness = 100;
float lcdContrast = 1.0;
bool toggleContrast = false;

VirtualUI::VirtualUI(SDL_Window* window, uint32_t displayTextureID, CentralDB* db,
                     std::function<void()> screenshotHandler) :
    _window(window),
    _io(ImGui::GetIO()), _displayTextureID(reinterpret_cast<ImTextureID>(displayTextureID)), _db(db),
    _screenshotHandler(screenshotHandler)
{
    LoadTextures();

    CreateFBO();
    SetupVBO();
    CompileShader();

    CompileShader();

    lcdObserver = std::make_shared<CentralDBObserver>(Attribute::ID::REMOTE_LCD_BRIGHTNESS, [](const CentralDB& db) {
        lcdBrightness = db.GetUint32(Attribute::ID::REMOTE_LCD_BRIGHTNESS);
        std::cout << "LCD brightness" << std::endl;
    });
    _db->Attach(lcdObserver.get());
}

void VirtualUI::SetupVBO()
{
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

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &_vertexBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);
}

void VirtualUI::LoadTextures()
{
    SDL_Surface* backgroundTexture = IMG_Load("images/enclosure.png");
    _backgroundTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(backgroundTexture));
    SDL_FreeSurface(backgroundTexture);

    SDL_Surface* surface = IMG_Load("images/knob.png");
    _knobTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(surface));
    SDL_FreeSurface(surface);

    SDL_Surface* buttonTexture = IMG_Load("images/button_normal.png");
    _buttonTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(buttonTexture));
    SDL_FreeSurface(buttonTexture);

    buttonTexture = IMG_Load("images/button_pressed.png");
    _buttonPressedTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(buttonTexture));
    SDL_FreeSurface(buttonTexture);

    buttonTexture = IMG_Load("images/button_round_normal.png");
    _buttonRoundTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(buttonTexture));
    SDL_FreeSurface(buttonTexture);

    buttonTexture = IMG_Load("images/button_round_pressed.png");
    _buttonRoundPressedTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(buttonTexture));
    SDL_FreeSurface(buttonTexture);

    buttonTexture = IMG_Load("images/photo_button_normal.png");
    _buttonPhotoTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(buttonTexture));
    SDL_FreeSurface(buttonTexture);

    buttonTexture = IMG_Load("images/photo_button_pressed.png");
    _buttonPhotoPressedTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(buttonTexture));
    SDL_FreeSurface(buttonTexture);

    buttonTexture = IMG_Load("images/record_button_normal.png");
    _buttonRecordTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(buttonTexture));
    SDL_FreeSurface(buttonTexture);

    buttonTexture = IMG_Load("images/record_button_pressed.png");
    _buttonRecordPressedTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(buttonTexture));
    SDL_FreeSurface(buttonTexture);

    buttonTexture = IMG_Load("images/LED_off.png");
    _ledTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(buttonTexture));
    SDL_FreeSurface(buttonTexture);

    buttonTexture = IMG_Load("images/LED_glow.png");
    _ledGlowTextureID = reinterpret_cast<ImTextureID>(CreateGLTextureFromSurface(buttonTexture));
    SDL_FreeSurface(buttonTexture);

    surface = IMG_Load("images/camera_preview.png");
    _cameraPreviewTextureID = CreateGLTextureFromSurface(surface);
    SDL_FreeSurface(surface);
}

void VirtualUI::CreateFBO()
{
    glGenFramebuffers(1, &_cameraFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _cameraFBO);

    _fboTextureID = CreateGLTexture(800, 480);
    _fboDisplayTextureID = CreateGLTexture(320, 240, nullptr, GL_RGB, GL_NEAREST);

    // Set "renderedTexture" as our colour attachment #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _fboTextureID, 0);

    // Set the list of draw buffers.
    uint32_t drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers); // "1" is the size of DrawBuffers

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "FBO init failed" << std::endl;
    }
}

void VirtualUI::CompileShader()
{
    uint32_t vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    LoadShader("shaders/camera_preview.vert", vertexShaderID);
    std::cout << "Vert ID: " << vertexShaderID << std::endl;

    uint32_t fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    LoadShader("shaders/camera_preview.frag", fragmentShaderID);
    std::cout << "Frag ID: " << fragmentShaderID << std::endl;

    //     // Link the program
    std::cout << "Linking program" << std::endl;

    _programID = glCreateProgram();
    glAttachShader(_programID, vertexShaderID);
    glAttachShader(_programID, fragmentShaderID);
    glLinkProgram(_programID);
    // glUseProgram(_programID);

    std::cout << "Program ID: " << _programID << std::endl;

    // Check the program
    // ShowShaderLog(_programID);

    GLint success = GL_TRUE;
    glValidateProgram(_programID);
    glGetProgramiv(_programID, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLchar errorLog[1024] = {0};
        glGetProgramInfoLog(_programID, 1024, nullptr, errorLog);
        int i = 0;
        // checkForErrors();
        // Log::error(std::string() + "error validating shader program; Details: " + errorLog);
    }

    _cameraPreviewTexture = glGetUniformLocation(_programID, "cameraPreviewTexture");
    _analogGainShader = glGetUniformLocation(_programID, "analogGain");
    _brightnessFactor = glGetUniformLocation(_programID, "brightnessFactor");
    _contrastFactor = glGetUniformLocation(_programID, "contrastFactor");
}

uint32_t VirtualUI::LoadShader(std::string shaderFilePath, uint32_t shaderID)
{
    std::ifstream shaderStream(shaderFilePath, std::ios::in);
    std::string shaderCode((std::istreambuf_iterator<char>(shaderStream)), std::istreambuf_iterator<char>());

    // Compile shader
    std::cout << "Compiling shader: " << shaderFilePath << std::endl;
    char const* sourcePointer = shaderCode.c_str();

    glShaderSource(shaderID, 1, &sourcePointer, nullptr);
    glCompileShader(shaderID);

    // Check shader compilation
    ShowShaderLog(shaderID);

    return shaderID;
}

void VirtualUI::ShowShaderLog(uint32_t shaderID)
{
    int result = 0;
    int infoLogLength = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> shaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(shaderID, infoLogLength, nullptr, shaderErrorMessage.data());
        std::cout << std::string(begin(shaderErrorMessage), end(shaderErrorMessage)) << std::endl;
    }
}

void VirtualUI::ToggleLCDContrast(const bool toggleContrastEnabled) const
{
    lcdContrast = toggleContrastEnabled ? 0.7f : 1.0f;
}

void VirtualUI::RenderDisplayToFBO() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, _cameraFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _fboDisplayTextureID, 0);

    glViewport(0, 0, 320, 240);

    glUseProgram(_programID);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)_displayTextureID);
    // Set our "renderedTexture" sampler to use Texture Unit 0
    glUniform1i(_cameraPreviewTexture, 0);

    float brightness = 1.0f / 100.0f * lcdBrightness;
    glUniform1f(_contrastFactor, lcdContrast);
    glUniform1f(_brightnessFactor, brightness);

    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glVertexAttribPointer(0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,        // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          nullptr   // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    // Disable shader and FBO to revert to previous OpenGL state
    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Grabbed from the ImGui examples
void VirtualUI::ShowZoomTooltip()
{
    ImVec2 pos = ImGui::GetCursorScreenPos();
    int16_t textureWidth = 320;
    int16_t textureHeight = 240;

    // uint16_t data[2] = {123, 456};
    // ImGui::GetWindowDrawList()->ImDrawList::AddCallback(EnableShader, data);
    ImGui::Image(reinterpret_cast<ImTextureID>(_fboDisplayTextureID), ImVec2(textureWidth, textureHeight), ImVec2(0, 0),
                 ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    // ImGui::GetWindowDrawList()->ImDrawList::AddCallback(DisableShader, nullptr);

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        float region_sz = 48.0f;
        float region_x = _io.MousePos.x - pos.x - region_sz * 0.5f;
        if (region_x < 0.0f)
            region_x = 0.0f;
        else if (region_x > textureWidth - region_sz)
            region_x = textureWidth - region_sz;
        float region_y = _io.MousePos.y - pos.y - region_sz * 0.5f;
        if (region_y < 0.0f)
            region_y = 0.0f;
        else if (region_y > textureHeight - region_sz)
            region_y = textureHeight - region_sz;
        float zoom = 4.0f;
        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
        ImVec2 uv0 = ImVec2((region_x) / textureWidth, (region_y) / textureHeight);
        ImVec2 uv1 = ImVec2((region_x + region_sz) / textureWidth, (region_y + region_sz) / textureHeight);
        ImGui::Image(reinterpret_cast<ImTextureID>(_fboDisplayTextureID), ImVec2(region_sz * zoom, region_sz * zoom),
                     uv0, uv1, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::EndTooltip();
    }
}
void VirtualUI::RenderCameraPreviewToFBO() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, _cameraFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _fboTextureID, 0);

    glViewport(0, 0, 800, 480);

    glUseProgram(_programID);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _cameraPreviewTextureID);
    // Set our "renderedTexture" sampler to use Texture Unit 0
    glUniform1i(_cameraPreviewTexture, 0);

    float brightness = 0.1f * brightnessLevel;
    glUniform1f(_brightnessFactor, brightness);
    glUniform1f(_contrastFactor, 1.0f);

    // glBindTexture(GL_TEXTURE_2D, 1); // (GLuint)(intptr_t)cmd->TextureId - 1);

    // glDrawElements(GL_TRIANGLES, (GLsizei)cmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT :
    // GL_UNSIGNED_INT,
    //                (void*)cmd->IdxOffset);
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glVertexAttribPointer(0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,        // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          nullptr   // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    // Disable shader and FBO to revert to previous OpenGL state
    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderOverlay()
{
    // Set darker background and semi-transparent
    ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImU32)ImColor(16, 16, 16, 196));

    ImGui::BeginChild("Parameters", ImVec2(120.0f, 400.0f), true);

    ImGui::SetWindowFontScale(1.5f);

    ImGui::SetCursorPos(ImVec2(20, 20));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::Text("WB\nN/A");
    ImGui::PopStyleVar();

    ImGui::SetCursorPos(ImVec2(20, 120));
    ImGui::Text("Analog\nGain\nN/A");

    ImGui::SetCursorPos(ImVec2(20, 220));
    ImGui::Text("Digital\nGain\nN/A");

    ImGui::PopStyleColor();
    ImGui::EndChild();
}

void VirtualUI::RenderVirtualCamera()
{
    RenderCameraPreviewToFBO();

    ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
    ImGui::SetNextWindowPos(ImVec2(0, 480));
    ImGui::SetNextWindowSize(ImVec2(800, 480));

    ImGui::Begin("Image2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    ImGui::Image(reinterpret_cast<ImTextureID>(_fboTextureID), ImVec2(800, 480), ImVec2(0, 0), ImVec2(1, 1),
                 ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    ImGui::SetCursorPos(ImVec2(650, 30));
    RenderOverlay();

    ImGui::PopStyleVar();
    ImGui::End();
}

void VirtualUI::RenderKnob(int8_t& knobValue, Button& button)
{
    ImGui::SetCursorPos(ImVec2(40, 140));
    bool knobPressed = false;
    if (ImGui::Knob("Test123", value, knobPressed, _knobTextureID))
    {
        knobValue = -(value - lastValue);
        brightnessLevel -= knobValue;
        lastValue = value;
    }
    if (knobPressed)
    {
        button = Button::E_1_UP;
    }
}

void VirtualUI::RenderButtons(Button& button)
{
    uint16_t buttonPhotoWidth = 46;
    uint16_t buttonPhotoHeight = 46;

    uint16_t buttonRecordWidth = 68;
    uint16_t buttonRecordHeight = 68;

    uint16_t buttonWidth = 46;
    uint16_t buttonHeight = 30;

    uint16_t buttonRoundWidth = 30;
    uint16_t buttonRoundHeight = 30;

    // Special buttons (top-left)
    ImGui::SetCursorPos(ImVec2(55, 22));
    if (ImGui::CustomImageButton("SPECIAL_1", _buttonTextureID, _buttonPressedTextureID,
                                 ImVec2(buttonWidth, buttonHeight)))
    {
    }

    ImGui::SetCursorPos(ImVec2(148, 22));
    if (ImGui::CustomImageButton("SPECIAL_2", _buttonTextureID, _buttonPressedTextureID,
                                 ImVec2(buttonWidth, buttonHeight)))
    {
    }

    // Button PHOTO
    ImGui::SetCursorPos(ImVec2(236, 367));
    if (ImGui::CustomImageButton("PHOTO", _buttonPhotoTextureID, _buttonPhotoPressedTextureID,
                                 ImVec2(buttonPhotoWidth, buttonPhotoHeight)))
    {
    }

    // Button RECORD
    ImGui::SetCursorPos(ImVec2(225, 56));
    if (ImGui::CustomImageButton("PHOTO", _buttonRecordTextureID, _buttonRecordPressedTextureID,
                                 ImVec2(buttonRecordWidth, buttonRecordHeight)))
    {
    }

    // Buttons 1-3
    ImGui::SetCursorPos(ImVec2(372, 55));
    if (ImGui::CustomImageButton("1", _buttonTextureID, _buttonPressedTextureID, ImVec2(buttonWidth, buttonHeight)))
    {
        button = Button::BUTTON_1_UP;
    }

    ImGui::SetCursorPos(ImVec2(475, 55));
    if (ImGui::CustomImageButton("2", _buttonTextureID, _buttonPressedTextureID, ImVec2(buttonWidth, buttonHeight)))
    {
        button = Button::BUTTON_2_UP;
    }

    ImGui::SetCursorPos(ImVec2(578, 55));
    if (ImGui::CustomImageButton("3", _buttonTextureID, _buttonPressedTextureID, ImVec2(buttonWidth, buttonHeight)))
    {
        button = Button::BUTTON_3_UP;
    }

    // Button 4-6
    ImGui::SetCursorPos(ImVec2(372, 395));
    if (ImGui::CustomImageButton("4", _buttonTextureID, _buttonPressedTextureID, ImVec2(buttonWidth, buttonHeight)))
    {
        button = Button::BUTTON_4_UP;
    }

    ImGui::SetCursorPos(ImVec2(475, 395));
    if (ImGui::CustomImageButton("5", _buttonTextureID, _buttonPressedTextureID, ImVec2(buttonWidth, buttonHeight)))
    {
        button = Button::BUTTON_5_UP;
    }

    ImGui::SetCursorPos(ImVec2(578, 395));
    if (ImGui::CustomImageButton("6", _buttonTextureID, _buttonPressedTextureID, ImVec2(buttonWidth, buttonHeight)))
    {
        button = Button::BUTTON_6_UP;
    }

    // Button 7-9
    ImGui::SetCursorPos(ImVec2(249, 170));
    if (ImGui::CustomImageButton("7", _buttonRoundTextureID, _buttonRoundPressedTextureID,
                                 ImVec2(buttonRoundWidth, buttonRoundHeight)))
    {
        button = Button::BUTTON_7_UP;
    }

    ImGui::SetCursorPos(ImVec2(249, 225));
    if (ImGui::CustomImageButton("8", _buttonRoundTextureID, _buttonRoundPressedTextureID,
                                 ImVec2(buttonRoundWidth, buttonRoundHeight)))
    {
        button = Button::BUTTON_8_UP;
    }

    ImGui::SetCursorPos(ImVec2(249, 281));
    if (ImGui::CustomImageButton("9", _buttonRoundTextureID, _buttonRoundPressedTextureID,
                                 ImVec2(buttonRoundWidth, buttonRoundHeight)))
    {
        button = Button::BUTTON_9_UP;
    }

    // Button 10-12
    ImGui::SetCursorPos(ImVec2(717, 170));
    if (ImGui::CustomImageButton("10", _buttonRoundTextureID, _buttonRoundPressedTextureID,
                                 ImVec2(buttonRoundWidth, buttonRoundHeight)))
    {
        button = Button::BUTTON_10_UP;
    }

    ImGui::SetCursorPos(ImVec2(717, 225));
    if (ImGui::CustomImageButton("11", _buttonRoundTextureID, _buttonRoundPressedTextureID,
                                 ImVec2(buttonRoundWidth, buttonRoundHeight)))
    {
        button = Button::BUTTON_11_UP;
    }

    ImGui::SetCursorPos(ImVec2(717, 281));
    if (ImGui::CustomImageButton("12", _buttonRoundTextureID, _buttonRoundPressedTextureID,
                                 ImVec2(buttonRoundWidth, buttonRoundHeight)))
    {
        button = Button::BUTTON_12_UP;
    }
}

void EnableBlending(const ImDrawList* parent_list, const ImDrawCmd* cmd) { glBlendFunc(GL_ONE, GL_ONE); }

void DisableBlending(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Render RGB LEDs
void VirtualUI::RenderLED(int8_t glowValue)
{
    // Render off state
    ImGui::SetCursorPos(ImVec2(63, 70));
    ImGui::Image(_ledTextureID, ImVec2(30, 29), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

    ImGui::SetCursorPos(ImVec2(63, 109));
    ImGui::Image(_ledTextureID, ImVec2(30, 29), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

    // Render LED light
    ImGui::GetWindowDrawList()->ImDrawList::AddCallback(EnableBlending, nullptr);

    ImGui::SetCursorPos(ImVec2(58, 65));
    ImGui::Image(_ledGlowTextureID, ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 32, 32, 255 - glowValue));

    ImGui::SetCursorPos(ImVec2(58, 104));
    ImGui::Image(_ledGlowTextureID, ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1), ImColor(64, 64, 255, glowValue));

    ImGui::GetWindowDrawList()->ImDrawList::AddCallback(DisableBlending, nullptr);
}

int glowValue = 0;

void VirtualUI::RenderUI(Button& button, int8_t& knobValue, bool& debugOverlayEnabled)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(_window);

    ImGui::NewFrame();

    RenderDisplayToFBO();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(800, 480));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImU32)ImColor(0, 0, 0, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Image", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::Image(_backgroundTextureID, ImVec2(800, 480));

    RenderKnob(knobValue, button);
    RenderButtons(button);

    glowValue += knobValue;
    RenderLED(glowValue);

    ImGui::SetCursorPos(ImVec2(337, 119));
    ShowZoomTooltip();

    ImGui::SetCursorPos(ImVec2(50, 375));
    ImGui::ToggleButton("debug_overlay_switch", "Debug overlay", &debugOverlayEnabled);

    ToggleLCDContrast(toggleContrast);
    ImGui::SetCursorPos(ImVec2(50, 410));
    ImGui::ToggleButton("toggle_contrast_switch", "Simulate LCD", &toggleContrast);

    ImGui::SetCursorPos(ImVec2(50, 445));
    if (ImGui::Button("Take Screenshot") && _screenshotHandler)
    {
        _screenshotHandler();
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::End();

    RenderVirtualCamera();

    ImGui::EndFrame();

    ImGui::Render();
}

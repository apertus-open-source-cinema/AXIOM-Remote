#include "VirtualUI.h"

#include <fstream>
#include <iostream>
#include <vector>

#include <GL/gl3w.h>

#include <SDL2/SDL_image.h>

// Additional controls, like knob
#include "imgui_modules/imgui_user.h"

#include "UI/ButtonDefinitions.h"

#include "HelpersGL.h"

uint8_t value = 0;
uint8_t lastValue = 0;
uint8_t brightnessLevel = 0;

VirtualUI::VirtualUI(SDL_Window* window, uint32_t displayTextureID, uint32_t backgroundTextureID) :
    _window(window), _io(ImGui::GetIO()), _displayTextureID(reinterpret_cast<ImTextureID>(displayTextureID)),
    _backgroundTextureID(reinterpret_cast<ImTextureID>(backgroundTextureID))
{
    SDL_Surface* surface = IMG_Load("images/knob_clean.png");
    _knobTextureID = CreateGLTextureFromSurface(surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("images/camera_preview.png");
    _cameraPreviewTextureID = CreateGLTextureFromSurface(surface);
    SDL_FreeSurface(surface);

    CreateFBO();

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
    glGenBuffers(1, &_vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

    CompileShader();
}

void VirtualUI::CreateFBO()
{
    glGenFramebuffers(1, &_cameraFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _cameraFBO);

    _fboTextureID = CreateGLTexture(800, 480);

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

bool RenderButton(std::string name, uint16_t x, uint16_t y, uint16_t width = 40, uint16_t height = 30)
{
    ImGui::SetCursorPos(ImVec2(x, y));
    return ImGui::Button(name.c_str(), ImVec2(width, height));
}

// Grabbed from the ImGui examples
void VirtualUI::ShowZoomTooltip()
{
    ImVec2 pos = ImGui::GetCursorScreenPos();
    int16_t textureWidth = 320;
    int16_t textureHeight = 240;

    ImGui::Image(_displayTextureID, ImVec2(textureWidth, textureHeight), ImVec2(0, 0), ImVec2(1, 1),
                 ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
                 
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
        ImGui::Image(_displayTextureID, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1,
                     ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
        ImGui::EndTooltip();
    }
}
void VirtualUI::RenderCameraPreviewToFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _cameraFBO);
    glViewport(0, 0, 800, 480);

    glUseProgram(_programID);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _cameraPreviewTextureID);
    // Set our "renderedTexture" sampler to use Texture Unit 0
    glUniform1i(_cameraPreviewTexture, 0);

    float brightness = 0.1f * brightnessLevel;
    std::cout << "Brightness: " << brightness << std::endl;
    glUniform1f(_analogGainShader, brightness);

    // glBindTexture(GL_TEXTURE_2D, 1); // (GLuint)(intptr_t)cmd->TextureId - 1);

    // glDrawElements(GL_TRIANGLES, (GLsizei)cmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT :
    // GL_UNSIGNED_INT,
    //                (void*)cmd->IdxOffset);
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
    glVertexAttribPointer(0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,        // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void*)0  // array buffer offset
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
    // ImGui::SetNextWindowContentSize(ImVec2(800, 480));

    ImGui::Begin("Image2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    ImGui::Image(reinterpret_cast<ImTextureID>(_fboTextureID), ImVec2(800, 480), ImVec2(0, 0), ImVec2(1, 1),
                 ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

    ImGui::SetCursorPos(ImVec2(650, 30));
    RenderOverlay();

    ImGui::PopStyleVar();
    ImGui::End();
}

void VirtualUI::RenderUI(Button& button, int8_t& knobValue, bool& debugOverlayEnabled)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(_window);

    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(800, 480));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImU32)ImColor(255, 0, 0, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Image", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::Image(_backgroundTextureID, ImVec2(800, 480));

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

    if (RenderButton("7", 340, 176))
    {
        button = Button::BUTTON_7_UP;
    }
    if (RenderButton("8", 340, 226))
    {
        button = Button::BUTTON_8_UP;
    }
    if (RenderButton("9", 340, 276))
    {
        button = Button::BUTTON_9_UP;
    }

    if (RenderButton("10", 740, 176))
    {
        button = Button::BUTTON_10_UP;
    }
    if (RenderButton("11", 740, 226))
    {
        button = Button::BUTTON_11_UP;
    }
    if (RenderButton("12", 740, 276))
    {
        button = Button::BUTTON_12_UP;
    }

    ImGui::SetCursorPos(ImVec2(40, 140));
    bool knobPressed = false;
    if (ImGui::Knob("Test123", value, knobPressed, (ImTextureID)_knobTextureID))
    {
        knobValue = -(value - lastValue);
        brightnessLevel -= knobValue;
        if (brightnessLevel < 0)
        {
            brightnessLevel = 0;
        }

        lastValue = value;
    }
    if (knobPressed)
    {
        button = Button::E_1_UP;
    }

    ImGui::SetCursorPos(ImVec2(338, 119));
    ShowZoomTooltip();

    ImGui::SetCursorPos(ImVec2(50, 400));
    ImGui::ToggleButton("debug_overlay_switch", "Debug overlay", &debugOverlayEnabled);

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::End();

    RenderVirtualCamera();

    ImGui::EndFrame();

    ImGui::Render();
}

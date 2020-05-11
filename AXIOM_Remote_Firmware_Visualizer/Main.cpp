#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/gl3w.h>
#include <GL/gl.h>

#include "VirtualUI.h"

#include "UI/MenuSystem.h"
#include "UI/Painter/Painter.h"

// Debug
#define DEBUG_DRAW
#ifdef DEBUG_DRAW
#include "UI/Painter/DebugPainter.h"
#endif

// Periphery
#include "USBCDCTerminalDevice.h"

#define FRAMEBUFFER_WIDTH 320
#define FRAMEBUFFER_HEIGHT 240

enum class GLTextureFilter
{
    Nearest,
    Linear
};

void Shutdown(SDL_Window* win)
{
    if (win != nullptr)
    {
        SDL_DestroyWindow(win);
    }

    SDL_Quit();
}

void Initialization(SDL_Window** window)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    *window = 
        SDL_CreateWindow("AXIOM Remote Visualizer", 100, 100, 800, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (*window == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void RenderDisplay(uint16_t* sourceFramebuffer, uint8_t* targetFramebuffer, int width, int height)
{
    unsigned int j = 0;
    for (int yIndex = 0; yIndex < height; yIndex++)
    {
        for (int xIndex = 0; xIndex < width; xIndex++)
        {
            uint16_t val = sourceFramebuffer[yIndex * width + xIndex];

            targetFramebuffer[j] = (val >> 11) << 3;
            targetFramebuffer[j + 1] = ((val >> 5) & 0x3F) << 2;
            targetFramebuffer[j + 2] = (val & 0x1F) << 3;
            j += 3;
        }
    }
}

uint32_t CreateGLTexture(SDL_Surface* surface, GLint textureFilter = GL_LINEAR)
{
    uint32_t textureID = 0;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int mode = GL_RGB;
    if (surface->format->BytesPerPixel == 4)
    {
        mode = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilter);

    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "Texture ID: " << textureID << std::endl;
    return textureID;
}

void SetupGL(SDL_Window* window, SDL_GLContext& glContext)
{
    gl3wInit();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    int majorVersionGL = 0;
    int minorVersionGL = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersionGL);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorVersionGL);
    std::cout << "GL Version: " << majorVersionGL << "." << minorVersionGL << std::endl;
}

void SetupImGui(SDL_Window* window, SDL_GLContext glContext)
{
    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 130");
}

int main()
{
    std::cout << "AXIOM Remote Visualizer" << std::endl;

    uint8_t* framebuffer = new uint8_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 3];
    uint16_t* frameBuffer = new uint16_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];

    SDL_Window* window;
    SDL_GLContext glContext;

    Initialization(&window);
    SetupGL(window, glContext);
    SetupImGui(window, glContext);

    const ImGuiIO& io = ImGui::GetIO();

    SDL_Surface* displayTexture = SDL_CreateRGBSurface(0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 24, 0, 0, 0, 0);
    uint32_t displayTextureID = CreateGLTexture(displayTexture, GL_NEAREST);
    SDL_FreeSurface(displayTexture);

    SDL_Surface* surface = IMG_Load("images/knob_clean.png");
    uint32_t knobTextureID = CreateGLTexture(surface);
    SDL_FreeSurface(surface);

    Painter painter(frameBuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

#ifdef DEBUG_DRAW
    DebugPainter debugPainter;
    painter.SetDebugOverlay(&debugPainter);
#endif

    USBCDCTerminalDevice cdcDevice;

    MenuSystem menuSystem(&cdcDevice);

    Button button = Button::BUTTON_NONE;

    int8_t knobValue = 0;
    bool debugOverlayEnabled = false;

    bool appIsRunning = true;
    const int frames = 60;
    SDL_Event events;
    while (appIsRunning)
    {
        while (SDL_PollEvent(&events))
        {
            if ((events.type == SDL_WINDOWEVENT && events.window.event == SDL_WINDOWEVENT_CLOSE) ||
                (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_ESCAPE))
            {
                appIsRunning = false;
            }
        }

#ifdef DEBUG_DRAW
        debugPainter.SetEnable(debugOverlayEnabled);
#endif

        menuSystem.Draw(&painter);
        RenderDisplay(frameBuffer, framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

        glBindTexture(GL_TEXTURE_2D, displayTextureID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE,
                        framebuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        button = Button::BUTTON_NONE;
        RenderUI(window, io, reinterpret_cast<ImTextureID>(knobTextureID),
                 reinterpret_cast<ImTextureID>(displayTextureID), button, knobValue, debugOverlayEnabled);

        menuSystem.Update(button, knobValue);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frames));
    }

    Shutdown(window);

    return 0;
}


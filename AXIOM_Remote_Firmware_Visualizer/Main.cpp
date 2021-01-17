#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <thread>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/gl3w.h>
#include <GL/gl.h>

#include "HelpersGL.h"
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

//#include "CentralDB.h"

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

    *window = SDL_CreateWindow("AXIOM Remote Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 480 * 2,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (*window == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void SetupGL(SDL_Window* window, SDL_GLContext& glContext)
{
    gl3wInit();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    // Set black background
    glClearColor(0, 0, 0, 1);

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

void OpenSerialPort(std::string port)
{
    int serial_port = open(port.c_str(), O_RDWR);

    // Check for errors
    if (serial_port < 0)
    {
        std::cout << "Error " << errno << " from open: " << strerror(errno) << std::endl;
    }

    std::string testMessage = "Vis serial test\n";
    write(serial_port, testMessage.c_str(), testMessage.length());
}

bool GetCmdOption(char** begin, uint8_t count, const std::string& option, std::string& value)
{
    char** end = begin + count;
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        value = *itr;
        return true;
    }

    return false;
}

void ProcessCommandLine(int argc, char* argv[])
{
    std::string value = "";
    if (GetCmdOption(argv, argc, "-p", value))
    {
        // TODO: Add serial port handling
        OpenSerialPort(value);
    }
}

int main(int argc, char* argv[])
{
    std::cout << "AXIOM Remote Visualizer" << std::endl;

    ProcessCommandLine(argc, argv);

    auto frameBuffer = new uint16_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];

    SDL_Window* window;
    SDL_GLContext glContext;

    Initialization(&window);
    SetupGL(window, glContext);
    SetupImGui(window, glContext);

    uint32_t displayTextureID = CreateGLTexture(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, nullptr, GL_RGB, GL_NEAREST);

    SDL_Rect texture_rect;
    texture_rect.x = 400;                    // the x coordinate
    texture_rect.y = 120;                    // the y coordinate
    texture_rect.w = FRAMEBUFFER_WIDTH * 4;  // the width of the texture
    texture_rect.h = FRAMEBUFFER_HEIGHT * 4; // the height of the texture

    Painter painter(frameBuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

#ifdef DEBUG_DRAW
    DebugPainter debugPainter;
    painter.SetDebugOverlay(&debugPainter);
#endif

    USBCDCTerminalDevice cdcDevice;

    CentralDB centralDB;

    MenuSystem menuSystem(&cdcDevice, &centralDB);

    Button button = Button::BUTTON_NONE;

    int8_t knobValue = 0;
    bool debugOverlayEnabled = false;

    std::shared_ptr<VirtualUI> virtualUI = std::make_shared<VirtualUI>(window, displayTextureID, &centralDB);

    centralDB.SetUint32(Attribute::ID::REMOTE_LCD_BRIGHTNESS, 75);

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

        glClear(GL_COLOR_BUFFER_BIT);

#ifdef DEBUG_DRAW
        debugPainter.SetEnable(debugOverlayEnabled);
#endif

        menuSystem.Draw(&painter);

        glBindTexture(GL_TEXTURE_2D, displayTextureID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, GL_RGB, GL_UNSIGNED_SHORT_5_6_5,
                        frameBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        button = Button::BUTTON_NONE;
        virtualUI->RenderUI(button, knobValue, debugOverlayEnabled);

        menuSystem.Update(button, knobValue);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frames));
    }

    Shutdown(window);

    return 0;
}

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <memory>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>

#include <glad/gl.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "3rdParty/imgui-src/imgui.h"
#include "DesktopGLDriver.h"
#include "ILvglPlatformDriver.h"
#include "LvglUI.h"
#include "SDL3/SDL_timer.h"
#include "VirtualUI.h"

#include "HelpersGL.h"

#include <ButtonDefinitions.h>

// Constants
const int FRAMEBUFFER_WIDTH = 320;
const int FRAMEBUFFER_HEIGHT = 240;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 960; // 480 * 2
const int GL_CONTEXT_MAJOR_VERSION = 3;
const int GL_CONTEXT_MINOR_VERSION = 3;
const int TARGET_FPS = 60;
const int MS_PER_SECOND = 1000;

enum class GLTextureFilter { Nearest, Linear };

class Application {
private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
    ImGuiIO io;

    std::unique_ptr<DesktopGLDriver> platformDriver;
    std::unique_ptr<LvglUI> lvglUI;
    std::unique_ptr<VirtualUI> virtualUI;

    ButtonID button = ButtonID::BUTTON_NONE;
    int8_t knobValue = 0;
    bool debugOverlayEnabled = false;

    int serialPortFd = -1;
    std::unique_ptr<uint16_t[]> frameBuffer;

    // Initialization state tracking
    bool sdlInitialized = false;
    bool glContextCreated = false;
    bool imguiInitialized = false;
    bool lvglInitialized = false;
    bool appInitialized = false;

    bool initializeSDL();
    bool initializeGL();
    bool initializeImGui();
    bool initializeLVGL();
    void setupSerialPort(const std::string& port);
    void sendSerialCommand(const std::string& command);
    void readAndPrintSerial();
    std::string createSerialCommand(ButtonID id, ButtonState state);
    void cleanup();

public:
    Application() = default;
    ~Application() { cleanup(); }

    bool initialize();
    void run();
};

bool Application::initializeSDL() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    sdlInitialized = true;

    window = SDL_CreateWindow("AXIOM Remote Visualizer", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

bool Application::initializeGL() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_CONTEXT_MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_CONTEXT_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Failed to create OpenGL context" << std::endl;
        return false;
    }
    glContextCreated = true;

    int gladVersion = gladLoadGL(SDL_GL_GetProcAddress);
    if (gladVersion == 0) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return false;
    }

    SDL_GL_SetSwapInterval(1);
    SDL_GL_MakeCurrent(window, glContext);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL Version: " << version << std::endl;

    return true;
}

bool Application::initializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO();

    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    imguiInitialized = true;
    return true;
}

bool Application::initializeLVGL() {
    platformDriver = std::make_unique<DesktopGLDriver>();

    if (!platformDriver) {
        SDL_Log("Failed to create platform driver");
        return false;
    }

    // Initialize platform driver first so it can provide resolution info
    platformDriver->Initialize();

    lv_init();

    lv_display_t* lvglDisplay = lv_display_create(platformDriver->GetHorRes(), platformDriver->GetVerRes());
    if (!lvglDisplay) {
        SDL_Log("Failed to create LVGL display");
        return false;
    }

    lv_display_set_flush_cb(lvglDisplay, DesktopGLDriver::FlushCallbackStatic);
    lv_display_set_buffers(lvglDisplay, DesktopGLDriver::s_drawBuf1, nullptr,
                           sizeof(DesktopGLDriver::s_drawBuf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_user_data(lvglDisplay, platformDriver.get());


    lvglUI = std::make_unique<LvglUI>();

    if (!lvglUI) {
        SDL_Log("Failed to create LVGL UI");
        return false;
    }

    lvglInitialized = true;
    return true;
}

bool Application::initialize() {
    frameBuffer = std::make_unique<uint16_t[]>(FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT);

    if (!initializeSDL()) {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return false;
    }
    if (!initializeGL()) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return false;
    }
    if (!initializeImGui()) {
        std::cerr << "Failed to initialize ImGui" << std::endl;
        return false;
    }
    if (!initializeLVGL()) {
        std::cerr << "Failed to initialize LVGL" << std::endl;
        return false;
    }

    GLuint uiTextureID = platformDriver->GetTextureId();
    virtualUI = std::make_unique<VirtualUI>(window, uiTextureID);

    auto buttonCallback = [&](ButtonID id, ButtonState state) {
        if (lvglUI) {
            lvglUI->TriggerButtonEvent(id, state);
            std::string command = createSerialCommand(id, state);
            sendSerialCommand(command);
        }
    };
    virtualUI->SetButtonClickHandler(buttonCallback);

    virtualUI->SetKnobHandler([&](int delta) {
        LvglUI::UpdateEncoder(delta);
    });

    virtualUI->SetDebugToggleHandler([&](bool enabled) {
        if (lvglUI) {
            lvglUI->SetDebugMarkersVisible(enabled);
        }
    });

    appInitialized = true;
    return true;
}

void Application::setupSerialPort(const std::string& port) {
    serialPortFd = open(port.c_str(), O_RDWR | O_NOCTTY);
    if (serialPortFd < 0) {
        std::cerr << "Error opening serial port " << port << ": " << strerror(errno) << std::endl;
        return;
    }
    std::cout << "Serial port " << port << " opened." << std::endl;
}

void Application::run() {
    bool appIsRunning = true;
    uint64_t lastTickUpdateMs = SDL_GetTicks();
    SDL_Event events;

    while (appIsRunning) {
        while (SDL_PollEvent(&events)) {
            ImGui_ImplSDL3_ProcessEvent(&events);

            if (events.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED ||
                (events.type == SDL_EVENT_KEY_DOWN && events.key.key == SDLK_ESCAPE)) {
                appIsRunning = false;
            }
        }

        readAndPrintSerial();

        uint64_t nowMs = SDL_GetTicks();
        uint64_t deltaMs = nowMs - lastTickUpdateMs;
        if (deltaMs > 0) {
            lv_tick_inc(static_cast<uint32_t>(deltaMs));
            lastTickUpdateMs = nowMs;
        }

        lv_task_handler();

        glClear(GL_COLOR_BUFFER_BIT);
        virtualUI->Render(button, knobValue, debugOverlayEnabled);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_SECOND / TARGET_FPS));
    }
}

void Application::cleanup() {
    // Destroy UI components first
    if (virtualUI) {
        virtualUI.reset();
    }
    if (lvglInitialized) {
        if (lvglUI) lvglUI.reset();
        if (platformDriver) platformDriver.reset();
    }

    // Shutdown ImGui before destroying GL context
    if (imguiInitialized) {
        // Ensure GL context is active before shutting down OpenGL backends
        if (glContextCreated) {
            SDL_GL_MakeCurrent(window, glContext);
        }
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    // Destroy GL resources
    if (glContextCreated) {
        SDL_GL_DestroyContext(glContext);
        glContext = nullptr;
        glContextCreated = false;
    }

    // Destroy window
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    // Quit SDL
    if (sdlInitialized) {
        SDL_Quit();
        sdlInitialized = false;
    }

    // Close serial port
    if (serialPortFd >= 0) {
        close(serialPortFd);
        serialPortFd = -1;
    }

    appInitialized = false;
}

void Application::sendSerialCommand(const std::string& command) {
    std::cout << "Sending command: " << command; // command already has newline
    if (serialPortFd >= 0) {
        ssize_t bytes_written = write(serialPortFd, command.c_str(), command.length());
        if (bytes_written < 0) {
            std::cerr << "Error writing to serial port: " << strerror(errno) << std::endl;
        } else if (bytes_written < command.length()) {
            std::cerr << "Warning: Serial write incomplete." << std::endl;
        }
    } else {
        std::cerr << "Error: Serial port not open." << std::endl;
    }
}

void Application::readAndPrintSerial() {
    if (serialPortFd < 0) return;

    char read_buf[256];
    ssize_t num_bytes = read(serialPortFd, read_buf, sizeof(read_buf) - 1);

    if (num_bytes > 0) {
        read_buf[num_bytes] = '\0'; // Null-terminate the string
        std::cout << "HW RX: " << read_buf;
        // The hardware might send multiple lines at once, so flush to ensure it's all printed
        std::cout.flush();
    } else if (num_bytes < 0 && errno != EAGAIN) {
        // EAGAIN means no data available, which is expected in non-blocking mode.
        // Any other error is unexpected.
        std::cerr << "Error reading from serial port: " << strerror(errno) << std::endl;
    }
    // if num_bytes is 0, it can mean the device was disconnected.
}

std::string Application::createSerialCommand(ButtonID id, ButtonState state) {
    std::string stateStr = (state == ButtonState::Pressed) ? "PRESS" : "RELEASE";
    std::string idStr = "BUTTON_";
    idStr += std::to_string(static_cast<int>(id));
    return stateStr + ":" + idStr + "\n";
}

bool GetCmdOption(char **begin, uint8_t count, const std::string &option,
                  std::string &value) {
  char **end = begin + count;
  char **itr = std::find(begin, end, option);
  if (itr != end && ++itr != end) {
    value = *itr;
    return true;
  }

  return false;
}

void ProcessCommandLine(int argc, char *argv[]) {
  std::string value = "";
  if (GetCmdOption(argv, argc, "-p", value)) {
    // TODO: Add serial port handling
    // OpenSerialPort(value);
  }
}

int main(int argc, char *argv[]) {
    std::cout << "AXIOM Remote Visualizer" << std::endl;
    ProcessCommandLine(argc, argv);

    Application app;
    if (!app.initialize()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }

    app.run();
    return 0;
}

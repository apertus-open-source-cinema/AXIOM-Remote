#include <algorithm>
#include <chrono>
#include <ctime>
#include <fcntl.h>
#include <fcntl.h> // For O_RDWR
#include <functional>
#include <iostream>
#include <memory>
#include <src/misc/lv_types.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>
#include <unistd.h> // For write(), close()

#include <glad/gl.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "3rdParty/imgui-src/imgui.h"
#include "DesktopGLDriver.h"
#include "ILvglPlatformDriver.h"
#include "LvglCore.h"
#include "LvglUI.h"
#include "SDL3/SDL_timer.h"
#include "VirtualUI.h"

#include "Theme.h"

// #include "UI/MenuSystem.h"
// #include "UI/Painter/Painter.h"

#include "HelpersGL.h"

// #include "lv_conf.h"
// #include <lvgl.h>
// #include <UI/UI.h>

// Debug
// #define DEBUG_DRAW
// #ifdef DEBUG_DRAW
// #include "UI/Painter/DebugPainter.h"
// #endif

// Periphery
// #include "USBCDCTerminalDevice.h"

// #include "CentralDB.h"

#include <ButtonDefinitions.h>

#define FRAMEBUFFER_WIDTH 320
#define FRAMEBUFFER_HEIGHT 240

enum class GLTextureFilter { Nearest, Linear };

void Shutdown(SDL_Window *win) {
  if (win != nullptr) {
    SDL_DestroyWindow(win);
  }

  SDL_Quit();
}

void Initialization(SDL_Window **window) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    exit(EXIT_FAILURE);
  }

  *window =
      SDL_CreateWindow("AXIOM Remote Visualizer", 800, 480 * 2,
                       SDL_WINDOW_OPENGL); // | SDL_WINDOW_HIGH_PIXEL_DENSITY);
  if (!window) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
}

void SetupGL(SDL_Window *window, SDL_GLContext &glContext) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  glContext = SDL_GL_CreateContext(window);

  int gladVersion = gladLoadGL(SDL_GL_GetProcAddress);
  if (gladVersion == 0) {
    printf("Failed to initialize OpenGL context\n");
    exit(-1);
  }

  SDL_GL_SetSwapInterval(1);
  SDL_GL_MakeCurrent(window, glContext);
  // Set black background

  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *version = glGetString(GL_VERSION);
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "OpenGL Version: " << version << std::endl;

  // int majorVersionGL = 0;
  // int minorVersionGL = 0;
  // SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersionGL);
  // SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorVersionGL);
  // std::cout << "GL Version: " << majorVersionGL << "." << minorVersionGL
  //           << std::endl;
}

ImGuiIO io;

void SetupImGui(SDL_Window *window, SDL_GLContext glContext) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = ImGui::GetIO();
  // ImGuiIO& io = ImGui::GetIO();
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  //   io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  //   io.ConfigViewportsNoAutoMerge = true;
  // io.ConfigViewportsNoTaskBarIcon = true;

  ImGui_ImplSDL3_InitForOpenGL(window, glContext);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void OpenSerialPort(std::string port) {
  int serial_port = open(port.c_str(), O_RDWR);

  // Check for errors
  if (serial_port < 0) {
    std::cout << "Error " << errno << " from open: " << strerror(errno)
              << std::endl;
  }

  std::string testMessage = "Vis serial test\n";
  write(serial_port, testMessage.c_str(), testMessage.length());
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
    OpenSerialPort(value);
  }
}

void ScreenshotHandler(uint16_t *frameBuffer, int width, int height) {
  SDL_Surface *surf = SDL_CreateSurfaceFrom(
      width, height, SDL_PIXELFORMAT_RGB24, frameBuffer, 0);
  const auto now = std::chrono::system_clock::now();
  const auto inTimeT = std::chrono::system_clock::to_time_t(now);
  const auto localTime = std::localtime(&inTimeT);
  std::string filePath = "../screenshots/";
  constexpr auto dateBufferSize = 50;
  char buffer[dateBufferSize];
  std::strftime(buffer, sizeof buffer, "%F_%T.png", localTime);
  filePath.append(buffer);
  IMG_SavePNG(surf, filePath.c_str());
}

int serial_port_fd = -1; // File descriptor

bool OpenAndConfigureSerial(const std::string &port_name) {
  serial_port_fd = open(port_name.c_str(), O_RDWR | O_NOCTTY);
  if (serial_port_fd < 0) {
    std::cerr << "Error opening serial port " << port_name << ": "
              << strerror(errno) << std::endl;
    return false;
  }
  // Optional: Configure termios settings (often not strictly needed for USB
  // CDC)
  // struct termios tty; if(tcgetattr(serial_port_fd, &tty) != 0) { /*
  // error */ } cfsetispeed(&tty, B115200); cfsetospeed(&tty, B115200); //
  // Example baud tty.c_cflag &= ~PARENB; tty.c_cflag &= ~CSTOPB; tty.c_cflag &=
  // ~CSIZE; tty.c_cflag |= CS8; tty.c_cflag &= ~CRTSCTS; tty.c_cflag |= CREAD |
  // CLOCAL; tty.c_lflag &= ~ICANON; tty.c_lflag &= ~ECHO; // Non-canonical, no
  // echo
  // // ... set other flags (IGNBRK, no flow control etc.) ...
  // if (tcsetattr(serial_port_fd, TCSANOW, &tty) != 0) { /* error */ }
  std::cout << "Serial port " << port_name << " opened." << std::endl;
  return true;
}

void SendSerialCommand(const std::string &command) {
  std::cout << "Sending command: " << command; // command already has newline
  if (serial_port_fd >= 0) {
    ssize_t bytes_written =
        write(serial_port_fd, command.c_str(), command.length());
    if (bytes_written < 0) {
      std::cerr << "Error writing to serial port: " << strerror(errno)
                << std::endl;
    } else if (bytes_written < command.length()) {
      std::cerr << "Warning: Serial write incomplete." << std::endl;
    }
  } else {
    std::cerr << "Error: Serial port not open." << std::endl;
  }
}

void ReadAndPrintSerial() {
  if (serial_port_fd < 0)
    return;

  char read_buf[256];
  ssize_t num_bytes = read(serial_port_fd, read_buf, sizeof(read_buf) - 1);

  if (num_bytes > 0) {
    read_buf[num_bytes] = '\0'; // Null-terminate the string
    std::cout << "HW RX: " << read_buf;
    // The hardware might send multiple lines at once, so flush to ensure it's
    // all printed
    std::cout.flush();
  } else if (num_bytes < 0 && errno != EAGAIN) {
    // EAGAIN means no data available, which is expected in non-blocking mode.
    // Any other error is unexpected.
    std::cerr << "Error reading from serial port: " << strerror(errno)
              << std::endl;
  }
  // if num_bytes is 0, it can mean the device was disconnected.
}

// enum class ButtonID {
//   BUTTON_NONE = 0,
//   BUTTON_1,
//   BUTTON_2,
//   BUTTON_3,
//   BUTTON_4,
//   BUTTON_5,
//   BUTTON_6,
//   BUTTON_7,
//   BUTTON_8,
//   BUTTON_9,
//   BUTTON_10,
//   BUTTON_11,
//   BUTTON_12
// };

auto frameBuffer = new uint16_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];

void handleLvglKeys(std::unique_ptr<LvglUI> &lvglUI, SDL_Event &events) {
  // if (lvglUI) {
  switch (events.key.key) {
  // Map keys (e.g., F1-F6) directly to UI blocks
  case SDLK_F1:
    // lvglUI->TriggerBlockClick(ButtonID::BUTTON_1);
    break;
    // case SDLK_F2:
    //   lvglUI->TriggerBlockClick(LvglUI::BlockIdentifier::A_GAIN);
    //   break;
    // case SDLK_F3:
    //   lvglUI->TriggerBlockClick(LvglUI::BlockIdentifier::D_GAIN);
    //   break;
    // case SDLK_F4:
    //   lvglUI->TriggerBlockClick(LvglUI::BlockIdentifier::MENU);
    //   break;
    // case SDLK_F5:
    //   lvglUI->TriggerBlockClick(LvglUI::BlockIdentifier::SHUTTER);
    //   break;
    // case SDLK_F6:
    //   lvglUI->TriggerBlockClick(LvglUI::BlockIdentifier::WB);
    //   break;
    // // Map number keys 1-6 as an alternative
    // case SDLK_1:
    //   lvglUI->TriggerBlockClick(LvglUI::BlockIdentifier::FPS);
    //   break;
    // case SDLK_2:
    //   lvglUI->TriggerBlockClick(LvglUI::BlockIdentifier::A_GAIN);
    //   break;
    // case SDLK_3:
    //   lvglUI->TriggerBlockClick(LvglUI::BlockIdentifier::D_GAIN);
    //   break;
    // case SDLK_4:
    //   lvglUI->TriggerBlockClick(LvglUI::BlockIdentifier::MENU);
    //   break;
    // case SDLK_5:
    //   lvglUI->TriggerBlockClick(LvglUI::BlockIdentifier::SHUTTER);
    //   break;
    // case SDLK_6:
    //   lvglUI->TriggerBlockClick(LvglUI::BlockIdentifier::WB);
    //   break;

  default:
    break; // Ignore other keys for this purpose
  }
  // }
}

std::string CreateSerialCommand(ButtonID id, ButtonState state) {
  std::string stateStr = (state == ButtonState::Pressed) ? "PRESS" : "RELEASE";
  std::string idStr = "BUTTON_";
  idStr += std::to_string(static_cast<int>(id));
  return stateStr + ":" + idStr + "\n";
}

int main(int argc, char *argv[]) {
  std::cout << "AXIOM Remote Visualizer" << std::endl;
  ProcessCommandLine(argc, argv);

  SDL_Window *window;
  SDL_GLContext glContext;

  Initialization(&window);
  SetupGL(window, glContext);
  SetupImGui(window, glContext);

  // uint32_t displayTextureID = CreateGLTexture(
  //     FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, frameBuffer, GL_RGB,
  //     GL_NEAREST);

  // SDL_Rect texture_rect;
  // texture_rect.x = 400;                    // the x coordinate
  // texture_rect.y = 120;                    // the y coordinate
  // texture_rect.w = FRAMEBUFFER_WIDTH * 4;  // the width of the texture
  // texture_rect.h = FRAMEBUFFER_HEIGHT * 4; // the height of the texture

  // Painter painter(frameBuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

  // #ifdef DEBUG_DRAW
  //   DebugPainter debugPainter;
  //   painter.SetDebugOverlay(&debugPainter);
  // #endif

  // USBCDCTerminalDevice cdcDevice;

  // CentralDB centralDB;

  // MenuSystem menuSystem(&cdcDevice, &centralDB);

  ButtonID button = ButtonID::BUTTON_NONE;

  int8_t knobValue = 0;
  bool debugOverlayEnabled = false;

  std::unique_ptr<LvglCore> lvglCore; // Keep smart pointer names
  std::unique_ptr<DesktopGLDriver> platformDriver;
  std::unique_ptr<LvglUI> lvglUI;

  lvglCore = std::make_unique<LvglCore>();
  platformDriver = std::make_unique<DesktopGLDriver>();

  lv_display_t *lvglDisplay = lv_display_create(platformDriver->GetHorRes(),
                                                platformDriver->GetVerRes());
  lv_display_set_flush_cb(lvglDisplay, DesktopGLDriver::FlushCallbackStatic);
  lv_display_set_buffers(lvglDisplay, DesktopGLDriver::s_drawBuf1, nullptr,
                         sizeof(DesktopGLDriver::s_drawBuf1), // Size in bytes
                         LV_DISPLAY_RENDER_MODE_PARTIAL); // Or FULL or DIRECT
  // Adjust buffer mode as needed! Partial is often good.
  lv_display_set_user_data(lvglDisplay, platformDriver.get());

  InitializeTheme();
  lvglUI = std::make_unique<LvglUI>();
  if (!lvglCore || !platformDriver || !lvglUI) {
    SDL_Log("Failed to create LVGL components");
    return -1;
  }

  platformDriver->Initialize();

  GLuint uiTextureID = platformDriver->GetTextureId();

  //   auto partialScreenshotHandler = std::bind(
  //       ScreenshotHandler, frameBuffer, FRAMEBUFFER_WIDTH,
  //       FRAMEBUFFER_HEIGHT);

  // std::shared_ptr<VirtualUI> virtualUI = std::make_shared<VirtualUI>(window,
  // displayTextureID, nullptr);
  // centralDB.SetUint32(Attribute::ID::REMOTE_LCD_BRIGHTNESS, 75);

  std::unique_ptr<VirtualUI> virtualUI =
      std::make_unique<VirtualUI>(window, uiTextureID);

  auto buttonCallback = [&](ButtonID id, ButtonState state) { // Use shared ID
    if (lvglUI) {
      lvglUI->TriggerButtonEvent(id, state);
      std::string command = CreateSerialCommand(id, state);
      SendSerialCommand(command);
    }
  };
  virtualUI->SetButtonClickHandler(buttonCallback);

  // glClearColor(0.45f, 0.55f, 0.60f, 1.00f);

  std::string serialPort = "/dev/ttyACM0"; // <--- CHANGE TO YOUR RP2040 PORT
  if (!OpenAndConfigureSerial(serialPort)) {
    // Optionally continue without serial, or exit
    SDL_LogWarn(
        SDL_LOG_CATEGORY_APPLICATION,
        "Could not open serial port, continuing without hardware link.");
    // return -1; // Or exit
  }

  bool done = false;
  uint64_t lastTickUpdateMs = SDL_GetTicks();

  bool appIsRunning = true;
  const int frames = 60;
  SDL_Event events;
  while (appIsRunning) {

    while (SDL_PollEvent(&events)) {
      ImGui_ImplSDL3_ProcessEvent(&events);

      if (events.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED ||
          (events.type == SDL_EVENT_KEY_DOWN &&
           events.key.key == SDLK_ESCAPE)) {
        appIsRunning = false;
      }

      if (events.type == SDL_EVENT_KEY_DOWN) {
        // Ensure lvglUI object exists before trying to trigger events
        // handleLvglKeys(lvglUI, events);
      }
    }

    ReadAndPrintSerial();

    uint64_t nowMs = SDL_GetTicks();
    uint64_t deltaMs = nowMs - lastTickUpdateMs;
    if (deltaMs > 0) {
      // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "TickInc Delta: %llu ms",
      // deltaMs);
      LvglCore::tick_inc(static_cast<uint32_t>(deltaMs));
      lastTickUpdateMs = nowMs; // Update baseline
    }

    // ImGui_ImplOpenGL3_NewFrame();
    // ImGui_ImplSDL3_NewFrame();

    // ImGui::NewFrame();

    // ImVec2 lvglImagePosScreen;
    // ImVec2 lvglImageSize = {(float)platformDriver->GetHorRes(),
    //                         (float)platformDriver->GetVerRes()};
    // ImGui::Begin("LVGL Viewport");
    // lvglImagePosScreen = ImGui::GetCursorScreenPos();
    // ImGui::Image(static_cast<ImTextureID>(uiTextureID), lvglImageSize);
    // ImGui::End();
    // platformDriver->UpdateDisplayContext(lvglImagePosScreen.x,
    //                                      lvglImagePosScreen.y);
    // ImGui::EndFrame();

    // platformDriver->UpdateDisplayContext(lvglImagePosScreen.x,
    // lvglImagePosScreen.y);

    LvglCore::timer_handler();

    glClear(GL_COLOR_BUFFER_BIT);
    virtualUI->Render(button, knobValue, debugOverlayEnabled);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frames));
  }

  // Shutdown(window);
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DestroyContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

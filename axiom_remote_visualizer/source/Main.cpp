#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <thread>

#include <SDL2/SDL.h>

extern "C"
{
    #include "axiom_remote_firmware/globals.h"
    #include "axiom_remote_firmware/menu.h"
    #include "axiom_remote_firmware/gfxfont.h"

    #include "axiom_remote_firmware/fonts/FreeSans9pt7b.h"
    #include "axiom_remote_firmware/fonts/FreeSans12pt7b.h"
    #include "axiom_remote_firmware/fonts/FreeSans18pt7b.h"
    #include "axiom_remote_firmware/fonts/FreeSans24pt7b.h"
}

void Shutdown(SDL_Window* win, SDL_Renderer* ren)
{
    if(ren != nullptr)
    {
        SDL_DestroyRenderer(ren);
    }

    if(win != nullptr)
    {
        SDL_DestroyWindow(win);
    }

    SDL_Quit();
}

void Initialization(SDL_Window** win, SDL_Renderer** renderer)
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }

    *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_OPENGL);
    if (*win == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        Shutdown(nullptr, nullptr);
        exit(1);
    }

    *renderer = SDL_CreateRenderer(*win, -1, 0);
    if (*renderer == nullptr)
    {
        Shutdown(*win, nullptr);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(2);
    }

//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
//    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

//    SDL_GLContext glContext = SDL_GL_CreateContext(*win);
}

static uint8_t* frameBuffer = new uint8_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 3];

void RenderDisplay(uint8_t* frameBuffer, int width, int height)
{
    draw_menu();
    //draw_page();

    unsigned int j = 0;
    for(int yIndex = 0; yIndex < height; ++yIndex)
    {
        for(int xIndex = 0; xIndex < width; xIndex++)
        {
            uint16_t val = framebuffer[xIndex][yIndex];
            frameBuffer[j] = (val >> 11) << 3;
            frameBuffer[j + 1] = ((val >> 5) & 0x3F) << 2;
            frameBuffer[j + 2] = (val & 0x1F) << 3;
            j += 3;//(xIndex + 2) * yIndex + yIndex;
        }
    }
}

int main()
{
    std::cout<<"AXIOM Remote Visualizer" << std::endl;

    SDL_Window* window;
    SDL_Renderer* renderer;
    Initialization(&window, &renderer);

    _FreeSans9pt7b = FreeSans9pt7b;
    _FreeSans12pt7b = FreeSans12pt7b;
    _FreeSans18pt7b = FreeSans18pt7b;
    _FreeSans24pt7b = FreeSans24pt7b;

    init_menus();
    //draw_menu();

    RenderDisplay(frameBuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    uint32_t rmask = 0x000000ff;
    uint32_t gmask = 0x0000ff00;
    uint32_t bmask = 0x00ff0000;
    uint32_t amask = 0xff000000;

    SDL_Surface* surface;
    SDL_Texture* texture;
    surface = SDL_CreateRGBSurfaceFrom(frameBuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 24, 3 * FRAMEBUFFER_WIDTH, rmask, gmask, bmask, amask);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    void* textureData;
    int pitch = 0;

    SDL_Rect texture_rect;
    texture_rect.x = 30;  //the x coordinate
    texture_rect.y = 30; // the y coordinate
    texture_rect.w = FRAMEBUFFER_WIDTH; //the width of the texture
    texture_rect.h = FRAMEBUFFER_HEIGHT; //the height of the texture

    SDL_SetRenderDrawColor(renderer, 0, 60, 80, 255);

    bool appIsRunning = true;
    const int frames = 30;
    SDL_Event events;
    while ( appIsRunning )
    {
        if(SDL_PollEvent(&events))
        {
            if(events.type == SDL_QUIT || (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_ESCAPE))
            {
                appIsRunning = false;
            }
        }

        SDL_RenderClear(renderer);
        RenderDisplay(frameBuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

        SDL_LockTexture( texture, nullptr, &textureData, &pitch);
        memcpy(textureData, frameBuffer, FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 3);
        SDL_UnlockTexture( texture );

        SDL_RenderCopy(renderer, texture, nullptr, &texture_rect);
        SDL_RenderPresent(renderer); //updates the renderer

        std::this_thread::sleep_for( std::chrono::milliseconds(1000/frames) );
    }

    Shutdown(window, renderer);
    return 0;
}

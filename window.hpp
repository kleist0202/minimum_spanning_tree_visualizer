#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <functional>
#include <chrono>
#include <thread>
#include <ctime>

namespace FastGui {

class Window {
public:
    int width;
    int height;
    bool isWorking;
    SDL_Renderer *renderer = NULL; 
    SDL_Window *window = NULL;    
    SDL_Surface *surface;
    SDL_Event event; 
    SDL_Color screenColor;
    const char* windowTitle;
    Uint32 flags;
    bool isFullscreen;
    bool isResizable;
    int delay;
    time_t startTime;

    // input variables
    int mouse_x;
    int mouse_y;
    bool leftButtonPressed;

    // fps variables
    int cSec;
    int cFrame;
    int fps;
    float delta;

public:
    Window();
    Window(int width_, int height_);
    ~Window();

    SDL_Event* getEvent();
    SDL_Renderer* getRenderer();
    int getFps() const;
    time_t getStartTime() const;

    void setScreenColor(Uint8 r, Uint8 g, Uint8 b);
    void setWindowTitle(const char* title);

    void window_init();
    void windowShouldClose();

    void setFullscreen(bool fullscreen);
    void setResizable(bool resizable);

    void setDelay(int d);
    void calcFps();
    bool isLeftButtonPressed();

    template<class Function, class... Args> 
    void main_loop(Function&& f, Args&&... args) {
        if (isResizable)
            flags |= SDL_WINDOW_RESIZABLE;
        if (isFullscreen)
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

        SDL_CreateWindowAndRenderer(width, height, flags, &window, &renderer);
        SDL_SetWindowTitle(window, windowTitle);

        auto func = std::bind(f, std::ref(args)...);

        Uint8 buttons;

        while(isWorking) {
            while (SDL_PollEvent(&event) > 0)
            {
                SDL_PumpEvents();

                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    windowShouldClose();
                }


                buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
                if ((buttons & SDL_BUTTON_LMASK) != 0) {
                    leftButtonPressed = true;
                }
                else {
                    leftButtonPressed = false;
                }

                switch (event.type) {
                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                            SDL_GetWindowSize(window , &width , &height);
                        }
                        break;

                    case SDL_QUIT:
                        windowShouldClose();
                        break;
                    default:
                        break;
                }
            }

            calcFps();
            SDL_SetRenderDrawColor(renderer, screenColor.r, screenColor.g, screenColor.b, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            func();

            // sleep
            SDL_Delay(delay);

            SDL_RenderPresent(renderer);
        }
    }
};

void drawCircle(SDL_Renderer* renderer, const SDL_Color& color, int x, int y, int r);
void drawLine(SDL_Renderer* renderer, const SDL_Color&, int x_start, int y_start, int x_end, int y_end);

}

#endif // WINDOW_HPP

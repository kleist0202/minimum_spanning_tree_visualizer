#include "window.hpp"
#include <SDL2/SDL_log.h>
#include <iostream>

namespace FastGui {

Window::Window(): width(800), height(600) { window_init(); };
Window::Window(int width_, int height_): width(width_), height(height_) { window_init(); }

Window::~Window(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Event* Window::getEvent() { return &event; }
SDL_Renderer* Window::getRenderer() { return renderer; }
int Window::getFps() const { return fps; }
time_t Window::getStartTime() const { return startTime; }
Vetctor2d Window::getMousePos() const { return mouse_pos; }

void Window::setScreenColor(u_int8_t r, u_int8_t g, u_int8_t b) {
    screenColor.r = r;
    screenColor.g = g;
    screenColor.b = b;
}

void Window::setWindowTitle(const char* title) {
    windowTitle = title;
}

void Window::window_init() {
    isWorking = true;
    windowTitle = "sdl";
    screenColor = {0, 0, 0};
    flags = 0;
    isResizable = false;
    isFullscreen = false;
    startTime = time(NULL);

    SDL_Init(SDL_INIT_VIDEO); 
}

void Window::windowShouldClose() {
    isWorking = false;
}

void Window::setFullscreen(bool fullscreen) {
    isFullscreen = fullscreen;
}

void Window::setResizable(bool resizable) {
    isResizable = resizable;
}

void Window::setDelay(int d) {
    delay = d;
}

void Window::calcFps() {
    if (cSec == SDL_GetTicks()/1000) {
        cFrame++;
    }
    else {
        fps = cFrame;
        cFrame = 0;
        cSec = SDL_GetTicks()/1000;
        if (fps > 0)
            delta = 1.0 / fps;
    }
} 

bool Window::isLeftButtonPressed() {
    return leftButtonPressed;
}

bool Window::keyPressed(SDL_Keycode key) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == key) {
        return true;
    }
    return false;
}

void Window::main_loop(Program& program) {
    if (isResizable)
        flags |= SDL_WINDOW_RESIZABLE;
    if (isFullscreen)
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    SDL_CreateWindowAndRenderer(width, height, flags, &window, &renderer);
    SDL_SetWindowTitle(window, windowTitle);

    program.init();

    Uint8 buttons;

    while(isWorking) {
        while (SDL_PollEvent(&event) > 0)
        {
            SDL_PumpEvents();

            program.keyboardInput();

            buttons = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
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
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        windowShouldClose();
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
        program.run();

        // sleep
        SDL_Delay(delay);

        SDL_RenderPresent(renderer);
    }
}

void drawCircle(SDL_Renderer* renderer, const SDL_Color& color, int x, int y, int r) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    for (int i = -r; i < r; ++i) {
        for (int j = -r; j < r; ++j) {
            if (i < sqrt(r*r - j*j) and -i < sqrt(r*r - j*j))
                SDL_RenderDrawPoint(renderer, i+x, j+y);
        }    
    }
}

void drawLine(SDL_Renderer* renderer, const SDL_Color& color, int x_start, int y_start, int x_end, int y_end) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderDrawLine(
            renderer,
            x_start,
            y_start,
            x_end,
            y_end
    );
}

}

#include "window.hpp"

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

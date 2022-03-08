#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <functional>
#include <chrono>
#include <thread>
#include <vector>
#include <ctime>
#include "program.hpp"

namespace FastGui {

struct Vetctor2d {
    int x;
    int y;
};

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
    Vetctor2d mouse_pos;
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
    Vetctor2d getMousePos() const;

    void setScreenColor(Uint8 r, Uint8 g, Uint8 b);
    void setWindowTitle(const char* title);

    void window_init();
    void windowShouldClose();

    void setFullscreen(bool fullscreen);
    void setResizable(bool resizable);

    void setDelay(int d);
    void calcFps();
    bool isLeftButtonPressed();
    bool keyPressed(SDL_Keycode key);

    //template<class Function, class... Args> 
    //void main_loop(Function&& f, Args&&... args) {
    void main_loop(Program& program);
};

void drawCircle(SDL_Renderer* renderer, const SDL_Color& color, int x, int y, int r);
void drawLine(SDL_Renderer* renderer, const SDL_Color&, int x_start, int y_start, int x_end, int y_end);

}

#endif // WINDOW_HPP

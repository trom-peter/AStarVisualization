#pragma once
#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL.h>
#include "visualization/gui.h"

class InputHandler;
class GUI;

class Window {
public:
    Window(const char* title);

    bool init();
    void swapBuffers();
    void quit();
    void updateTime();
    int getWidth();
    int getHeight();
    float getDelta();
    float getTime();
    SDL_Window* getWindow();
    SDL_GLContext getGLContext();

private:
    void setupOpenGLAttributes();
    void setupTimeCount();

    const char* title;
    int width, height;
    SDL_Window* window;
    SDL_GLContext glContext;
    uint64_t perfCounterFrequency;
    uint64_t lastCounter;
    float time;
    float delta;
    int FPS;
};
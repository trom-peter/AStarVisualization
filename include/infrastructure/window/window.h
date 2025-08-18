#pragma once
#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL.h>
#include "gui/gui.h"

class GUI;

class Window {
public:
    Window(const char* title);

    bool init();
    void swapBuffers() const;
    void quit() const;
    void updateTime();
    int getWidth() const;
    int getHeight() const;
    float getDelta() const;
    float getTime() const;
    SDL_Window* getWindow() const;
    SDL_GLContext getGLContext() const;

private:
    void setupOpenGLAttributes() const;
    void setupTimeCount();

    const char* title; // Title of window
    int width, height; // Window width, height
    SDL_Window* window;
    SDL_GLContext glContext;
    uint64_t perfCounterFrequency;
    uint64_t lastCounter;
    float time; // Time since start of application
    float delta; // Passed time since last frame
};

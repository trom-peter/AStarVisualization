#include "window.h"
#define SDL_MAIN_HANDLED
//#define SHOW_FPS

Window::Window(int width, int height, const char* title) : title(title), window(nullptr), glContext(nullptr) {
    this->width = width;
    this->height = height;
}

bool Window::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    setupOpenGLAttributes();

    SDL_SetRelativeMouseMode(SDL_FALSE);

    setupTimeCount();

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // 4x Multisampling

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
    if (!window) {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1); // Activate V-Sync

    return true;
}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(window);
}

void Window::quit() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::setupOpenGLAttributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); //set up depth buffer size
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


    #ifdef _DEBUG
    std::cout << "Running in debug mode" << std::endl;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    #endif
}

void Window::setupTimeCount() {
    perfCounterFrequency = SDL_GetPerformanceFrequency();
    lastCounter = SDL_GetPerformanceCounter();
    delta = 0.0f;
    time = 0.0f;
    FPS = 0;
}

void Window::updateTime() {
    uint64_t endCounter = SDL_GetPerformanceCounter();
    uint64_t counterElapsed = SDL_GetPerformanceCounter() - lastCounter;
    delta = (float)counterElapsed / (float)perfCounterFrequency;
    FPS = (int)((float)perfCounterFrequency / (float)counterElapsed);
#ifdef SHOW_FPS
    std::cout << FPS << std::endl;
#endif
    lastCounter = endCounter;
    time += delta;
}

int Window::getWidth() {
    return width;
}

int Window::getHeight() {
    return height;
}

float Window::getDelta() {
    return delta;
}

float Window::getTime() {
    return time;
}

SDL_Window* Window::getWindow() {
    return window;
}

SDL_GLContext Window::getGLContext() {
    return glContext;
}

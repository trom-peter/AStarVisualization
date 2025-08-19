#include "infrastructure/opengl/base_renderer.h"
#include "infrastructure/opengl/index_buffer.h"
#include "infrastructure/opengl/camera.h"

void GLAPIENTRY openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, 
    GLsizei length, const GLchar* message, const void* userParam) 
{
    std::cout << "[OpenGL Error " << id << "] " << message << std::endl;
}

BaseRenderer::BaseRenderer() {}

bool BaseRenderer::init() {
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

#ifdef _DEBUG
    setupGLDebugCallback();
#endif

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);  // Specify that the front faces are wound counter clock wise (CCW)    
    glCullFace(GL_BACK);  // Cull back faces
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

    return true;
}

// Clear background with the set clearColor
void BaseRenderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BaseRenderer::setClearColor(const GLclampf r, const GLclampf g, const GLclampf b) {
    glClearColor(r, g, b, 1.0f);
}

void BaseRenderer::setupGLDebugCallback() {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openGLDebugCallback, 0);
}

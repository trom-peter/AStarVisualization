#pragma once
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "infrastructure/opengl/shader.h"
#include "infrastructure/opengl/vertex_array.h"
#include "glm/glm.hpp"

class Camera;
class VertexArray;
struct Shader;

class BaseRenderer {
public:
    BaseRenderer();

    static bool init();

    static void clear();

    static void setClearColor(const GLclampf r, const GLclampf g, const GLclampf b);

protected:
	std::unique_ptr<Shader> shader;
	std::unique_ptr<VertexArray> vao;

private:
    static void setupGLDebugCallback();
};
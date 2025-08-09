#pragma once
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "opengl/shader.h"
#include "opengl/vertex_array.h"
#include "glm/glm.hpp"

class Camera;
class VertexArray;
struct Shader;

class BaseRenderer {
public:
    BaseRenderer();

    static bool init();

    static void clear();

    static void setClearColor(GLclampf r, GLclampf g, GLclampf b);

protected:
	std::unique_ptr<Shader> shader;
	std::unique_ptr<VertexArray> vao;

private:
    static void setupGLDebugCallback();
};
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

    void clear();

    void setClearColor(GLclampf r, GLclampf g, GLclampf b);

    virtual void setupUniforms() = 0;

    virtual void updateUniforms(Camera* camera, glm::mat4 model = glm::mat4(1.0f)) = 0;

protected:
	std::unique_ptr<Shader> shader;
	std::unique_ptr<VertexArray> vao;

private:
    static void setupGLDebugCallback();

};
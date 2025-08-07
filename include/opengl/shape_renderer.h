#pragma once
#include "opengl/base_renderer.h"

struct Shape;
class Camera;

class ShapeRenderer : public BaseRenderer {

public:
    ShapeRenderer();

    void setupUniforms() override;

    void updateUniforms(Camera* camera, glm::mat4 model = glm::mat4(1.0f)) override;

    void draw(Shape* shape);

    glm::vec3 getColor();

    void setColor(glm::vec3 color);

private:
    glm::vec3 color;
};
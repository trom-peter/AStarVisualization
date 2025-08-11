#pragma once
#include "opengl/base_renderer.h"
#include "model/state_grid.h"
#include "sphere.h"

class Camera;

class StategridRenderer : public BaseRenderer {

public:
    StategridRenderer(Stategrid& stategrid);

    void setStategrid(Stategrid& stategrid);
    void setupUniforms(Camera* camera);
    void updateUniforms(Camera* camera, glm::mat4 model = glm::mat4(1.0f), glm::vec3 color = glm::vec3(0.0f));
    void drawStategrid(Camera* camera);

private:
    Sphere stateSphere;
    Stategrid& stategrid;
};
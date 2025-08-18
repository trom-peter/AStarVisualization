#pragma once
#include "infrastructure/opengl/base_renderer.h"
#include "model/state_grid.h"
#include "infrastructure/geometry/sphere.h"

class Camera;

class StategridRenderer : public BaseRenderer {

public:
    StategridRenderer(Stategrid& stategrid);

    void setStategrid(const Stategrid& stategrid);
    void setupUniforms(const Camera& camera) const;
    void updateUniforms(const Camera& camera, const glm::mat4 model = glm::mat4(1.0f), 
        const glm::vec3 color = glm::vec3(0.0f)) const;
    void drawStategrid(const Camera& camera);

private:
    Sphere stateSphere;
    Stategrid& stategrid;
};
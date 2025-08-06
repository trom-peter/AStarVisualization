#pragma once

#include "topography.h"
#include "base_renderer.h"

class Topography;
class Camera;

class TopographyRenderer : public BaseRenderer {

public:
    TopographyRenderer(float amplitude);

    void setupUniforms() override;

    void updateUniforms(Camera* camera, glm::mat4 model = glm::mat4(1.0f)) override;

    void draw(Topography* topography);

    void setAmplitude(float amplitude);
private:
    float amplitude;
};
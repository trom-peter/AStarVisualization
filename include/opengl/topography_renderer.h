#pragma once
#include "opengl/base_renderer.h"
#include "opengl/mesh.h"
#include "model/topography.h"

class Topography;
class Camera;

class TopographyRenderer : public BaseRenderer {

public:
    TopographyRenderer(Topography& topography);

    void setTopography(Topography& topography);
    void draw();
    void setupUniforms(Camera* camera);
    void updateUniforms(Camera* camera, glm::mat4 model = glm::mat4(1.0f));

private:
    std::vector<Vertex> generateVertices();
    std::vector<uint32_t> generateIndices();
    void calculateNormals(std::vector<Vertex>& vertices, std::vector<uint32_t> indices);

    Topography& topography;
    Mesh* topographyMesh;
};
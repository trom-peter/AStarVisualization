#pragma once
#include <memory>
#include "infrastructure/opengl/base_renderer.h"
#include "infrastructure/opengl/mesh.h"
#include "model/topography.h"

class Topography;
class Camera;

class TopographyRenderer : public BaseRenderer {
public:
    TopographyRenderer(Topography& topography);
    ~TopographyRenderer();

    void setTopography(Topography& topography);
    void drawTopography() const;
    void setupUniforms(const Camera& camera) const;
    void updateUniforms(const Camera& camera, const glm::mat4 model = glm::mat4(1.0f)) const;

private:
    std::vector<Vertex> generateVertices() const;
    std::vector<uint32_t> generateIndices() const;
    void calculateNormals(std::vector<Vertex>& vertices, std::vector<uint32_t> indices) const;

    Topography& topography;
    std::unique_ptr<Mesh> topographyMesh;
};

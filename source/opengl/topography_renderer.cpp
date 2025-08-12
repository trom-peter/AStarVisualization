#include "opengl/topography_renderer.h"
#include "model/topography.h"
#include "opengl/camera.h"

TopographyRenderer::TopographyRenderer(Topography& topography) :
    topography(topography), topographyMesh(nullptr), BaseRenderer() 
{
    shader = std::unique_ptr<Shader>(new Shader("shaders/topography.vert", "shaders/topography.frag"));
    shader->bind();

    vao = std::unique_ptr<VertexArray>(new VertexArray());
    vao->bind();
    vao->setupVertexLayout();

    setTopography(topography);
}

TopographyRenderer::~TopographyRenderer() {
    delete topographyMesh;
}

void TopographyRenderer::setTopography(Topography& topography) {
    this->topography = topography;
    if (topographyMesh != nullptr) delete topographyMesh;
    std::vector<Vertex> vertices = generateVertices();
    std::vector<uint32_t> indices = generateIndices();
    calculateNormals(vertices, indices);
    topographyMesh = new Mesh(vertices, vertices.size(), indices, indices.size());
}

void TopographyRenderer::drawTopography() {
    if (topographyMesh == nullptr) {
        std::cerr << "ERROR: no topography available to render" << std::endl;
        return;
    }
    vao->addVertexBuffer(*topographyMesh->vertexBuffer);
    shader->bind();
    topographyMesh->vertexBuffer->bind();
    topographyMesh->indexBuffer->bind();
    glDrawElements(GL_TRIANGLES, topographyMesh->numIndices, GL_UNSIGNED_INT, 0);
    topographyMesh->indexBuffer->unbind();
    topographyMesh->vertexBuffer->unbind();
    shader->unbind();
    vao->unbind();
}

void TopographyRenderer::setupUniforms(Camera* camera) {
    shader->bind();
    glm::mat4 view = camera->getView();
    glm::mat4 proj = camera->getProj();
    shader->setUniformMatrix4fv("u_view", 1, GL_FALSE, view);
    shader->setUniformMatrix4fv("u_projection", 1, GL_FALSE, proj);
    shader->setUniform1f("u_amplitude", topography.getAmplitude());
}

void TopographyRenderer::updateUniforms(Camera* camera, glm::mat4 model) {
    shader->bind();
    shader->setUniformMatrix4fv("u_model", 1, GL_FALSE, model);
    glm::mat4 view = camera->getView();
    glm::mat4 proj = camera->getProj();

    shader->setUniformMatrix4fv("u_model", 1, GL_FALSE, model);
    shader->setUniformMatrix4fv("u_view", 1, GL_FALSE, view);
    shader->setUniformMatrix4fv("u_projection", 1, GL_FALSE, proj);
}

std::vector<Vertex> TopographyRenderer::generateVertices() {
    std::vector<Vertex> vertices;
    for (double z = 0; z < topography.getSize(); z += topography.getSpacing()) {
        for (double x = 0; x < topography.getSize(); x += topography.getSpacing()) {
            Vertex v;
            float y = topography.getY(x, z);
            v.position = glm::vec3(x, y, z);
            vertices.push_back(v);
        }
    }

    return vertices;
}

std::vector<uint32_t> TopographyRenderer::generateIndices() {
    std::vector<uint32_t> indices;
    int lastX = topography.getSize() / topography.getSpacing() - 1;
    int lastZ = topography.getSize() / topography.getSpacing() - 1;
    for (int z = 0; z < lastZ; z++) {
        for (int x = 0; x < lastX; x++) {
            int topLeft = (z + 1) * (lastX + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = z * (lastX + 1) + x;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(topLeft);

            indices.push_back(topRight);
            indices.push_back(bottomRight);
            indices.push_back(bottomLeft);
        }
    }
    return indices;
}

void TopographyRenderer::calculateNormals(std::vector<Vertex>& vertices, std::vector<uint32_t> indices) {
    for (size_t i = 0; i < indices.size(); i += 3) {
        uint32_t i0 = indices[i];
        uint32_t i1 = indices[i + 1];
        uint32_t i2 = indices[i + 2];

        glm::vec3 p0 = vertices[i0].position;
        glm::vec3 p1 = vertices[i1].position;
        glm::vec3 p2 = vertices[i2].position;

        glm::vec3 u(p1 - p0);
        glm::vec3 v(p2 - p0);

        glm::vec3 normal = glm::normalize(glm::cross(u, v));

        vertices[i0].normal = normal;
        vertices[i1].normal = normal;
        vertices[i2].normal = normal;
    }
}

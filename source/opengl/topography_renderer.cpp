#include "opengl/topography_renderer.h"
#include "topography.h"
#include "opengl/camera.h"

TopographyRenderer::TopographyRenderer(float amplitude) : amplitude(amplitude), BaseRenderer() {
    shader = std::unique_ptr<Shader>(new Shader("shaders/topography.vert", "shaders/topography.frag"));
    shader->bind();

    vao = std::unique_ptr<VertexArray>(new VertexArray());
    vao->bind();
    vao->setupVertexLayout();
}

void TopographyRenderer::draw(Topography* topography) {
    vao->addVertexBuffer(*topography->getMesh()->vertexBuffer);
    shader->bind();
    topography->getMesh()->vertexBuffer->bind();
    topography->getMesh()->indexBuffer->bind();
    glDrawElements(GL_TRIANGLES, topography->getMesh()->numIndices, GL_UNSIGNED_INT, 0);
    topography->getMesh()->indexBuffer->unbind();
    topography->getMesh()->vertexBuffer->unbind();
    shader->unbind();
    vao->unbind();
}

void TopographyRenderer::setupUniforms() {
    shader->bind();
    shader->setUniform1f("u_amplitude", amplitude);
}

void TopographyRenderer::updateUniforms(Camera* camera, glm::mat4 model) {
    shader->bind();
    glm::mat4 view = camera->getView();
    glm::mat4 proj = camera->getProj();

    shader->setUniformMatrix4fv("u_model", 1, GL_FALSE, model);
    shader->setUniformMatrix4fv("u_view", 1, GL_FALSE, view);
    shader->setUniformMatrix4fv("u_projection", 1, GL_FALSE, proj);
}

void TopographyRenderer::setAmplitude(float amplitude) {
    this->amplitude = amplitude;
}

#include "opengl/shape_renderer.h"
#include "shape.h"
#include "opengl/camera.h"

ShapeRenderer::ShapeRenderer() : color(glm::vec3(0.0f)), BaseRenderer() {
    shader = std::unique_ptr<Shader>(new Shader("shaders/3dshape.vert", "shaders/3dshape.frag"));
    shader->bind();

    vao = std::unique_ptr<VertexArray>(new VertexArray());
    vao->bind();
    vao->setupVertexLayout();
}

void ShapeRenderer::draw(Shape* shape) {
    vao->addVertexBuffer(*shape->mesh->vertexBuffer);
    shader->bind();
    shape->mesh->vertexBuffer->bind();
    shape->mesh->indexBuffer->bind();
    glDrawElements(GL_TRIANGLES, shape->mesh->numIndices, GL_UNSIGNED_INT, 0);
    shape->mesh->vertexBuffer->unbind();
    shape->mesh->indexBuffer->unbind();
    shader->unbind();
    vao->unbind();
}

void ShapeRenderer::setupUniforms() {
    shader->bind();
    shader->setUniform4fv("u_material.ambient", 1, glm::vec4(0.25, 0.25, 0.28, 1.0));
    shader->setUniform4fv("u_material.diffuse", 1, glm::vec4(1.3, 1.3, 1.0, 1.0));
    shader->setUniform4fv("u_material.specular", 1, glm::vec4(0.4, 0.4, 0.4, 1.0));
    shader->setUniform1f("u_material.shininess", 500);
}

void ShapeRenderer::updateUniforms(Camera* camera, glm::mat4 model) {
    shader->bind();

    glm::mat4 view = camera->getView();
    glm::mat4 proj = camera->getProj();

    shader->setUniform3f("u_color", color.x, color.y, color.z);
    shader->setUniformMatrix4fv("u_model", 1, GL_FALSE, model);
    shader->setUniformMatrix4fv("u_view", 1, GL_FALSE, view);
    shader->setUniformMatrix4fv("u_projection", 1, GL_FALSE, proj);
}

glm::vec3 ShapeRenderer::getColor() {
    return color;
}

void ShapeRenderer::setColor(glm::vec3 color) {
    this->color = color;
}

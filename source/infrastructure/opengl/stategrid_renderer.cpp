#include "infrastructure/opengl/stategrid_renderer.h"
#include "infrastructure/geometry/shape.h"
#include "infrastructure/opengl/camera.h"

StategridRenderer::StategridRenderer(Stategrid& stategrid) : stategrid(stategrid), BaseRenderer() {
    shader = std::unique_ptr<Shader>(new Shader("shaders/3dshape.vert", "shaders/3dshape.frag"));
    shader->bind();

    vao = std::unique_ptr<VertexArray>(new VertexArray());
    vao->bind();
    vao->setupVertexLayout();
}

void StategridRenderer::setStategrid(const Stategrid& stategrid) {
    this->stategrid = stategrid;
}

/*
    Draws the stategrid state by state by updating the
    stateSphere member variable position, scale, color for every state.
*/
void StategridRenderer::drawStategrid(const Camera& camera) {
    if (stategrid.grid.size() == 0) {
        return;
    }

    //setup rendering
    vao->addVertexBuffer(*stateSphere.mesh->vertexBuffer);
    shader->bind();
    stateSphere.mesh->vertexBuffer->bind();
    stateSphere.mesh->indexBuffer->bind();

    //render stategrid
    for (std::pair<const State, glm::vec3>& kv : stategrid.grid) {
        glm::vec3 stateColor = kv.second;

        if (!stategrid.isVisible(stateColor)) // only draw visible states
            continue;

        //update state sphere
        stateSphere.setColor(stateColor);
        stateSphere.setPosition(glm::vec3(kv.first.x, kv.first.y + 40.0f, kv.first.z));
        stateSphere.setScale(glm::vec3(1000.0f / stategrid.gridSize));

        updateUniforms(camera, stateSphere.model, stateSphere.color);

        glDrawElements(GL_TRIANGLES, stateSphere.mesh->numIndices, GL_UNSIGNED_INT, 0);
    }

    //end rendering
    stateSphere.mesh->vertexBuffer->unbind();
    stateSphere.mesh->indexBuffer->unbind();
    shader->unbind();
    vao->unbind();
}

void StategridRenderer::setupUniforms(const Camera& camera) const {
    shader->bind();

    glm::mat4 view = camera.getView();
    glm::mat4 proj = camera.getProj();

    shader->setUniformMatrix4fv("u_view", 1, GL_FALSE, view);
    shader->setUniformMatrix4fv("u_projection", 1, GL_FALSE, proj);
}

void StategridRenderer::updateUniforms(const Camera& camera, const glm::mat4 model, 
    const glm::vec3 color) const 
{
    shader->bind();
    glm::mat4 proj = camera.getProj();
    glm::mat4 view = camera.getView();

    shader->setUniform3f("u_color", color.x, color.y, color.z);

    shader->setUniformMatrix4fv("u_model", 1, GL_FALSE, model);
    shader->setUniformMatrix4fv("u_view", 1, GL_FALSE, view);
    shader->setUniformMatrix4fv("u_projection", 1, GL_FALSE, proj);
}

#include "infrastructure/opengl/stategrid_renderer.h"
#include "infrastructure/opengl/camera.h"
#include "infrastructure/geometry/shape.h"

constexpr const char* VERTEX_SHADER_LOCATION = "shaders/3dshape.vert";
constexpr const char* FRAGMENT_SHADER_LOCATION = "shaders/3dshape.frag";

// StateSphere height offset from terrain ground
constexpr int SPHERE_Y_OFFSET = 40;

// Determines how big the stateSphere should be in relation to the grid resolution
constexpr int SPHERE_GRID_SCALING = 1000; 

StategridRenderer::StategridRenderer(Stategrid& stategrid) : stategrid(stategrid), BaseRenderer() {
    shader = std::make_unique<Shader>(VERTEX_SHADER_LOCATION, FRAGMENT_SHADER_LOCATION);
    shader->bind();

    vao = std::make_unique<VertexArray>();
    vao->bind();
}

void StategridRenderer::setStategrid(const Stategrid& stategrid) {
    this->stategrid = stategrid;
}

/*
    Draws the stategrid state by state by updating the stateSphere 
    member variable position, scale, color for every state.
*/
void StategridRenderer::drawStategrid(const Camera& camera) {
    if (stategrid.grid.size() == 0) {
        return;
    }

    // Setup rendering
    vao->addVertexBuffer(*stateSphere.mesh->vertexBuffer);
    shader->bind();
    stateSphere.mesh->vertexBuffer->bind();
    stateSphere.mesh->indexBuffer->bind();

    // Render stategrid
    for (std::pair<const State, glm::vec3>& kv : stategrid.grid) {
        glm::vec3 stateColor = kv.second;

        if (!stategrid.isVisible(stateColor)) { // Only draw visible states
            continue;
        }

        // Update state sphere
        stateSphere.setColor(stateColor);
        stateSphere.setPosition(glm::vec3(kv.first.x, kv.first.y + SPHERE_Y_OFFSET, kv.first.z));
        stateSphere.setScale(glm::vec3(SPHERE_GRID_SCALING / stategrid.gridResolution));

        updateUniforms(camera, stateSphere.model, stateSphere.color);

        glDrawElements(GL_TRIANGLES, stateSphere.mesh->numIndices, GL_UNSIGNED_INT, 0);
    }

    // End rendering
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

void StategridRenderer::updateUniforms(const Camera& camera, 
    const glm::mat4 model, const glm::vec3 color) const 
{
    shader->bind();
    glm::mat4 proj = camera.getProj();
    glm::mat4 view = camera.getView();

    shader->setUniform3f("u_color", color.x, color.y, color.z);

    shader->setUniformMatrix4fv("u_model", 1, GL_FALSE, model);
    shader->setUniformMatrix4fv("u_view", 1, GL_FALSE, view);
    shader->setUniformMatrix4fv("u_projection", 1, GL_FALSE, proj);
}

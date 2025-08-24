#pragma once
#include "glm/glm.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;

    Vertex() : position(glm::vec3(0.0f)), normal(glm::vec3(0.0f)) {}

	Vertex(const glm::vec3 position, const glm::vec3 normal) : 
        position(position), normal(normal) {}
};

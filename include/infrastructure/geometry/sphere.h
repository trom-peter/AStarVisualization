#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "infrastructure/geometry/vertex.h"
#include "infrastructure/geometry/shape.h"

struct Sphere : Shape {
    Sphere(const float radius = 1.0f, const int sectorCount = 36, const int stackCount = 18,
        const glm::vec3 position = glm::vec3(0.0f),
        const glm::vec3 scale = glm::vec3(1.0f), 
        const glm::vec3 color = glm::vec3(1.0f));

    std::string toString() const override;
    std::vector<Vertex> getVertices() const override;
    std::vector<uint32_t> getIndices() const override;

    float radius;
    int sectorCount;
    int stackCount;
};

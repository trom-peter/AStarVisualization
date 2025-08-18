#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "infrastructure/geometry/vertex.h"
#include "infrastructure/geometry/shape.h"

// Initial sphere configuration
float constexpr RADIUS = 1.0f;
int constexpr SECTOR_COUNT = 36;
int constexpr STACK_COUNT = 18;

struct Sphere : Shape {
    Sphere(const float radius = RADIUS, 
        const int sectorCount = SECTOR_COUNT, 
        const int stackCount = STACK_COUNT,
        const glm::vec3 position = glm::vec3(0.0f),
        const glm::vec3 scale = glm::vec3(1.0f), 
        const glm::vec3 color = glm::vec3(1.0f));

    std::string toString() const override;

    float radius; // Sphere radius
    int sectorCount; // Sector resolution
    int stackCount; // Stack resolution

private:
    // Get vertices for specific shape
    std::vector<Vertex> getVertices() const override;

    // Get indices for specific shape
    std::vector<uint32_t> getIndices() const override;
};

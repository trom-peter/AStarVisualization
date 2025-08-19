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
glm::vec3 constexpr POSITION = glm::vec3(0.0f);
glm::vec3 constexpr SCALE = glm::vec3(1.0f);
glm::vec3 constexpr COLOR = glm::vec3(1.0f);

struct Sphere : Shape {
    Sphere(const float radius = RADIUS, 
        const int sectorCount = SECTOR_COUNT, 
        const int stackCount = STACK_COUNT,
        const glm::vec3 position = POSITION,
        const glm::vec3 scale = SCALE,
        const glm::vec3 color = COLOR);

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

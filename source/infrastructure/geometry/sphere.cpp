#include "infrastructure/geometry/sphere.h"

/*
* Implementation of getVertices() and getIndices() is based on portions of the code of Eric Arnebäck.
* https://github.com/Erkaman/cute-deferred-shading/tree/master
*/

constexpr int MIN_SECTOR_COUNT = 2;
constexpr int MIN_STACK_COUNT = 2;
constexpr float MIN_RADIUS = 0.0f;

Sphere::Sphere(const float radius, const int sectorCount, const int stackCount, 
    const glm::vec3 position, const glm::vec3 scale, const glm::vec3 color) : 
    Shape(position, scale, color) 
{
    if (radius < MIN_RADIUS) {
        this->radius = MIN_RADIUS;
    }
    else {
        this->radius = radius;
    }

    if (sectorCount < MIN_SECTOR_COUNT) {
        this->sectorCount = MIN_SECTOR_COUNT;
    }
    else {
        this->sectorCount = sectorCount;
    }

    if (stackCount < MIN_STACK_COUNT) {
        this->stackCount = MIN_STACK_COUNT;
    }
    else {
        this->stackCount = stackCount;
    }

    std::vector<Vertex> vertices = getVertices();
    std::vector<uint32_t> indices = getIndices();

    mesh = std::make_unique<Mesh>(vertices, vertices.size(), indices, indices.size());
}

std::vector<Vertex> Sphere::getVertices() const {
    std::vector<Vertex> vertices;
    // Loop through stacks
    for (int i = 0; i <= stackCount; ++i) {

        float V = (float)i / (float)stackCount;
        float phi = V * M_PI;

        // Loop through the slices
        for (int j = 0; j <= sectorCount; ++j) {

            float U = (float)j / (float)sectorCount;
            float theta = U * (M_PI * 2);

            // Use spherical coordinates to calculate the positions
            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            vertices.push_back(Vertex(glm::vec3(x,y,z), glm::vec3(0.0f)));
        }
    }

    return vertices;
}

std::vector<uint32_t> Sphere::getIndices() const {
    std::vector<uint32_t> indices;

    // Calculate the indices
    for (int i = 0; i < sectorCount * stackCount + sectorCount; ++i) {
        indices.push_back(GLuint(i));
        indices.push_back(GLuint(i + sectorCount + 1));
        indices.push_back(GLuint(i + sectorCount));

        indices.push_back(GLuint(i + sectorCount + 1));
        indices.push_back(GLuint(i));
        indices.push_back(GLuint(i + 1));
    }

    return indices;
}

std::string Sphere::toString() const {
    return std::string("Sphere with radius " + std::to_string(radius));
}

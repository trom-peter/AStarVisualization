#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>
#include "opengl/vertex.h"
#include "shape.h"

struct Sphere : Shape {
    Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18,
        glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 color = glm::vec3(1.0f));

    std::string toString() const override;
    std::vector<Vertex> getVertices() const override;
    std::vector<uint32_t> getIndices() const override;

    float radius;
    int sectorCount;
    int stackCount;
};

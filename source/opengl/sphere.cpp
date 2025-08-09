#include "opengl/sphere.h"

Sphere::Sphere(float radius, int sectorCount, int stackCount, glm::vec3 position, glm::vec3 scale, glm::vec3 color) : Shape(position, scale, color) {
    const int MIN_SECTOR_COUNT = 2;
    const int MIN_STACK_COUNT = 2;

    this->radius = radius > 0.0f ? radius : 0.0f;
    this->sectorCount = sectorCount > MIN_SECTOR_COUNT ? sectorCount : MIN_SECTOR_COUNT;
    this->stackCount = stackCount > MIN_STACK_COUNT ? stackCount : MIN_STACK_COUNT;

    std::vector<Vertex> vertices = getVertices();
    std::vector<uint32_t> indices = getIndices();

    mesh = new Mesh(vertices, vertices.size(), indices, indices.size());
}

std::vector<Vertex> Sphere::getVertices() const {
    std::vector<Vertex> vertices;

    float x, y, z, xy;                           // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius; // normal

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);  

        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            // vertex position
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            vertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(nx, ny, nz)));
        }
    }

    return vertices;
}

std::vector<uint32_t> Sphere::getIndices() const {
    std::vector<uint32_t> indices;

    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if(i != (stackCount-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return indices;
}

std::string Sphere::toString() const {
    return std::string("I am a Sphere with radius " + std::to_string(radius));
}

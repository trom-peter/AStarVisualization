#include "topography.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

Topography::Topography(unsigned char seed, float scale, bool type, int size, int amplitude) :
    seed(seed), size(size), amplitude(500), scale(scale),
    spacing(20.0f), type(type), mesh(nullptr) {}

Topography::~Topography() {
    delete mesh;
}

std::vector<Vertex> Topography::generateVertices() {
    std::vector<Vertex> vertices;
    float minY = FLT_MAX;
    float maxY = -FLT_MAX;
    for (double z = 0; z < size; z += spacing) {
        for (double x = 0; x < size; x += spacing) {
            Vertex v;
            float y;
            if (type == 1) 
                y = stb_perlin_fbm_noise3_seed(x * scale, z * scale, 0.0f, 2.0f, 0.5f, 4, seed) * amplitude;
            else 
                y = stb_perlin_ridge_noise3_seed(x * scale, z * scale, 0.0f, 2.0f, 0.5f, 1.0f, 4, seed) * amplitude;
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
            v.position = glm::vec3(x, y, z);
            vertices.push_back(v);
        }
    }

    this->minY = minY;
    this->maxY = maxY;

    //normalize all vertex heights from 0 to maxHeight
    for (Vertex& v : vertices) {
        v.position.y = ((v.position.y - minY) * amplitude) / (maxY - minY);
    }

    return vertices;
}

std::vector<uint32_t> Topography::generateIndices() {
    std::vector<uint32_t> indices;
    int lastX = size / spacing - 1;
    int lastZ = size / spacing - 1;
    for (int z = 0; z < lastZ; z++) {
        for (int x = 0; x < lastX; x++) {
            int topLeft = (z + 1) * (lastX + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = z * (lastX + 1) + x;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(topLeft);

            indices.push_back(topRight);
            indices.push_back(bottomRight);
            indices.push_back(bottomLeft);
        }
    }
    return indices;
}

void Topography::calculateNormals(std::vector<Vertex>& vertices, std::vector<uint32_t> indices) {
    for (size_t i = 0; i < indices.size(); i += 3) {
        uint32_t i0 = indices[i];
        uint32_t i1 = indices[i + 1];
        uint32_t i2 = indices[i + 2];

        glm::vec3 p0 = vertices[i0].position;
        glm::vec3 p1 = vertices[i1].position;
        glm::vec3 p2 = vertices[i2].position;

        glm::vec3 u(p1 - p0);
        glm::vec3 v(p2 - p0);

        glm::vec3 normal = glm::normalize(glm::cross(u, v));

        vertices[i0].normal = normal;
        vertices[i1].normal = normal;
        vertices[i2].normal = normal;
    }
}

void Topography::generate() {
    if (mesh != nullptr) delete mesh;
    std::vector<Vertex> vertices = Topography::generateVertices();
    std::vector<uint32_t> indices = Topography::generateIndices();
    calculateNormals(vertices, indices);
    mesh = new Mesh(vertices, vertices.size(), indices, indices.size());
}

Mesh* Topography::getMesh() {
	return mesh;
}

int Topography::getY(int x, int z) {
    int y;

    if (type == 1)
        y = stb_perlin_fbm_noise3_seed(x * scale, z * scale, 0.0f, 2.0f, 0.5f, 4, seed) * amplitude;
    else 
        y = stb_perlin_ridge_noise3_seed(x * scale, z * scale, 0.0f, 2.0f, 0.5f, 1.0f, 4, seed) * amplitude;

    y = ((y - minY) * amplitude) / (maxY - minY); //normalize
    return y;
}

float Topography::getMinY() {
    return minY;
}

float Topography::getMaxY() {
    return maxY;
}

unsigned char Topography::getSeed() {
    return seed;
}

void Topography::setSeed(unsigned char seed) {
    this->seed = seed;
}

bool Topography::getType() {
    return type;
}

void Topography::setType(int type) {
    this->type = type;
}

float Topography::getScale() {
    return scale;
}

int Topography::getSize() {
    return size;
}

void Topography::setScale(float scale) {
    this->scale = scale;
}

#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, size_t numVertices, std::vector<uint32_t>& indices, size_t numIndices) {
    this->numIndices = numIndices;
    vertexBuffer = new VertexBuffer(vertices.data(), numVertices, sizeof(vertices[0]));
    indexBuffer = new IndexBuffer(indices.data(), numIndices, sizeof(indices[0]));
}

Mesh::~Mesh() {
    delete vertexBuffer;
    delete indexBuffer;
}
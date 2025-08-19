#include "infrastructure/opengl/mesh.h"
#include "infrastructure/geometry/vertex.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const size_t numVertices, 
    const std::vector<uint32_t>& indices, const size_t numIndices) 
{
    this->numIndices = numIndices;
    vertexBuffer = new VertexBuffer(vertices.data(), numVertices, sizeof(vertices[0]));
    indexBuffer = new IndexBuffer(indices.data(), numIndices, sizeof(indices[0]));
}

Mesh::~Mesh() {
    delete vertexBuffer;
    delete indexBuffer;
}

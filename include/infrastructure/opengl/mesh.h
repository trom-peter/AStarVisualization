#pragma once
#include <vector>
#include "infrastructure/opengl/vertex_buffer.h"
#include "infrastructure/opengl/index_buffer.h"

struct Vertex;

struct Mesh {
    Mesh(const std::vector<Vertex>& vertices, const size_t numVertices, 
        const std::vector<uint32_t>& indices, const size_t numIndices);

    ~Mesh();

    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    uint64_t numIndices = 0;
};

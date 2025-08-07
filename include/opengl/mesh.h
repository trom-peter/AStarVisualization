#pragma once
#include <vector>
#include "opengl/vertex_buffer.h"
#include "opengl/index_buffer.h"

struct Vertex;

struct Mesh {
    Mesh(std::vector<Vertex>& vertices, size_t numVertices, std::vector<uint32_t>& indices, size_t numIndices);
    ~Mesh();

    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    uint64_t numIndices = 0;
};
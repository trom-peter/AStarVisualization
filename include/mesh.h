#pragma once

#include <vector>

#include "vertex.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

struct VertexBuffer;

struct Mesh {
    Mesh(std::vector<Vertex>& vertices, size_t numVertices, std::vector<uint32_t>& indices, size_t numIndices);
    ~Mesh();

    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    uint64_t numIndices = 0;
};
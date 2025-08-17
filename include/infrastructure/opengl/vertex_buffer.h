#pragma once
#include <cstdint>
#include "infrastructure/opengl/base_renderer.h"
#include "infrastructure/geometry/vertex.h"

struct VertexBuffer {
	VertexBuffer(const void* data, const uint64_t numVertices, const unsigned long long vertexSize) : 
		numVertices(numVertices) 
	{
		glGenBuffers(1, &bufferId); //generate buffer
		glBindBuffer(GL_ARRAY_BUFFER, bufferId); //bind it to gl state machine
		glBufferData(GL_ARRAY_BUFFER, numVertices * vertexSize, data, GL_STATIC_DRAW); // fill it with data
	}

	virtual ~VertexBuffer() {
		glDeleteBuffers(1, &bufferId);
	}

	void bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	}

	void unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	uint64_t getNumVertices() const {
		return numVertices;
	}

private:
	uint64_t numVertices;
	GLuint bufferId;
};
#pragma once
#include "base_renderer.h"
#include <cstdint>
#include "vertex.h"

struct VertexBuffer {
	VertexBuffer(void* data, uint64_t numVertices, unsigned long long vertexSize) : numVertices(numVertices) {
		glGenBuffers(1, &bufferId); //generate buffer
		glBindBuffer(GL_ARRAY_BUFFER, bufferId); //bind it to gl state machine
		glBufferData(GL_ARRAY_BUFFER, numVertices * vertexSize, data, GL_STATIC_DRAW); // fill it with data
	}

	virtual ~VertexBuffer() {
		glDeleteBuffers(1, &bufferId);
	}

	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	}

	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	uint64_t getNumVertices() {
		return numVertices;
	}

private:
	uint64_t numVertices;
	GLuint bufferId;
};
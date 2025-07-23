#pragma once
#include <cstdint>
#include "base_renderer.h"

struct IndexBuffer {
	IndexBuffer(void* data, uint32_t numIndices, uint8_t elementSize) : numIndices(numIndices) {
		glGenBuffers(1, &bufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * elementSize, data, GL_STATIC_DRAW);
	}

	virtual ~IndexBuffer() {
		glDeleteBuffers(1, &bufferId);
	}

	void bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
	}

	void unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t getNumIndices() {
		return numIndices;
	}

private:
	uint32_t numIndices;
	GLuint bufferId;
};
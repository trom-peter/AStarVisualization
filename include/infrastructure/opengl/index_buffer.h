#pragma once
#include <cstdint>
#include "infrastructure/opengl/base_renderer.h"

struct IndexBuffer {
	IndexBuffer(const void* data, const uint32_t numIndices, const uint8_t elementSize) : 
		numIndices(numIndices) 
	{
		glGenBuffers(1, &bufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * elementSize, data, GL_STATIC_DRAW);
	}

	virtual ~IndexBuffer() {
		glDeleteBuffers(1, &bufferId);
	}

	void bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
	}

	void unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t getNumIndices() const {
		return numIndices;
	}

private:
	uint32_t numIndices;
	GLuint bufferId;
};
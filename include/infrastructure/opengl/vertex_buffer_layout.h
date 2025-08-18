#pragma once
#include <vector>
#include <stdexcept>
#include "infrastructure/opengl/base_renderer.h"

struct VertexBufferElement{
	VertexBufferElement(const GLenum type, const GLuint amount, const GLboolean normalized) :
		type(type), amount(amount), normalized(normalized) {}

	unsigned int getSize() {
		switch (type) {
			case GL_FLOAT:          return 4;
			case GL_DOUBLE:         return 8;
			case GL_UNSIGNED_INT:   return 4;
			case GL_INT:            return 4;
			case GL_UNSIGNED_SHORT: return 2;
			case GL_SHORT:          return 2;
			case GL_UNSIGNED_BYTE:  return 1;
			case GL_BYTE:           return 1;
			case GL_HALF_FLOAT:     return 2;
			default:
				std::cerr << "ERROR: unknown VertexBufferElement type " << type << std::endl;
		}
	}

	GLenum type;
	GLuint amount;
	GLboolean normalized;
};

class VertexBufferLayout {
public:
	VertexBufferLayout() : stride(0) {};

	void add(VertexBufferElement element) {
		elements.push_back(element);
		stride += element.getSize() * element.amount;
	}

	std::vector<VertexBufferElement> getElements() const { 
		return elements; 
	}

	unsigned int getStride() const { 
		return stride; 
	}

private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;
};

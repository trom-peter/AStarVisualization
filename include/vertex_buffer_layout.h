#pragma once
#include <vector>
#include "base_renderer.h"
#include <stdexcept>

struct VertexBufferElement{
	GLenum type;
	GLuint count;
	GLboolean normalized;

	VertexBufferElement(GLenum type, GLuint count, GLboolean normalized) : type(type), count(count), normalized(normalized){}

	static unsigned int getSizeOfType(GLenum type) {
		switch (type) {
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_SHORT:	return 2;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		assert(false);
		return 0;
	}
};

class VertexBufferLayout {

public:
	VertexBufferLayout() : stride(0) {};

	template<typename T>
	void push(unsigned int count) {
		std::runtime_error(false);
	}

	template<>
	void push<float>(unsigned int count) {
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	template<>
	void push<unsigned short>(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_SHORT, count, GL_TRUE });
		stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_SHORT);
	}

	inline const std::vector<VertexBufferElement> getElements() const& { return elements; }
	inline unsigned int getStride() const { return stride; }

	void printElements() {
		for (VertexBufferElement e : elements) {
			std::cout << e.count << std::endl;
		}
	}

private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;
};
#include "infrastructure/opengl/vertex_array.h"
#include "infrastructure/opengl/vertex_buffer.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &vaoId);
	setupVertexLayout();
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vaoId);
}

void VertexArray::bind() const {
	glBindVertexArray(vaoId);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(VertexBuffer& vb) const {
	bind();
	vb.bind();
	unsigned int offset = 0;
	std::vector<VertexBufferElement> elements = layout.getElements();
	for (int i = 0; i < elements.size(); i++) {
		VertexBufferElement element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.amount, element.type, 
			element.normalized, layout.getStride(), (void*)offset);
		offset += element.amount * element.getSize();
	}
}

void VertexArray::setupVertexLayout() {
	layout.add(VertexBufferElement(GL_FLOAT, 3, GL_FALSE)); // Vertex position
	layout.add(VertexBufferElement(GL_FLOAT, 3, GL_FALSE)); // Vertex normal
}

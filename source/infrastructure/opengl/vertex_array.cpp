#include "infrastructure/opengl/vertex_array.h"
#include "infrastructure/opengl/vertex_buffer.h"

VertexArray::VertexArray() : layout(nullptr) {
	glGenVertexArrays(1, &vaoId);
	setupVertexLayout();
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vaoId);
	delete layout;
}

void VertexArray::bind() const {
	glBindVertexArray(vaoId);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

void VertexArray::setupVertexLayout() {
	VertexBufferLayout* layout = new VertexBufferLayout();
	layout->push<float>(3); //vertex position
	layout->push<float>(3); //vertex normal
	this->layout = layout;
}

void VertexArray::addVertexBuffer(VertexBuffer& vb) const {
	bind();
	vb.bind();
	unsigned int offset = 0;
	auto elements = layout->getElements();
	for (int i = 0; i < elements.size(); i++) {
		VertexBufferElement element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout->getStride(), (void*)offset);
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

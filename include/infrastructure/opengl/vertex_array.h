#pragma once
#include "infrastructure/opengl/vertex_buffer_layout.h"

struct VertexBuffer;

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void addVertexBuffer(VertexBuffer& vb) const;
	void setupVertexLayout();
	void bind() const;
	void unbind() const;

private:
	unsigned int vaoId;
	VertexBufferLayout* layout;
};
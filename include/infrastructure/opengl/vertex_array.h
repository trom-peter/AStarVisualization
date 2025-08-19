#pragma once
#include "infrastructure/opengl/vertex_buffer_layout.h"

struct VertexBuffer;

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void addVertexBuffer(VertexBuffer& vb) const;
	void bind() const;
	void unbind() const;

private:
	void setupVertexLayout();
	unsigned int vaoId;
	VertexBufferLayout layout;
};

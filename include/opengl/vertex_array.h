#pragma once
#include "opengl/vertex_buffer_layout.h"

struct VertexBuffer;

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void addVertexBuffer(VertexBuffer& vb);
	void setupVertexLayout();
	void bind();
	void unbind();

private:
	unsigned int vaoId;
	VertexBufferLayout* layout;
};
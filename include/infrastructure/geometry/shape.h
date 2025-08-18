#pragma once
#include <string>
#include "glm/glm.hpp"
#include "infrastructure/opengl/mesh.h"

struct Mesh;

struct Shape {
	Shape(const glm::vec3 position = glm::vec3(0.0f), 
		const glm::vec3 scale = glm::vec3(1.0f), 
		const glm::vec3 color = glm::vec3(1.0f));

	virtual std::string toString() const = 0;

	// Get vertices for specific shape
	virtual std::vector<Vertex> getVertices() const = 0;

	// Get indices for specific shape
	virtual std::vector<uint32_t> getIndices() const = 0;

	void setPosition(const glm::vec3 position);
	void setScale(const glm::vec3 scale);
	void setColor(const glm::vec3 color);

	std::unique_ptr<Mesh> mesh;
	glm::mat4 model; // Model matrix
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 color;

private:
	void updateModelMatrix();
};
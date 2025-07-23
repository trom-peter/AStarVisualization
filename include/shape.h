#pragma once
#include "glm/glm.hpp"
#include "mesh.h"
#include <string>

struct Mesh;

struct Shape {
	Shape(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 color = glm::vec3(1.0f));
	virtual std::string toString() const = 0;
	virtual std::vector<Vertex> getVertices() const = 0;
	virtual std::vector<uint32_t> getIndices() const = 0;
	void setPosition(glm::vec3 position);
	void setScale(glm::vec3 scale);
	void setColor(glm::vec3 color);

	Mesh* mesh;
	glm::mat4 model;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 color;

private:
	void updateModelMatrix();
};
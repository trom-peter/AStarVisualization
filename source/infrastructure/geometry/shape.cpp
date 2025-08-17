#include "infrastructure/geometry/shape.h"
#include "glm/gtc/matrix_transform.hpp"

Shape::Shape(const glm::vec3 position, const glm::vec3 scale, const glm::vec3 color) : 
	position(position), scale(scale), color(color), mesh(nullptr), model(glm::mat4(1.0f)) 
{
	updateModelMatrix();
};

void Shape::setScale(const glm::vec3 scale) {
	this->scale = scale;
	updateModelMatrix();
}

void Shape::setPosition(const glm::vec3 position) {
	this->position = position;
	updateModelMatrix();
}

void Shape::setColor(const glm::vec3 color) {
	this->color = color;
}

void Shape::updateModelMatrix() {
	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
}

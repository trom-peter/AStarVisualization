#include "shape.h"
#include "glm/gtc/matrix_transform.hpp"

Shape::Shape(glm::vec3 position, glm::vec3 scale, glm::vec3 color) : position(position), scale(scale), color(color), mesh(nullptr), model(glm::mat4(1.0f)) {
	updateModelMatrix();
};

void Shape::setScale(glm::vec3 scale) {
	this->scale = scale;
	updateModelMatrix();
}

void Shape::setPosition(glm::vec3 position) {
	this->position = position;
	updateModelMatrix();
}

void Shape::setColor(glm::vec3 color) {
	this->color = color;
}

void Shape::updateModelMatrix() {
	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
}

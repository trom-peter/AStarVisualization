#include <iostream>
#include "opengl/camera.h"

Camera::Camera(float fov, float width, float height, float nearPlane, float farPlane) : fov(fov) {
	projection = glm::perspective(fov / 2.0f, width / height, nearPlane, farPlane);
	view = glm::mat4(1.0f);
	position = glm::vec3(0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 0.0f;
	pitch = 0.0f;
	update();
}

void Camera::resizeProj(float width, float height) {
	projection = glm::perspective(fov / 2.0f, width / height, 10.0f, 20000.0f);
	update();
}

glm::mat4 Camera::getProj() {
	return projection;
}

glm::mat4 Camera::getView() {
	return view;
}

void Camera::update() {
	view = glm::lookAt(position, position + lookAt, up);
	viewProj = projection * view;
}

void Camera::translate(glm::vec3 v) {
	position += v;
	view = glm::translate(view, v * -1.0f);
}

void Camera::rotate(glm::vec2 degrees) {
	yaw += degrees.x;
	pitch -= degrees.y;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 front(0.0f);
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	lookAt = glm::normalize(front);
	update();
}

glm::vec3 Camera::getPosition() {
	return position;
}

float Camera::getFov() {
	return fov;
}

glm::vec3 Camera::getLookAt() {
	return lookAt;
}

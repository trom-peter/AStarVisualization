#include <iostream>
#include "fps_camera.h"

FPSCamera::FPSCamera(float fov, float width, float height, float speed, float mouseSensitivity, float nearPlane, float farPlane) : 
	speed(speed), mouseSensitivity(mouseSensitivity),  Camera(fov, width, height, nearPlane, farPlane)
{
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = -90.0f;
	pitch = 0.0f;
	onMouseMoved(0.0f, 0.0f);
	update();
}

void FPSCamera::onMouseMoved(float xRel, float yRel) {
	yaw += xRel * mouseSensitivity;
	pitch -= yRel * mouseSensitivity;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	lookAt = glm::normalize(front);
	update();
}

void FPSCamera::update() {
	view = glm::lookAt(position, position + lookAt, up);
	viewProj = projection * view;
}

void FPSCamera::move(float forwardAmount, float sidewaysAmount) {
	glm::vec3 transForward(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * lookAt) * forwardAmount);
	glm::vec3 transSideways(glm::normalize(glm::cross(lookAt, up)) * sidewaysAmount);
	glm::vec3 moveVec(transForward + transSideways);
	if (forwardAmount != 0.0f && sidewaysAmount != 0.0f) moveVec /= sqrt(2); //adjust diagonal movement
	translate(moveVec);
	update();
}

void FPSCamera::moveUp(float amount) {
	translate(up * amount);
}

float FPSCamera::getSpeed() {
	return speed;
}

glm::vec3 FPSCamera::getLookAt() {
	return lookAt;
}

#include <iostream>
#include "camera.h"

	Camera::Camera(float fov, float width, float height, float nearPlane, float farPlane) : fov(fov) {
		projection = glm::perspective(fov / 2.0f, width / height, nearPlane, farPlane);
		view = glm::mat4(1.0f);
		position = glm::vec3(0.0f);
		update();
	}

	glm::mat4 Camera::getProj() {
		return projection;
	}

	glm::mat4 Camera::getView() {
		return view;
	}

	void Camera::update() {
		viewProj = projection * view;
	}

	void Camera::translate(glm::vec3 v) {
		position += v;
		view = glm::translate(view, v * -1.0f);
	}

	glm::vec3 Camera::getPosition() {
		return position;
	}

	float Camera::getFov() {
		return fov;
	}

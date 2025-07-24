#pragma once

#include "camera.h"
#include "glm/ext/matrix_transform.hpp"

class FPSCamera : public Camera {
public:
	FPSCamera(float fov, float width, float height, float speed, float mouseSensitivity, float nearPlane, float farPlane);

	void onMouseMoved(float xRel, float yRel);

	void update() override;

	void move(float forwardAmount, float sidewaysAmount);

	void moveUp(float amount);

	void rotate(glm::vec2 degrees);

	float getSpeed();

	glm::vec3 getLookAt();
protected:
	float yaw; // rotation around x-axis
	float pitch; // rotation around y-axis
	glm::vec3 lookAt;
	const float mouseSensitivity;
	glm::vec3 up;
	float speed; // in units per second
};
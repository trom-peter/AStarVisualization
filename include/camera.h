#pragma once 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:

	Camera(float fov, float width, float height, float nearPlane, float farPlane);

	glm::mat4 getProj();

	glm::mat4 getView();

	virtual void update();

	virtual void translate(glm::vec3 v);

	void rotate(glm::vec2 degrees);

	glm::vec3 getPosition();

	float getFov();

	glm::vec3 getLookAt();

protected:
	glm::vec3 position;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 viewProj;
	glm::vec3 lookAt;
	float yaw; // rotation around x-axis
	float pitch; // rotation around y-axis
	glm::vec3 up;
	float fov;
};
#pragma once 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:

	Camera(const float fov, const float width, const float height, const float nearPlane, const float farPlane);

	void resizeProj(const float width, const float height);

	glm::mat4 getProj() const;

	glm::mat4 getView() const;

	virtual void update();

	virtual void translate(glm::vec3 v);

	void rotate(glm::vec2 degrees);

	glm::vec3 getPosition() const;

	float getFov() const;

	glm::vec3 getLookAt() const;

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
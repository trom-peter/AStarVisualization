#pragma once 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
	Camera(const float fov, 
		const float width, const float height, 
		const float nearPlane, const float farPlane);

	virtual void update();
	virtual void translate(glm::vec3 v);
	void rotate(glm::vec2 degrees);
	void resizeProj(const float width, const float height);

	float getFov() const;
	glm::vec3 getPosition() const;
	glm::mat4 getView() const;
	glm::mat4 getProj() const;
	glm::vec3 getLookAt() const;

private:
	glm::vec3 position;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 viewProj;
	glm::vec3 lookAt;
	float nearPlane; // Minimum seeing distance (objects in front of nearPlane are not rendererd)
	float farPlane; // Maximum seeing distance (objects behind farPlane are not rendererd)
	float yaw; // Rotation around x-axis
	float pitch; // Rotation around y-axis
	glm::vec3 up; // Vector that points up (+y)
	float fov; // Field of view
};

#pragma once
#include "glm/glm.hpp"

struct StategridConfig {
	void init();

	//state colors
	glm::vec3 defaultColor;
	glm::vec3 frontierColor;
	glm::vec3 reachedColor;
	glm::vec3 initialStateColor;
	glm::vec3 goalStateColor;
	glm::vec3 solutionStateColor;

	//state visibility
	bool defaultVisible;
	bool frontierVisible;
	bool reachedVisible;
};

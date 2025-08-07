#include "configurations/stategrid_configuration.h"

void StategridConfig::init() {
	defaultColor = glm::vec3(0.0f, 0.0f, 0.0f);
	frontierColor = glm::vec3(1.0f, 1.0f, 1.0f);
	reachedColor = glm::vec3(0.2f, 0.6f, 1.0f);
	initialStateColor = glm::vec3(0.2f, 1.0f, 0.2f);
	goalStateColor = glm::vec3(1.0f, 0.2f, 0.2f);
	solutionStateColor = glm::vec3(1.0f, 1.0f, 0.0f);

	defaultVisible = true;
	frontierVisible = true;
	reachedVisible = true;
}

#include "configurations/problem_configuration.h"

void ProblemConfig::init() {
	initial = glm::ivec2(0, 0);
	goal = glm::ivec2(0, 0);
	heuristic = 0;
	overestimateFactor = 1.0f;
}
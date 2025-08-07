#pragma once
#include "glm/glm.hpp"

struct ProblemConfig {
	void init();

	int heuristic; // what heuristic should be used for searching
	float overestimateFactor; // by what factor the overestimated heuristic should be multiplied by
	glm::ivec2 initial;
	glm::ivec2 goal;
};

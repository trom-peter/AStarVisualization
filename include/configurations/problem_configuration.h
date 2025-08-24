#pragma once
#include "glm/glm.hpp"

struct ProblemConfig {
	ProblemConfig();

	void reset();

	int heuristic; // What heuristic should be used for searching
	float overestimateFactor; // By what factor the overestimated heuristic should be multiplied by
	glm::ivec2 initial;
	glm::ivec2 goal;

	// Mininum and maximum overestimate factor
	constexpr static float OVERESTIMATE_FACTOR_MIN = 0.0f;
	constexpr static float OVERESTIMATE_FACTOR_MAX = 10.0f;
};

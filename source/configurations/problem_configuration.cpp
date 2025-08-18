#include "configurations/problem_configuration.h"

glm::ivec2 constexpr INITIAL_STATE = glm::ivec2(0, 0);
glm::ivec2 constexpr GOAL_STATE = glm::ivec2(0, 0);
int constexpr HEURISTIC = 0;
float constexpr OVERESTIMATE_FACTOR = 1.0f;

ProblemConfig::ProblemConfig() : 
	initial(INITIAL_STATE),
	goal(GOAL_STATE),
	heuristic(HEURISTIC),
	overestimateFactor(OVERESTIMATE_FACTOR) {}

void ProblemConfig::reset() {
	initial = INITIAL_STATE;
	goal = GOAL_STATE;
	heuristic = HEURISTIC;
	overestimateFactor = OVERESTIMATE_FACTOR;
}
#include "configurations/problem_configuration.h"

// Initial search problem configuration
constexpr glm::ivec2 INITIAL_STATE = glm::ivec2(0, 0);
constexpr glm::ivec2 GOAL_STATE = glm::ivec2(0, 0);
constexpr int HEURISTIC = 0;
constexpr float OVERESTIMATE_FACTOR = 1.0f;

ProblemConfig::ProblemConfig() : 
	initial(INITIAL_STATE),
	goal(GOAL_STATE),
	heuristic(HEURISTIC),
	overestimateFactor(OVERESTIMATE_FACTOR) {}

// Reset search problem to inital problem configuration
void ProblemConfig::reset() {
	initial = INITIAL_STATE;
	goal = GOAL_STATE;
	heuristic = HEURISTIC;
	overestimateFactor = OVERESTIMATE_FACTOR;
}

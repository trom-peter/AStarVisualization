#include "configurations/stategrid_configuration.h"

// Initial stategrid configuration
constexpr glm::vec3 DEFAULT_COLOR = glm::vec3(0.0f);
constexpr glm::vec3 FRONTIER_COLOR = glm::vec3(1.0f);
constexpr glm::vec3 REACHED_COLOR = glm::vec3(0.2f, 0.6f, 1.0f);
constexpr glm::vec3 INITIAL_STATE_COLOR = glm::vec3(0.2f, 1.0f, 0.2f);
constexpr glm::vec3 GOAL_STATE_COLOR = glm::vec3(1.0f, 0.2f, 0.2f);
constexpr glm::vec3 SOLUTION_STATE_COLOR = glm::vec3(1.0f, 1.0f, 0.0f);

constexpr bool DEFAULT_VISIBLE = true;
constexpr bool FRONTIER_VISIBLE = true;
constexpr bool REACHED_VISIBLE = true;

StategridConfig::StategridConfig() :
	defaultColor(DEFAULT_COLOR),
	frontierColor(FRONTIER_COLOR),
	reachedColor(REACHED_COLOR),
	initialStateColor(INITIAL_STATE_COLOR),
	goalStateColor(GOAL_STATE_COLOR),
	solutionStateColor(SOLUTION_STATE_COLOR),
	defaultVisible(DEFAULT_VISIBLE),
	frontierVisible(FRONTIER_VISIBLE),
	reachedVisible(REACHED_VISIBLE) {}

// Reset stategrid to initial configuration
void StategridConfig::reset() {
	defaultColor = DEFAULT_COLOR;
	frontierColor = FRONTIER_COLOR;
	reachedColor = REACHED_COLOR;
	initialStateColor = INITIAL_STATE_COLOR;
	goalStateColor = GOAL_STATE_COLOR;
	solutionStateColor = SOLUTION_STATE_COLOR;

	defaultVisible = DEFAULT_VISIBLE;
	frontierVisible = FRONTIER_VISIBLE;
	reachedVisible = REACHED_VISIBLE;
}

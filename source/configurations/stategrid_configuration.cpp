#include "configurations/stategrid_configuration.h"

// Initial stategrid configuration
glm::vec3 constexpr DEFAULT_COLOR = glm::vec3(0.0f);
glm::vec3 constexpr FRONTIER_COLOR = glm::vec3(1.0f);
glm::vec3 constexpr REACHED_COLOR = glm::vec3(0.2f, 0.6f, 1.0f);
glm::vec3 constexpr INITIAL_STATE_COLOR = glm::vec3(0.2f, 1.0f, 0.2f);
glm::vec3 constexpr GOAL_STATE_COLOR = glm::vec3(1.0f, 0.2f, 0.2f);
glm::vec3 constexpr SOLUTION_STATE_COLOR = glm::vec3(1.0f, 1.0f, 0.0f);

bool constexpr DEFAULT_VISIBLE = true;
bool constexpr FRONTIER_VISIBLE = true;
bool constexpr REACHED_VISIBLE = true;

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


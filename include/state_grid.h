#pragma once
#include "sphere.h"
#include <unordered_map>
#include "a_star/state.h"
#include "search_configuration.h"
#include "a_star/graph.h"
#include "a_star/a_star_search.h"

class StateGrid {
public:
	StateGrid(int gridSize);

	void updateToStep(int step, Graph& g, AStarSearch& aStar, bool forwards);

	void initGrid(Topography* topo);

	std::unordered_map<State, Sphere*> grid;
	int gridSize;
	glm::vec3 defaultColor = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 frontierColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 reachedColor = glm::vec3(0.2f, 0.6f, 1.0f);
	glm::vec3 initialStateColor = glm::vec3(0.2f, 1.0f, 0.2f);
	glm::vec3 goalStateColor = glm::vec3(1.0f, 0.2f, 0.2f);
};


#pragma once
#include "model/topography.h"
#include "model/state.h"

class AStarSearch;
struct State;
class Topography;
class Graph;
struct StateHash;
struct SearchProblem;

class Stategrid {
public:
	Stategrid();
	Stategrid(const int gridSize, 
		const glm::vec3 defaultColor, const glm::vec3 frontierColor, const glm::vec3 reachedColor,
		const glm::vec3 initialStateColor, const glm::vec3 goalStateColor, const glm::vec3 solutionStateColor,
		const bool defaultVisible, const bool frontierVisible, const bool reachedVisible);

	~Stategrid();

	void initGrid(const Topography& topo);
	void updateToStep(const int step, const Graph& g, AStarSearch& aStar, const bool forwards);

	void updateVisibility(const bool defaultVisible, const bool frontierVisible, const bool reachedVisible);
	void showSolutionPath(const std::vector<State> solutionPath, const SearchProblem& problem);
	bool isVisible(const glm::vec3 color) const;
	void clearGrid();

	std::unordered_map<State, glm::vec3, StateHash> grid; // Map that stores color for each state
	int gridResolution;

	// State colors
	glm::vec3 defaultColor;
	glm::vec3 frontierColor;
	glm::vec3 reachedColor;
	glm::vec3 initialStateColor;
	glm::vec3 goalStateColor;
	glm::vec3 solutionStateColor;

	// State visibility
	bool defaultVisible;
	bool frontierVisible;
	bool reachedVisible;
};

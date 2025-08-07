#pragma once
#include "topography.h"
#include "a_star/state.h"

class AStarSearch;
struct StategridConfig;
struct State;
class Topography;
struct Sphere;
class Graph;
struct StateHash;
struct SearchProblem;

class Stategrid {
public:
	Stategrid(int gridSize, StategridConfig& config);

	void updateToStep(int step, Graph& g, AStarSearch& aStar, bool forwards);
	void initGrid(Topography* topo);
	void updateVisibility(StategridConfig& config);
	void showSolutionPath(std::vector<State> solutionPath, SearchProblem& problem);
	bool isVisible(glm::vec3 color) const;

	std::unordered_map<State, Sphere*, StateHash> grid;
	int gridSize;

	//state colors
	glm::vec3 defaultColor;
	glm::vec3 frontierColor;
	glm::vec3 reachedColor;
	glm::vec3 initialStateColor;
	glm::vec3 goalStateColor;
	glm::vec3 solutionStateColor;

	//state visibility
	bool defaultVisible;
	bool frontierVisible;
	bool reachedVisible;
};


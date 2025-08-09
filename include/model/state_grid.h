#pragma once
#include "model/topography.h"
#include "model/state.h"

class AStarSearch;
struct StategridConfig;
struct State;
class Topography;
class Graph;
struct StateHash;
struct SearchProblem;

class Stategrid {
public:
	Stategrid();
	Stategrid(int gridSize, StategridConfig& config);

	~Stategrid();

	void initGrid(Topography* topo);
	void updateToStep(int step, Graph& g, AStarSearch& aStar, bool forwards);

	void updateVisibility(StategridConfig& config);
	void showSolutionPath(std::vector<State> solutionPath, SearchProblem& problem);
	bool isVisible(glm::vec3 color) const;
	void clearGrid();

	std::unordered_map<State, glm::vec3, StateHash> grid;
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


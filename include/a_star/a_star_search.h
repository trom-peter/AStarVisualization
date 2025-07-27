#pragma once
#include <queue>
#include <unordered_map>
#include <set>
#include <functional>
#include "a_star/problem.h"
#include "a_star/node.h"
#include "topography.h"

class AStarSearch {
public:
	AStarSearch(Problem& p);
	Node* search();
	void setHeuristic(std::function<float(State, State)> h);

	std::vector<std::vector<State>> allFrontiers;
	std::vector<State> allExpanded;
	std::vector<State> solutionPath;
	int consideredNodes;

private:
	Problem& problem;
	std::function<float(State, State)> heuristic;
	void setConsideredNodes();
	void setSolution(Node* n);
};
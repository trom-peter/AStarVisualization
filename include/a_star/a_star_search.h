#pragma once
#include <queue>
#include <unordered_map>
#include <set>
#include <functional>
#include "a_star/search_problem.h"
#include "a_star/node.h"
#include "topography.h"

class AStarSearch {
public:
	AStarSearch(SearchProblem& p);
	Node* search();
	void setHeuristic(std::function<float(State, State)> h);
	SearchProblem getProblem();

	std::vector<std::vector<State>> allFrontiers;
	std::vector<State> allExpanded;
	std::vector<State> solutionPath;
	int consideredNodes;

private:
	SearchProblem& problem;
	std::function<float(State, State)> heuristic;
	void setConsideredNodes();
	void setSolution(Node* n);
};
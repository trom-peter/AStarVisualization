#pragma once
#include <queue>
#include <unordered_map>
#include <set>
#include "a_star/problem.h"
#include "a_star/node.h"

class AStarSearch {
public:
	AStarSearch();
	Node* search(Problem& problem);

	std::vector<std::vector<State>> allFrontiers;
	std::vector<State> allExpanded;
	std::vector<State> solutionPath;
private:
	float euclideanDistanceDuration(State s0, State s1) const;
	float travelTime(State s0, State s1, Graph& g) const;
	void addToSolution(Node* n);
};
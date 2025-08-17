#pragma once
#include "model/graph.h"
#include "model/state.h"

struct SearchEnvironment;
struct Node;
struct State;
struct Graph;

struct SearchProblem {
	SearchProblem(SearchEnvironment& environment, const glm::ivec2 initial, const glm::ivec2 goal);

	bool isGoal(const State s) const;

	std::vector<Node*> actions(Node* n) const;

	Graph& graph;
	State initial;
	State goal;
};
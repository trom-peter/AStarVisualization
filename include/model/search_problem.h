#pragma once
#include "model/graph.h"
#include "model/state.h"

struct SearchEnvironment;
struct Node;
struct State;
struct Graph;

struct SearchProblem {
	SearchProblem(SearchEnvironment& environment, glm::ivec2 initial, glm::ivec2 goal);

	bool isGoal(State s);

	std::vector<Node*> actions(Node* n);

	Graph& graph;
	State initial;
	State goal;
};
#pragma once
#include "model/graph.h"
#include "model/state.h"

struct SearchEnvironment;
struct ProblemConfig;
struct Node;
struct State;
struct Graph;

struct SearchProblem {
public:
	SearchProblem(SearchEnvironment& environment, ProblemConfig problemConfig);

	bool isGoal(State s);

	std::vector<Node*> actions(Node* n);

	Graph& graph;
	State initial;
	State goal;

private:
	float travelTimeActionCost(State a, State b);
};
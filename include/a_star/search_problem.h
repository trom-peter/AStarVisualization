#pragma once
#include "a_star/state.h"
#include "a_star/graph.h"
#include "heuristics.h"

struct SearchProblem {

	float travelTimeActionCost(State a, State b) {
		float dx = abs(a.x - b.x);
		float dh = abs(a.y - b.y);
		float dz = abs(a.z - b.z);

		float horizontalDistance = sqrt(dx * dx + dz * dz);

		if (horizontalDistance == 0.0f) return 0.0f; // no movement

		float slope = dh / horizontalDistance;
		float distance = (sqrt(dx * dx + dh * dh + dz * dz));
		float speed = (6 * exp(-3.5 * fabsf(slope + 0.05)));
		float travelTime = ((distance / 1000) / speed) * 3600; // travel time in seconds

		return travelTime;
	};

	SearchProblem(Graph* g) : g(g), initial(State(0, 0, 0)), goal(State(0, 0, 0)) {
		g->init();
	}
	
	SearchProblem(Graph* g, State initial, State goal) : g(g), initial(initial), goal(goal) {
		g->init();
	}

	bool isGoal(State s) const {
		return s == goal;
	}

	std::vector<Node*> actions(Node* n) {
		State& s0 = n->s;
		std::vector<Node*> nodes;
		std::vector<State> neighbours = g->getNeighbours(s0.x, s0.z);

		for (State s1 : neighbours) {
			int actionCost = travelTimeActionCost(s0, s1);
			g->addEdge(s0.x, s0.z, s1.x, s1.z, actionCost);
			int pathCost = n->pathCost + actionCost;
			nodes.push_back(new Node(s1.x, s1.y, s1.z, n, pathCost));
		}

		return nodes;
	}

	Graph* g;
	State initial;
	State goal;

};
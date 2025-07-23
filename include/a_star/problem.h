#pragma once
#include "a_star/state.h"
#include "a_star/graph.h"

struct Problem {

	int euclideanDistanceDuration(State s0, State s1) {
		int dx = abs(s0.x - s1.x);
		int dy = abs(s0.y - s1.y);
		int dz = abs(s0.z - s1.z);
		return sqrt(dx * dx + dy * dy + dz * dz) / 1;
	}

	Problem(Graph& g) : g(g), initial(State(0, 0, 0)), goal(State(0, 0, 0)) {
		g.init();
	}

	Problem(Graph& g, State initial, State goal) : g(g), initial(initial), goal(goal) {
		g.init();
	}

	bool isGoal(State s) const {
		return s == goal;
	}

	std::vector<Node*> actions(Node* n) {
		State& s0 = n->s;
		std::vector<Node*> nodes;
		std::vector<State> neighbours = g.getNeighbours(s0.x, s0.z);

		for (State s1 : neighbours) {
			g.addEdge(s0.x, s0.z, s1.x, s1.z, euclideanDistanceDuration(s0, s1));
			int cost = n->pathCost + g.getWeight(s0.x, s0.z, s1.x, s1.z);
			nodes.push_back(new Node(s1.x, s1.y, s1.z, n, cost));
		}

		return nodes;
	}

	Graph& g;
	State initial;
	State goal;

};
#pragma once
#include "a_star/state.h"
#include "a_star/graph.h"
#include "heuristics.h"

struct Problem {
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
			g.addEdge(s0.x, s0.z, s1.x, s1.z, Heuristics::travelTime(s0, s1));
			int pathCost = n->pathCost + g.getWeight(s0.x, s0.z, s1.x, s1.z);
			nodes.push_back(new Node(s1.x, s1.y, s1.z, n, pathCost));
		}

		return nodes;
	}

	Graph& g;
	State initial;
	State goal;

};
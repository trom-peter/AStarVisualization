#include "model/search_problem.h"
#include "model/search_environment.h"
#include "model/node.h"
#include "model/performance_measure.h"
#include "configurations/problem_configuration.h"
#include <iostream>

SearchProblem::SearchProblem(SearchEnvironment& environment, const glm::ivec2 initial, const glm::ivec2 goal) :
	graph(environment.graph),
	initial(State(initial.x, environment.topography.getY(initial.x, initial.y), initial.y)),
	goal(State(goal.x, environment.topography.getY(goal.x, goal.y), goal.y)) {
}

bool SearchProblem::isGoal(const State s) const {
	return s == goal;
}

std::vector<Node*> SearchProblem::actions(Node* n) const {
	State& s0 = n->s;
	std::vector<Node*> nodes;
	std::vector<State> neighbours = graph.getNeighbours(s0.x, s0.z);

	for (State s1 : neighbours) {
		int actionCost = 0;

		if (!graph.hasEdge(s0.x, s0.y, s1.x, s1.y)) {
			//add edge with action cost if necessary
			actionCost = PerformanceMeasure::hikingFunction(s0, s1);
			graph.addEdge(s0.x, s0.z, s1.x, s1.z, actionCost);
		}
		else {
			//get action cost if edge already exists
			actionCost = graph.getWeight(s0.x, s0.y, s1.x, s1.y);
		}

		int pathCost = n->pathCost + actionCost;
		nodes.push_back(new Node(s1.x, s1.y, s1.z, n, pathCost));
	}

	return nodes;
}

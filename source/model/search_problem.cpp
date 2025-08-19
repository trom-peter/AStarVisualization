#include "model/search_problem.h"
#include "model/search_environment.h"
#include "model/node.h"
#include "model/performance_measure.h"
#include "configurations/problem_configuration.h"
#include <iostream>
#include <memory>

SearchProblem::SearchProblem(SearchEnvironment& environment, const glm::ivec2 initial, const glm::ivec2 goal) :
	graph(environment.graph),
	initial(State(initial.x, environment.topography.getY(initial.x, initial.y), initial.y)),
	goal(State(goal.x, environment.topography.getY(goal.x, goal.y), goal.y)) {
}

// Goal test
bool SearchProblem::isGoal(const State s) const {
	return s == goal;
}

// Returns all possible actions for a state.
std::vector<std::shared_ptr<Node>> SearchProblem::actions(std::shared_ptr<Node> n) const {
	State& s0 = n->s;
	std::vector<std::shared_ptr<Node>> nodes;
	std::vector<State> neighbours = graph.getNeighbours(s0.x, s0.z);

	for (State s1 : neighbours) {
		int actionCost = 0;

		if (!graph.hasEdge(s0.x, s0.z, s1.x, s1.z)) {
			// Add edge with action cost if necessary
			actionCost = PerformanceMeasure::hikingFunction(s0, s1);
			graph.addEdge(s0.x, s0.z, s1.x, s1.z, actionCost);
		}
		else {
			// Get action cost if edge already exists
			actionCost = graph.getWeight(s0.x, s0.z, s1.x, s1.z);
		}

		int pathCost = n->pathCost + actionCost;
		nodes.push_back(std::make_shared<Node>(s1.x, s1.y, s1.z, n, pathCost));
	}

	return nodes;
}

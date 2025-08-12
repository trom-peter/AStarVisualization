#include "model/search_problem.h"
#include "model/search_environment.h"
#include "model/node.h"
#include "model/performance_measure.h"
#include "configurations/problem_configuration.h"

SearchProblem::SearchProblem(SearchEnvironment& environment, ProblemConfig problemConfig) :
	graph(environment.graph),
	initial(State(
		problemConfig.initial.x,
		environment.topography.getY(problemConfig.initial.x, problemConfig.initial.y),
		problemConfig.initial.y)),
	goal(State(
		problemConfig.goal.x,
		environment.topography.getY(problemConfig.goal.x, problemConfig.goal.y),
		problemConfig.goal.y)) {
}

bool SearchProblem::isGoal(State s) {
	return s == goal;
}

std::vector<Node*> SearchProblem::actions(Node* n) {
	State& s0 = n->s;
	std::vector<Node*> nodes;
	std::vector<State> neighbours = graph.getNeighbours(s0.x, s0.z);

	for (State s1 : neighbours) {
		int actionCost = PerformanceMeasure::hikingFunction(s0, s1);
		graph.addEdge(s0.x, s0.z, s1.x, s1.z, actionCost);
		int pathCost = n->pathCost + actionCost;
		nodes.push_back(new Node(s1.x, s1.y, s1.z, n, pathCost));
	}

	return nodes;
}

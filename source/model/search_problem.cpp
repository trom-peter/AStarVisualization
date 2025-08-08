#include "model/search_problem.h"
#include "model/search_environment.h"
#include "model/node.h"
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
		int actionCost = travelTimeActionCost(s0, s1);
		graph.addEdge(s0.x, s0.z, s1.x, s1.z, actionCost);
		int pathCost = n->pathCost + actionCost;
		nodes.push_back(new Node(s1.x, s1.y, s1.z, n, pathCost));
	}

	return nodes;
}

float SearchProblem::travelTimeActionCost(State a, State b) {
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

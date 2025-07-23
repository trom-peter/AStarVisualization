#include "a_star/a_star_search.h"

AStarSearch::AStarSearch() {}

float AStarSearch::euclideanDistanceDuration(State s0, State s1) const {
	int dx = abs(s0.x - s1.x);
	int dy = abs(s0.y - s1.y);
	int dz = abs(s0.z - s1.z);
	return sqrt(dx * dx + dy * dy + dz * dz) / 1;
}

float AStarSearch::travelTime(State s0, State s1, Graph& g) const {
	float dx = abs(s0.x - s1.x);
	float dh = abs(s0.y - s1.y);
	float dz = abs(s0.z - s1.z);

	float horizontalDistance = sqrt(dx * dx + dz * dz);

	if (horizontalDistance == 0.0f) return 0.0f; // no movement

	float slope = dh / horizontalDistance;
	float distance = (sqrt(dx * dx + dh * dh + dz * dz));
	float speed = (6 * exp(-3.5 * fabsf(slope + 0.05)));

	float travelTime = ((distance / 1000) / speed) * 3600; // travel time in seconds

	//std::cout << "[" << s0.x << ", " << s0.z << "]	to	[" << s1.x << ", " << s1.z << "]:		" << travelTime << "s	" << distance << "m		" << slope << "d	" << speed << "km/h" << std::endl;
	return travelTime;
}

Node* AStarSearch::search(Problem& problem) {
	int step = 0;
	Node* initial = new Node(problem.initial, 0);

	auto f = [this, problem](Node* a, Node* b) { 
		return a->pathCost + travelTime(a->s, problem.goal, problem.g) > b->pathCost + travelTime(b->s, problem.goal, problem.g);
		};

	std::priority_queue<Node*, std::vector<Node*>, decltype(f)> frontier(f); //grenzbereich
	frontier.push(initial);
	allFrontiers.push_back({ initial->s });

	std::unordered_map<State, Node*> reached; // bester pfad pro state
	reached.insert({ problem.initial, initial });

	while (!frontier.empty()) {
		Node* n = frontier.top();
		frontier.pop();

		//skip outdated nodes that are not optimal
		if (reached[n->s]->pathCost != n->pathCost) {
			continue;
		}

		allExpanded.push_back(n->s);

		//std::cout << "Expanded: " << n->s.x << ", " << n->s.y << ", " << n->s.z << " with pathcost " << n->pathCost << std::endl;

		if (problem.isGoal(n->s)) {
			addToSolution(n);
			return n;
		}

		for (Node* child : problem.actions(n)) {
			State s = child->s;
			if (reached.count(s) == 0 || child->pathCost < reached[s]->pathCost) {
				reached[s] = child;
				frontier.push(child);
				allFrontiers.at(step).push_back(s);
			}
		}
		step++;
		allFrontiers.push_back({});
	}

	return nullptr;
}

void AStarSearch::addToSolution(Node* n) {
	if (n == nullptr) return;
	else {
		solutionPath.push_back(n->s);
		addToSolution(n->parent);
	}
}
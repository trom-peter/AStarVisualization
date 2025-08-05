#include "a_star/a_star_search.h"

AStarSearch::AStarSearch(SearchProblem& p) : problem(p) {}

void AStarSearch::setHeuristic(std::function<float(State, State)> h) {
	this->heuristic = h;
}

Node* AStarSearch::search() {
	int step = 0;
	allFrontiers.clear();
	allExpanded.clear();
	solutionPath.clear();
	consideredNodes = 0;

	Node* initial = new Node(problem.initial, 0);

	auto f = [this](Node* a, Node* b) {
		return a->pathCost + heuristic(a->s, problem.goal) >
			b->pathCost + heuristic(b->s, problem.goal);
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

		//std::cout << "STEP: " << step << 
		//	" COORDS: " << n->s.x << ", " << n->s.y << ", " << n->s.z <<
		//	" HEURISTIC:" << heuristic(n->s, problem.goal) << 
		//	" COST:" << n->pathCost << +
		//	" F-COST: " << n->pathCost + heuristic(n->s, problem.goal) << std::endl;

		allExpanded.push_back(n->s);

		if (problem.isGoal(n->s)) {
			setSolution(n);
			setConsideredNodes();
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

void AStarSearch::setSolution(Node* n) {
	if (n == nullptr) return;
	else {
		solutionPath.push_back(n->s);
		setSolution(n->parent);
	}
}

void AStarSearch::setConsideredNodes() {
	for (std::vector<State> frontier : allFrontiers) {
		consideredNodes += frontier.size();
	}
}

SearchProblem AStarSearch::getProblem() {
	return problem;
}

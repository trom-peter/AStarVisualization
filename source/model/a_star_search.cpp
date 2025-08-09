#include "model/a_star_search.h"

#include <queue>
#include <unordered_map>
#include <set>
#include <functional>
#include "model/topography.h"
#include "configurations/problem_configuration.h"

AStarSearch::AStarSearch(SearchProblem& p, ProblemConfig& config, Topography* topo) :
	problem(p), heuristic(Heuristic(topo, config.heuristic, config.overestimateFactor)), solution(nullptr) {}

AStarSearch::~AStarSearch() {
	delete solution;
}

Node* AStarSearch::search() {
	int step = 0;
	allFrontiers.clear();
	allExpanded.clear();
	solutionPath.clear();
	consideredNodes = 0;

	Node* initial = new Node(problem.initial, 0);

	auto f = [this](Node* a, Node* b) {
		return a->pathCost + heuristic.getFunction()(a->s, problem.goal) >
			b->pathCost + heuristic.getFunction()(b->s, problem.goal);
		};

	std::priority_queue<Node*, std::vector<Node*>, decltype(f)> frontier(f); //grenzbereich
	frontier.push(initial);
	allFrontiers.push_back({ initial->s });

	std::unordered_map<State, Node*, StateHash> reached; // bester pfad pro state
	reached.insert({ problem.initial, initial });

	while (!frontier.empty()) {
		Node* n = frontier.top();
		frontier.pop();

		//skip outdated nodes that are not optimal
		if (reached[n->s]->pathCost != n->pathCost) {
			continue;
		}

		allExpanded.push_back(n->s);

		if (problem.isGoal(n->s)) {
			solution = n;
			setSolutionPath(n);
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

void AStarSearch::setSolutionPath(Node* n) {
	if (n == nullptr) return;
	else {
		solutionPath.push_back(n->s);
		setSolutionPath(n->parent);
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

void AStarSearch::setHeuristic(int heuristicId, float overestimateFactor) {
	heuristic.heuristicId = heuristicId;
	heuristic.overestimateFactor = overestimateFactor;
}

Heuristic AStarSearch::getHeuristic() {
	return heuristic;
}

Node* AStarSearch::getSolution() {
	return solution;
}

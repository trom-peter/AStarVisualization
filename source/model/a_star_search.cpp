#include "model/a_star_search.h"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <set>
#include <functional>
#include "model/search_environment.h"

AStarSearch::AStarSearch(SearchProblem& problem, Heuristic heuristic, SearchEnvironment& environment) :
	problem(problem), heuristic(heuristic), solution(nullptr), consideredNodes(0) {}

AStarSearch::~AStarSearch() {}

void AStarSearch::search() {
	int step = 0;
	allFrontiers.clear();
	allExpanded.clear();
	solutionPath.clear();
	consideredNodes = 0;

	std::shared_ptr<Node> initial = std::make_shared<Node>(problem.initial, 0);

	auto f = [this](std::shared_ptr<Node> a, std::shared_ptr<Node> b) {
		return a->pathCost + heuristic.getFunction()(a->s, problem.goal) >
			b->pathCost + heuristic.getFunction()(b->s, problem.goal);
		};

	std::priority_queue<std::shared_ptr<Node>, 
		std::vector<std::shared_ptr<Node>>, decltype(f)> frontier(f);
	frontier.push(initial);
	allFrontiers.push_back({ initial->s });

	std::unordered_map<State, std::shared_ptr<Node>, StateHash> reached;
	reached.insert({ problem.initial, initial});

	while (!frontier.empty()) {
		std::shared_ptr<Node> n = frontier.top();
		frontier.pop();

		//skip outdated nodes that are not optimal
		if (reached[n->s]->pathCost != n->pathCost) {
			continue;
		}

		allExpanded.push_back(n->s);

		if (problem.isGoal(n->s)) { //solution found
			solution = n;
			setSolutionPath(n);
			setConsideredNodes();
			return;
		}

		for (std::shared_ptr<Node> child : problem.actions(n)) {
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

	solution = nullptr;
}

void AStarSearch::setSolutionPath(const std::shared_ptr<Node> n) {
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

SearchProblem AStarSearch::getProblem() const {
	return problem;
}

void AStarSearch::setHeuristic(const int heuristicId, const float overestimateFactor) {
	heuristic.heuristicId = heuristicId;
	heuristic.overestimateFactor = overestimateFactor;
}

Heuristic AStarSearch::getHeuristic() const {
	return heuristic;
}

std::shared_ptr<Node> AStarSearch::getSolution() const {
	return solution;
}

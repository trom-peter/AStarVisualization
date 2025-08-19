#include "model/a_star_search.h"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <set>
#include <functional>
#include "model/search_environment.h"

AStarSearch::AStarSearch(SearchProblem& problem, Heuristic heuristic, SearchEnvironment& environment) :
	problem(problem), heuristic(heuristic), solution(nullptr), consideredNodes(0) {
}

AStarSearch::~AStarSearch() {}

// Start the search for the current search problem
void AStarSearch::search() {
	int step = 0; // Number of expanded nodes. Used for adding states to allFrontiers at each step

	std::shared_ptr<Node> initial = std::make_shared<Node>(problem.initial, 0);

	// Evaluation function
	auto f = [this](std::shared_ptr<Node> a, std::shared_ptr<Node> b) {
		return a->pathCost + heuristic.getFunction()(a->s, problem.goal) >
			b->pathCost + heuristic.getFunction()(b->s, problem.goal);
		};

	std::priority_queue<std::shared_ptr<Node>,
		std::vector<std::shared_ptr<Node>>, decltype(f)> frontier(f); // Frontier
	frontier.push(initial);
	allFrontiers.push_back({ initial->s });

	std::unordered_map<State, std::shared_ptr<Node>, StateHash> reached; // Reached
	reached.insert({ problem.initial, initial });

	while (!frontier.empty()) {
		std::shared_ptr<Node> n = frontier.top();
		frontier.pop();

		// Skip outdated nodes that are not optimal
		if (reached[n->s]->pathCost != n->pathCost) {
			continue;
		}

		allExpanded.push_back(n->s);

		if (problem.isGoal(n->s)) { // Solution found
			solution = n;
			setSolutionPath(n);
			setConsideredNodes();
			while (!frontier.empty()) {
				frontier.pop(); // Clear rest of frontier
			}
			reached.clear(); // Clear reached
			return;
		}

		// Get all possible actions of current node
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

void AStarSearch::resetSearch() {
	solution = nullptr;
	allFrontiers.clear();
	allExpanded.clear();
	solutionPath.clear();
	consideredNodes = 0;
}

void AStarSearch::setSolutionPath(std::shared_ptr<Node> n) {
    std::vector<State> tmp;
    while (n) {
        tmp.push_back(n->s);
        n = n->parent.lock();
    }
    std::reverse(tmp.begin(), tmp.end());
    solutionPath = std::move(tmp);
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

std::vector<std::vector<State>> AStarSearch::getAllFrontiers() const {
	return allFrontiers;
}

std::vector<State> AStarSearch::getAllExpanded() const {
	return allExpanded;
}

std::vector<State> AStarSearch::getSolutionPath() const {
	return solutionPath;
}

int AStarSearch::getConsideredNodes() const {
	return consideredNodes;
}

#pragma once
#include "model/heuristic.h"
#include "model/search_problem.h"
#include "model/node.h"

struct SearchProblem;
struct SearchEnvironment;
struct Node;
struct Heuristic;

class AStarSearch {
public:
	AStarSearch(SearchProblem& problem, Heuristic heuristic, SearchEnvironment& environment);
	~AStarSearch();

	void search();
	SearchProblem getProblem() const;
	Heuristic getHeuristic() const;
	Node* getSolution() const;
	void setHeuristic(const int heuristicId, const float overestimateFactor = 1.0f);

	std::vector<std::vector<State>> allFrontiers;
	std::vector<State> allExpanded;
	std::vector<State> solutionPath;
	int consideredNodes;

private:
	Node* solution;
	SearchProblem& problem;
	Heuristic heuristic;
	void setConsideredNodes();
	void setSolutionPath(const Node* n);
};
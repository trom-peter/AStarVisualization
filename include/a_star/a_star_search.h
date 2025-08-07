#pragma once
#include "a_star/heuristic.h"
#include "a_star/search_problem.h"
#include "a_star/node.h"

struct SearchProblem;
struct ProblemConfig;
struct Node;
struct Heuristic;

class AStarSearch {
public:
	AStarSearch(SearchProblem& p, ProblemConfig& config, Topography* topo);
	~AStarSearch();

	Node* search();
	SearchProblem getProblem();
	Heuristic getHeuristic();
	Node* getSolution();
	void setHeuristic(int heuristicId, float overestimateFactor = 1.0f);

	std::vector<std::vector<State>> allFrontiers;
	std::vector<State> allExpanded;
	std::vector<State> solutionPath;
	int consideredNodes;

private:
	Node* solution;
	SearchProblem& problem;
	Heuristic heuristic;
	void setConsideredNodes();
	void setSolutionPath(Node* n);
};
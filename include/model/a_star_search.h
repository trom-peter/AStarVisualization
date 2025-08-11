#pragma once
#include "model/heuristic.h"
#include "model/search_problem.h"
#include "model/node.h"

struct SearchProblem;
struct SearchEnvironment;
struct ProblemConfig;
struct Node;
struct Heuristic;

class AStarSearch {
public:
	AStarSearch(SearchProblem& problem, ProblemConfig& config, SearchEnvironment& environment);
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
#pragma once
#include <memory>
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
	void resetSearch();
	void setHeuristic(const int heuristicId, const float overestimateFactor = 1.0f);

	SearchProblem getProblem() const;
	Heuristic getHeuristic() const;
	std::shared_ptr<Node> getSolution() const;
	std::vector<std::vector<State>> getAllFrontiers() const;
	std::vector<State> getAllExpanded() const;
	std::vector<State> getSolutionPath() const;
	int getConsideredNodes() const;

private:
	// Solution node. Is nullptr if search has not started or no solution was found
	std::shared_ptr<Node> solution; 

	// All frontiers of the search process. Only stores new states
	std::vector<std::vector<State>> allFrontiers; 

	// All expanded states in search process
	std::vector<State> allExpanded; 

	// Path of the found solution
	std::vector<State> solutionPath;

	int consideredNodes; // Amount of considered nodes in search process

	SearchProblem& problem;
	Heuristic heuristic;
	void setConsideredNodes();
	void setSolutionPath(const std::shared_ptr<Node> n);
};

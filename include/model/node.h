#pragma once
#include <string>
#include "model/state.h"

struct Node {

	Node(const int x, const int y, const int z, const int pathCost, Node* parent);
	Node(const int x, const int y, const int z, const int pathCost);
	Node(const int x, const int y, const int z, Node* parent, const int pathCost);
	Node(const State state, const int pathCost);
	Node(const State state, Node* parent, const int pathCost);

	std::string getPath() const;

	Node* parent;
	State s;
	int pathCost; // travel time in seconds
};
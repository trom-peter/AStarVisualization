#pragma once
#include <string>
#include "a_star/state.h"

struct Node {

	Node(int x, int y, int z, int pathCost, Node* parent);
	Node(int x, int y, int z, int pathCost);
	Node(int x, int y, int z, Node* parent, int pathCost);
	Node(State state, int pathCost);
	Node(State state, Node* parent, int pathCost);

	std::string getPath();

	Node* parent;
	State s;
	int pathCost; // travel time in seconds
};
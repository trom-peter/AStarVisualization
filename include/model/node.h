#pragma once
#include <string>
#include <memory>
#include "model/state.h"

struct Node {
	Node(const int x, const int y, const int z, const int pathCost, std::weak_ptr<Node> parent);
	Node(const int x, const int y, const int z, const int pathCost);
	Node(const int x, const int y, const int z, std::weak_ptr<Node> parent, const int pathCost);
	Node(const State state, const int pathCost);
	Node(const State state, std::weak_ptr<Node> parent, const int pathCost);

	std::weak_ptr<Node> parent;
	State s;
	int pathCost; // Travel time in seconds
};

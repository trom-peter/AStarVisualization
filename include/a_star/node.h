#pragma once
#include <string>
#include "a_star/state.h"

struct Node {
	Node(int x, int y, int z, int pathCost, Node* parent) : 
		s(State(x, y, z)), parent(parent), pathCost(pathCost) {};

	Node(int x, int y, int z, int pathCost)	: 
		s(State(x, y, z)), parent(nullptr), pathCost(pathCost) {};

	Node(int x, int y, int z, Node* parent, int pathCost) :
		s(State(x, y, z)), parent(parent), pathCost(pathCost) {};

	Node(State state, int pathCost) : 
		s(state), parent(nullptr), pathCost(pathCost) {};

	Node(State state, Node* parent, int pathCost) : 
		s(state), parent(parent), pathCost(pathCost) {};

	std::string getPath() {
		if (parent == nullptr) return "[" + std::to_string(s.x) + ", " + std::to_string(s.y) + ", " + std::to_string(s.z) + "]";
		return parent->getPath() + " -> " + "[" + std::to_string(s.x) + ", " + std::to_string(s.y) + ", " + std::to_string(s.z) + "]";
	}

	Node* parent;
	State s;
	int pathCost; // travel time in seconds
};
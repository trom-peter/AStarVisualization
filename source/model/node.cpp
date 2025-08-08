#include "model/node.h"
#include <string>
#include "model/state.h"

Node::Node(int x, int y, int z, int pathCost, Node* parent) :
	s(State(x, y, z)), parent(parent), pathCost(pathCost) {
};

Node::Node(int x, int y, int z, int pathCost) :
	s(State(x, y, z)), parent(nullptr), pathCost(pathCost) {
};

Node::Node(int x, int y, int z, Node* parent, int pathCost) :
	s(State(x, y, z)), parent(parent), pathCost(pathCost) {
};

Node::Node(State state, int pathCost) :
	s(state), parent(nullptr), pathCost(pathCost) {
};

Node::Node(State state, Node* parent, int pathCost) :
	s(state), parent(parent), pathCost(pathCost) {
};

std::string Node::getPath() {
	if (parent == nullptr) return "[" + std::to_string(s.x) + ", " + std::to_string(s.y) + ", " + std::to_string(s.z) + "]";
	return parent->getPath() + " -> " + "[" + std::to_string(s.x) + ", " + std::to_string(s.y) + ", " + std::to_string(s.z) + "]";
}

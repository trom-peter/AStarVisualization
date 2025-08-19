#include "model/node.h"
#include "model/state.h"
#include <string>

Node::Node(const int x, const int y, const int z, const int pathCost, std::weak_ptr<Node> parent) :
	s(State(x, y, z)), parent(parent), pathCost(pathCost) {
};

Node::Node(const int x, const int y, const int z, const int pathCost) :
	s(State(x, y, z)), pathCost(pathCost) {
};

Node::Node(const int x, const int y, const int z, std::weak_ptr<Node> parent, const int pathCost) :
	s(State(x, y, z)), parent(parent), pathCost(pathCost) {
};

Node::Node(const State state, const int pathCost) :
	s(state), pathCost(pathCost) {
};

Node::Node(const State state, std::weak_ptr<Node> parent, const int pathCost) :
	s(state), parent(parent), pathCost(pathCost) {
};


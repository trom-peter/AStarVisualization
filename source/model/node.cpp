#include "model/node.h"
#include "model/state.h"
#include <string>

Node::Node(const int x, const int y, const int z, const int pathCost, std::shared_ptr<Node> parent) :
	s(State(x, y, z)), parent(parent), pathCost(pathCost) {
};

Node::Node(const int x, const int y, const int z, const int pathCost) :
	s(State(x, y, z)), parent(nullptr), pathCost(pathCost) {
};

Node::Node(const int x, const int y, const int z, std::shared_ptr<Node> parent, const int pathCost) :
	s(State(x, y, z)), parent(parent), pathCost(pathCost) {
};

Node::Node(const State state, const int pathCost) :
	s(state), parent(nullptr), pathCost(pathCost) {
};

Node::Node(const State state, std::shared_ptr<Node> parent, const int pathCost) :
	s(state), parent(parent), pathCost(pathCost) {
};

std::string Node::getPath() const {
	if (parent == nullptr) {
		return "[" + std::to_string(s.x) + ", " + 
			std::to_string(s.y) + ", " + 
			std::to_string(s.z) + "]";
	}
	else {
		return parent->getPath() + " -> " + 
			"[" + std::to_string(s.x) + ", " + 
			std::to_string(s.y) + ", " + 
			std::to_string(s.z) + "]";
	}
}

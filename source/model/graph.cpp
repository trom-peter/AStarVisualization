#include "model/graph.h"
#include <iostream>
#include "model/state.h"

Graph::Graph(int nodes, int size) : n(nodes), size(size) {
	spacing = size / (nodes - 1);
	int totalNodes = n * n;
	adjMatrix.resize(totalNodes, std::vector<int>(totalNodes, 0));
}

void Graph::reset(int nodes, int size) {
	this->n = nodes;
	this->size = size;
	spacing = size / (nodes - 1);
	int totalNodes = n * n;
	adjMatrix.clear();
	adjMatrix.resize(totalNodes, std::vector<int>(totalNodes, 0));
}

std::vector<State> Graph::getNeighbours(int x, int z) {
	std::vector<State> neighbours;

	//all eight x,z neighbour offsets
	int neighbourOffsets[8][2] = { 
		{0, spacing},			// top neighbour
		{-spacing, spacing},	// top left neighbour
		{-spacing, 0},			// left neighbour
		{-spacing, -spacing},	// bottom left neighbour
		{0, -spacing},			// bottom neighbour
		{spacing, -spacing},	// bottom right neighbour
		{spacing, 0},			// right neighbour
		{spacing, spacing}		// top right neighbour
	};

	for (int i = 0; i < 8; i++) {
		int neighbourX = x + neighbourOffsets[i][0];
		int neighbourZ = z + neighbourOffsets[i][1];

		if (isValid(neighbourX, neighbourZ))
			neighbours.push_back(State(neighbourX, topo->getY(neighbourX, neighbourZ), neighbourZ));
	}

	return neighbours;
}

int Graph::toNodeIndex(int x, int z) const {
	x /= spacing;
	z /= spacing;
	return z * n + x;
}

// does graph contain a node at a given position
bool Graph::isValid(int x, int z) const {
	return (x >= 0 && x <= size && z >= 0 && z <= size);
}

// does graph contain a node at index i
bool Graph::isValid(int i) const {
	return (0 <= i) && (i < n * n);
}

void Graph::addEdge(int i, int j, int weight) {
	adjMatrix[i][j] = weight;
	adjMatrix[j][i] = weight;
}

void Graph::addEdge(int x1, int z1, int x2, int z2, int weight) {
	addEdge(toNodeIndex(x1, z1), toNodeIndex(x2, z2), weight);
}

int Graph::getWeight(int x1, int z1, int x2, int z2) {
	int i = toNodeIndex(x1, z1);
	int j = toNodeIndex(x2, z2);
	return adjMatrix[i][j];
}

bool Graph::hasEdge(int x1, int z1, int x2, int z2) {
	int i = toNodeIndex(x1, z1);
	int j = toNodeIndex(x2, z2);
	return adjMatrix[i][j] || adjMatrix[j][i];
}

void Graph::printAdjacencyMatrix() {
	for (int i = 0; i < n * n; i++) {
		for (int j = 0; j < n * n; j++) {
			std::cout << adjMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

float Graph::getSpacing() {
	return spacing;
}

void Graph::setTopography(Topography* topo) {
	this->topo = topo;
}

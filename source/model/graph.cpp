#include "model/graph.h"
#include "model/state.h"
#include <iostream>

// Maximum amount of neighbours per state
constexpr int MAX_NEIGHBOURS = 8;

Graph::Graph(const int resolution, const int size, Topography& topo) : 
	resolution(resolution), size(size), topography(topo) 
{
	spacing = size / (resolution - 1);
	totalNodes = resolution * resolution;
	adjMatrix.resize(totalNodes, std::vector<int>(totalNodes, 0));
}

void Graph::reset(const int resolution, const int size) {
	this->resolution = resolution;
	this->size = size;
	spacing = size / (resolution - 1);
	totalNodes = resolution * resolution;
	adjMatrix.clear();
	adjMatrix.resize(totalNodes, std::vector<int>(totalNodes, 0));
}

std::vector<State> Graph::getNeighbours(const int x, const int z) const {
	std::vector<State> neighbours;

	//all eight x,z neighbour offsets
	int neighbourOffsets[MAX_NEIGHBOURS][2] = {
		{0, spacing},			// top neighbour
		{-spacing, spacing},	// top left neighbour
		{-spacing, 0},			// left neighbour
		{-spacing, -spacing},	// bottom left neighbour
		{0, -spacing},			// bottom neighbour
		{spacing, -spacing},	// bottom right neighbour
		{spacing, 0},			// right neighbour
		{spacing, spacing}		// top right neighbour
	};

	for (int i = 0; i < MAX_NEIGHBOURS; i++) {
		int neighbourX = x + neighbourOffsets[i][0];
		int neighbourZ = z + neighbourOffsets[i][1];

		if (isValid(neighbourX, neighbourZ)) {
			neighbours.push_back(
				State(neighbourX, 
					topography.getY(neighbourX, neighbourZ), 
					neighbourZ));
		}
	}

	return neighbours;
}

int Graph::toNodeIndex(int x, int z) const {
	x /= spacing;
	z /= spacing;
	return z * resolution + x;
}

// does graph contain a node at a given position
bool Graph::isValid(const int x, const int z) const {
	return (x >= 0 && x <= size && z >= 0 && z <= size);
}

// does graph contain a node at index i
bool Graph::isValid(const int i) const {
	return (0 <= i) && (i < totalNodes);
}

void Graph::addEdge(const int i, const int j, const int weight) {
	adjMatrix[i][j] = weight;
	adjMatrix[j][i] = weight;
}

void Graph::addEdge(const int x1, const int z1, 
	const int x2, const int z2, 
	const int weight) 
{
	addEdge(toNodeIndex(x1, z1), toNodeIndex(x2, z2), weight);
}

int Graph::getWeight(const int x1, const int z1, const int x2, const int z2) const {
	int i = toNodeIndex(x1, z1);
	int j = toNodeIndex(x2, z2);
	return adjMatrix[i][j];
}

bool Graph::hasEdge(const int x1, const int z1, const int x2, const int z2) const {
	int i = toNodeIndex(x1, z1);
	int j = toNodeIndex(x2, z2);
	return adjMatrix[i][j] || adjMatrix[j][i];
}

void Graph::printAdjacencyMatrix() const {
	for (int i = 0; i < totalNodes; i++) {
		for (int j = 0; j < totalNodes; j++) {
			std::cout << adjMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

float Graph::getSpacing() const {
	return spacing;
}

void Graph::setTopography(Topography& topo) {
	this->topography = topo;
}

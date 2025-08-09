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

//add grid edges
void Graph::init() {
	for (int z = 0; z <= size; z += spacing) {
		for (int x = 0; x <= size; x += spacing) {

			// get index of this node
			int i = toNodeIndex(x, z);

			// add top neighbour
			if (z < n - 1) {
				int top = toNodeIndex(x, z + 1);
				addEdge(i, top, 1);
			}

			// add top left neighbour
			if (z < n - 1 && x > 0) {
				int topLeft = toNodeIndex(x - 1, z + 1);
				addEdge(i, topLeft, 1);
			}

			// add left neighbour
			if (x > 0) {
				int left = toNodeIndex(x - 1, z);
				addEdge(i, left, 1);
			}

			// add bottom left neighbour
			if (x > 0 && z > 0) {
				int bottomLeft = toNodeIndex(x - 1, z - 1);
				addEdge(i, bottomLeft, 1);
			}

			// add bottom neighbour
			if (z > 0) {
				int bottom = toNodeIndex(x, z - 1);
				addEdge(i, bottom, 1);
			}

			// add bottom right neighbour
			if (z > 0 && x < n - 1) {
				int bottomRight = toNodeIndex(x + 1, z - 1);
				addEdge(i, bottomRight, 1);
			}

			// add right neighbour
			if (x < n - 1) {
				int right = toNodeIndex(x + 1, z);
				addEdge(i, right, 1);
			}

			// add top right neighbour
			if (x < n - 1 && z < n - 1) {
				int topRight = toNodeIndex(x + 1, z + 1);
				addEdge(i, topRight, 1);
			}
		}
	}
}

std::vector<State> Graph::getNeighbours(int x, int z) {
	std::vector<State> neighbours;

	//calculate node position in graph (gX and gZ are between 0 and n)
	int gX = x / spacing;
	int gZ = z / spacing;

	if (gZ < n - 1)
		neighbours.push_back(State(x, topo->getY(x, z + spacing), (z + spacing))); // top neighbour

	if (gZ < n - 1 && gX > 0)
		neighbours.push_back(State((x - spacing), topo->getY(x - spacing, z + spacing), (z + spacing))); // top left neighbour

	if (gX > 0)
		neighbours.push_back(State((x - spacing), topo->getY(x - spacing, z), z)); // left neighbour

	if (gX > 0 && gZ > 0)
		neighbours.push_back(State((x - spacing), topo->getY(x - spacing, z - spacing), (z - spacing))); // bottom left neighbour

	if (gZ > 0)
		neighbours.push_back(State(x, topo->getY(x, z - spacing), (z - spacing))); // bottom neighbour

	if (gZ > 0 && gX < n - 1)
		neighbours.push_back(State((x + spacing), topo->getY(x + spacing, z - spacing), (z - spacing))); // bottom right neighbour

	if (gX < n - 1)
		neighbours.push_back(State((x + spacing), topo->getY(x + spacing, z), z)); // right neighbour

	if (gX < n - 1 && gZ < n - 1)
		neighbours.push_back(State((x + spacing), topo->getY(x + spacing, z + spacing), (z + spacing))); // top right neighbour

	return neighbours;
}

int Graph::toNodeIndex(int x, int z) const {
	x /= spacing;
	z /= spacing;
	return z * n + x;
}

// does graph contain a node at a given position
bool Graph::isValid(int x, int z) const {
	return isValid(toNodeIndex(x, z));
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
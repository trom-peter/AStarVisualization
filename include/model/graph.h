#pragma once
#include <vector>
#include "model/topography.h"

struct State;

class Graph {
public:
	Graph(const int nodes, const int size);

	void reset(const int nodes, const int size);
	std::vector<State> getNeighbours(const int x, const int z) const;
	int toNodeIndex(int x, int z) const;
	bool isValid(const int x, const int z) const;
	bool isValid(const int i) const;			
	void addEdge(const int i, const int j, const int weight);
	void addEdge(const int x1, const int z1, const int x2, const int z2, const int weight);
	int getWeight(const int x1, const int z1, const int x2, const int z2) const;
	bool hasEdge(const int x1, const int z1, const int x2, const int z2) const;
	void printAdjacencyMatrix() const;
	float getSpacing() const;
	void setTopography(Topography* topo);

private:
	int n; // size of grid (n x n nodes)
	int size; //size in units (size x size units)
	int spacing; //horizontal, vertical space between all nodes
	std::vector<std::vector<int>> adjMatrix; //adjacency matrix for storing edges and weights
	Topography* topography; // topography object for getting y values for any x, z point
};
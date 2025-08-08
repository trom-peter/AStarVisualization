#pragma once
#include <vector>
#include "topography.h"

struct State;

class Graph {
public:
	Graph(int nodes, int size);

	void reset(int nodes, int size);
	void init(); //add initial grid edges
	std::vector<State> getNeighbours(int x, int z);
	int toNodeIndex(int x, int z) const;
	bool isValid(int x, int z) const; // does graph contain a node at a given position
	bool isValid(int i) const;	// does graph contain a node at index i
	void addEdge(int i, int j, int weight);
	void addEdge(int x1, int z1, int x2, int z2, int weight);
	int getWeight(int x1, int z1, int x2, int z2);
	bool hasEdge(int x1, int z1, int x2, int z2);
	void printAdjacencyMatrix();
	float getSpacing();
	void setTopography(Topography* topo);

private:
	int n; // size of grid (n x n nodes)
	int size; //size in units (size x size units)
	int spacing; //horizontal, vertical space between all nodes
	std::vector<std::vector<int>> adjMatrix;
	Topography* topo; // topography object for getting y values for any x, z point
};
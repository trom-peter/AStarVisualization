#pragma once
#include <vector>
#include "model/topography.h"

struct State;

class Graph {
public:
	Graph(const int resolution, const int size, Topography& topo);

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
	void setTopography(Topography& topo);

private:
	int resolution; // Resolution of grid
	int totalNodes; // Total amount of nodes in graph (resolution * resolution)
	int size; // Size in units (size x size units)
	int spacing; // Horizontal, vertical space between all nodes
	std::vector<std::vector<int>> adjMatrix; // Adjacency matrix for storing edges and weights
	Topography& topography; // Topography object for getting y values for any x, z point
};
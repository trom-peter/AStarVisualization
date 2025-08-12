#pragma once
#include "model/state_grid.h"
#include "model/graph.h"
#include "model/topography.h"

class Stategrid;

struct SearchEnvironment {

	SearchEnvironment(
		unsigned char seed, float terrainScaling, int topographyType, 
		int topographySize, int topographyAmplitude,
		int gridSize, Stategrid stategrid);

	void resetGrid(int gridSize);
	void resetTopography(unsigned char seed, float scale, bool type);

	Topography topography;
	Stategrid stateGrid;
	Graph graph;
};
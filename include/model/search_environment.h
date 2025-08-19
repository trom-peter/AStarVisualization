#pragma once
#include "model/state_grid.h"
#include "model/graph.h"
#include "model/topography.h"

class Stategrid;

struct SearchEnvironment {
	SearchEnvironment(
		const unsigned char seed, const float terrainScaling, const int topographyType,
		const int topographySize, const int topographyAmplitude,
		const int gridSize, const Stategrid stategrid);

	void resetGrid(const int gridSize);
	void resetTopography(const unsigned char seed, const float scale, const bool type);

	Topography topography;
	Stategrid stategrid;
	Graph graph;
};

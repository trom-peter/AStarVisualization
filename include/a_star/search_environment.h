#pragma once
#include "a_star/state_grid.h"
#include "topography.h"
#include "a_star/graph.h"

struct EnvironmentConfig;
struct StateGridConfig;
class Stategrid;

struct SearchEnvironment {

	SearchEnvironment(EnvironmentConfig& envConfig, StategridConfig& stateConfig);

	void resetGrid(int gridSize);
	void resetTopography(unsigned char seed, float scale, bool type);

	Topography topography;
	Stategrid stateGrid;
	Graph graph;
};

#pragma once
#include "model/state_grid.h"
#include "model/graph.h"
#include "model/topography.h"

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
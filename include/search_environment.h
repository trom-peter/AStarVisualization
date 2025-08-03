#pragma once
#include "topography.h"
#include "state_grid.h"
#include "a_star/graph.h"

struct SearchEnvironment {
	SearchEnvironment(unsigned char seed, int gridSize, float scale, bool type) : 
		topography(Topography(seed, scale, type)), graph(new Graph(15, topography.getSize()))
	{
		topography.generate();
		graph->init();
		graph->setTopography(&topography);
		stateGrid = new StateGrid(gridSize);
		resetGrid(gridSize);
	}

	void resetGrid(int gridSize) {
		stateGrid->grid.clear();
		stateGrid->gridSize = gridSize;
		stateGrid->initGrid(&topography);
	}

	void resetTopography(unsigned char seed, float scale, bool type) {
		topography.setSeed(seed);
		topography.setType(type);
		topography.setScale(scale);
		topography.generate();
	}

	Topography topography;
	StateGrid* stateGrid;
	Graph* graph;
};

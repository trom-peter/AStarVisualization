#include "model/search_environment.h"
#include "model/state_grid.h"

SearchEnvironment::SearchEnvironment(
	unsigned char seed, float terrainScaling, int topographyType, 
	int topographySize, int topographyAmplitude,
	int gridSize, Stategrid stategrid) :
	topography(Topography(seed, terrainScaling, topographyType, topographySize, topographyAmplitude)),
	graph(Graph(gridSize, topographySize)),
	stateGrid(stategrid)
{
	topography.setMinMaxHeight();
	graph.setTopography(&topography);
	stateGrid.initGrid(&topography);
}

void SearchEnvironment::resetGrid(int gridSize) {
	stateGrid.clearGrid();
	stateGrid.gridSize = gridSize;
	stateGrid.initGrid(&topography);
}

void SearchEnvironment::resetTopography(unsigned char seed, float scale, bool type) {
	topography.setSeed(seed);
	topography.setType(type);
	topography.setScale(scale);
	topography.setMinMaxHeight();
}
#include "model/search_environment.h"
#include "model/state_grid.h"

SearchEnvironment::SearchEnvironment(
	const unsigned char seed, const float terrainScaling, const int topographyType,
	const int topographySize, const int topographyAmplitude,
	const int gridSize, const Stategrid stategrid) :
	topography(Topography(seed, terrainScaling, topographyType, topographySize, topographyAmplitude)),
	graph(Graph(gridSize, topographySize, topography)),
	stateGrid(stategrid)
{
	topography.setMinMaxHeight();
	stateGrid.initGrid(topography);
}

void SearchEnvironment::resetGrid(const int gridSize) {
	stateGrid.clearGrid();
	stateGrid.gridSize = gridSize;
	stateGrid.initGrid(topography);
}

void SearchEnvironment::resetTopography(const unsigned char seed, const float scale, const bool type) {
	topography.setSeed(seed);
	topography.setType(type);
	topography.setScale(scale);
	topography.setMinMaxHeight();
}
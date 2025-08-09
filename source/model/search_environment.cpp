#include "model/search_environment.h"
#include "model/state_grid.h"
#include "configurations/environment_configuration.h"
#include "configurations/stategrid_configuration.h"

SearchEnvironment::SearchEnvironment(EnvironmentConfig& envConfig, StategridConfig& stateConfig) :
	topography(
		Topography(envConfig.seed, envConfig.terrainScaling, envConfig.topographyType,
			envConfig.topographySize, envConfig.topographyAmplitude)),
	graph(Graph(envConfig.gridSize, envConfig.topographySize)),
	stateGrid(Stategrid(envConfig.gridSize, stateConfig))
{
	topography.setMinMaxHeight();
	graph.init();
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
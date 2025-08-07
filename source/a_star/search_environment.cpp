#include "a_star/search_environment.h"
#include "a_star/state_grid.h"
#include "configurations/environment_configuration.h"
#include "configurations/stategrid_configuration.h"

SearchEnvironment::SearchEnvironment(EnvironmentConfig& envConfig, StategridConfig& stateConfig) :
	topography(
		Topography(envConfig.seed, envConfig.terrainScaling, envConfig.topographyType,
			envConfig.topographySize, envConfig.topographyAmplitude)),
	graph(Graph(envConfig.gridSize, envConfig.topographySize)),
	stateGrid(Stategrid(envConfig.gridSize, stateConfig))
{
	topography.generate();
	graph.init();
	graph.setTopography(&topography);
	resetGrid(envConfig.gridSize);
}

void SearchEnvironment::resetGrid(int gridSize) {
	stateGrid.grid.clear();
	stateGrid.gridSize = gridSize;
	stateGrid.initGrid(&topography);
	stateGrid.defaultVisible = true;
}

void SearchEnvironment::resetTopography(unsigned char seed, float scale, bool type) {
	topography.setSeed(seed);
	topography.setType(type);
	topography.setScale(scale);
	topography.generate();
}

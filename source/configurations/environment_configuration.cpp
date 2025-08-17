#include "configurations/environment_configuration.h"

//initial environment configuration
int constexpr SEED = 0;
int constexpr TOPOGRAPHY_TYPE = 0;
float constexpr TERRAIN_SCALING = EnvironmentConfig::COMPLEXITY_LOW;
int constexpr GRID_SIZE = 10;
int constexpr TOPOGRAPHY_SIZE = 7000;
int constexpr TOPOGRAPHY_AMPLITUDE = 1000;
int constexpr STATE_SPACING = (TOPOGRAPHY_SIZE / (GRID_SIZE - 1));

EnvironmentConfig::EnvironmentConfig() : 
	seed(SEED),
	topographyType(TOPOGRAPHY_TYPE),
	terrainScaling(TERRAIN_SCALING),
	gridSize(GRID_SIZE),
	topographySize(TOPOGRAPHY_SIZE),
	topographyAmplitude(TOPOGRAPHY_AMPLITUDE),
	stateSpacing(STATE_SPACING) {}

//set to initial configuration
void EnvironmentConfig::reset() {
	seed = SEED;
	topographyType = TOPOGRAPHY_TYPE;
	terrainScaling = TERRAIN_SCALING;
	gridSize = GRID_SIZE;
	topographySize = TOPOGRAPHY_SIZE;
	topographyAmplitude = TOPOGRAPHY_AMPLITUDE;
	stateSpacing = STATE_SPACING;
}

void EnvironmentConfig::updateStateSpacing() {
	this->stateSpacing = topographySize / (gridSize - 1);
}

#include "configurations/environment_configuration.h"

// Initial environment configuration
constexpr int SEED = 0;
constexpr int TOPOGRAPHY_TYPE = 0;
constexpr float TERRAIN_SCALING = EnvironmentConfig::COMPLEXITY_LOW;
constexpr int GRID_SIZE = 15;
constexpr int TOPOGRAPHY_SIZE = 7000;
constexpr int TOPOGRAPHY_AMPLITUDE = 500;
constexpr int STATE_SPACING = (TOPOGRAPHY_SIZE / (GRID_SIZE - 1));

EnvironmentConfig::EnvironmentConfig() : 
	seed(SEED),
	topographyType(TOPOGRAPHY_TYPE),
	terrainScaling(TERRAIN_SCALING),
	gridResolution(GRID_SIZE),
	topographySize(TOPOGRAPHY_SIZE),
	topographyAmplitude(TOPOGRAPHY_AMPLITUDE),
	stateSpacing(STATE_SPACING) {}

// Set to initial configuration
void EnvironmentConfig::reset() {
	seed = SEED;
	topographyType = TOPOGRAPHY_TYPE;
	terrainScaling = TERRAIN_SCALING;
	gridResolution = GRID_SIZE;
	topographySize = TOPOGRAPHY_SIZE;
	topographyAmplitude = TOPOGRAPHY_AMPLITUDE;
	stateSpacing = STATE_SPACING;
}

void EnvironmentConfig::updateStateSpacing() {
	this->stateSpacing = topographySize / (gridResolution - 1);
}

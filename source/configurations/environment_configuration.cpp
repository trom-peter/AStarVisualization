#include "configurations/environment_configuration.h"

void EnvironmentConfig::init() {
	seed = 0;
	topographyType = 0;
	terrainScaling = COMPLEXITY_LOW;
	gridSize = 10;
	topographySize = 7000;
	topographyAmplitude = 1000;
	stateSpacing = topographySize / (gridSize - 1);
}

void EnvironmentConfig::updateStateSpacing() {
	this->stateSpacing = topographySize / (gridSize - 1);
}

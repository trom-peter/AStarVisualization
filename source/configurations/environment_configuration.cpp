#include "configurations/environment_configuration.h"

void EnvironmentConfig::init() {
	seed = 0;
	topographyType = 0;
	terrainScaling = 0.0001f;
	gridSize = 15;
	topographySize = 7000;
	topographyAmplitude = 500;
	stateSpacing = topographySize / (gridSize - 1);
}

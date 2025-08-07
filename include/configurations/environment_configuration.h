#pragma once
#include "glm/glm.hpp"

struct EnvironmentConfig {

	void init();

	unsigned char seed; // with what seed the topography should be generated
	int topographyType;
	int topographySize;
	int topographyAmplitude;
	float terrainScaling; // the higher this parameter, the larger the terrain area
	int gridSize;
	int stateSpacing;
};

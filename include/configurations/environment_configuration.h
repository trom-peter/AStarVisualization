#pragma once
#include "glm/glm.hpp"

struct EnvironmentConfig {
	EnvironmentConfig();

	void reset();
	void updateStateSpacing();

	unsigned char seed; // with what seed the topography should be generated
	int topographyType;
	int topographySize;
	int topographyAmplitude;

	// the higher this parameter, the larger the terrain area (also known as terrain complexity)
	float terrainScaling; 

	int gridSize;
	int stateSpacing;

	static constexpr float COMPLEXITY_LOW = 0.0001f;
	static constexpr float COMPLEXITY_MEDIUM = 0.00015f;
	static constexpr float COMPLEXITY_HIGH = 0.0002f;
};
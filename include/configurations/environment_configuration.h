#pragma once
#include "glm/glm.hpp"

struct EnvironmentConfig {
	EnvironmentConfig();

	void reset();
	void updateStateSpacing();

	unsigned char seed; // With what seed the topography should be generated
	int topographyType; // Type of topography (0/1)
	int topographySize; // Size of topography in meters
	int topographyAmplitude; // Amplitude of topography in meters

	// The higher this parameter, the larger the terrain area. Also known as terrain complexity
	float terrainScaling; 

	int gridResolution;
	int stateSpacing; // Horizontal spacing between states in meters

	// Terrain scaling values for every complexity
	static constexpr float COMPLEXITY_LOW = 0.0001f;
	static constexpr float COMPLEXITY_MEDIUM = 0.00015f;
	static constexpr float COMPLEXITY_HIGH = 0.0002f;

	// Minimum and maximum grid resolution
	static constexpr int GRID_RESOLUTION_MIN = 10;
	static constexpr int GRID_RESOLUTION_MAX = 30;
};
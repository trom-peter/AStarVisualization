#pragma once
#include "glm/glm.hpp"

struct PlaybackConfig {
	PlaybackConfig();

	void reset();
	void updateTimePerIncrement();

	int step; // Step number in search process. Measured in number of expanded nodes
	int maxSteps; // Maximum steps in search process
	float searchTime; // Time spent searching, in seconds
	int searchRate; // Steps per second
	bool searchPlaying; // Is the search currently automatically playing
	float timePerIncrement; // How many seconds a step increment takes

	// Minimum and maximum search rate
	constexpr static int SEARCH_RATE_MIN = 1;
	constexpr static int SEARCH_RATE_MAX = 60;
};

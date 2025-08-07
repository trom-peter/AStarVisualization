#pragma once
#include "glm/glm.hpp"

struct PlaybackConfig {

	void init();
	void updateTimePerIncrement();

	int step; // step number in search process. measured in number of expanded nodes
	int maxSteps;
	float searchTime;
	int searchRate;
	bool searchPlaying;
	float timePerIncrement;
};

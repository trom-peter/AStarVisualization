#pragma once
#include "glm/glm.hpp"

struct PlaybackConfig {

	void init();
	void updateTimePerIncrement();

	int step; // step number in search process. measured in number of expanded nodes
	int maxSteps; // maximum steps in search process
	float searchTime;
	int searchRate; //steps per second
	bool searchPlaying;
	float timePerIncrement;
};

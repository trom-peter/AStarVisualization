#pragma once
#include "glm/glm.hpp"

struct SearchConfiguration {

	SearchConfiguration() {
		reset();
	}

	void reset() {
		initial = glm::ivec2(0, 0);
		goal = glm::ivec2(0, 0);
		gridSize = 0;
		stateSpacing = 0;
		step = 0;
		previousStep = 0;
		maxSteps = 0;
		finished = false;
		searching = false;
		configuring = true;
		initialStateColor = glm::vec3(0.2f, 1.0f, 0.2f);
		goalStateColor = glm::vec3(1.0f, 0.2f, 0.2f);
		defaultColor = glm::vec3(0.0f, 0.0f, 0.0f);
		frontierColor = glm::vec3(1.0f, 1.0f, 1.0f);
		reachedColor = glm::vec3(0.17f, 0.63f, 1.0f);
		frontierVisible = true;
		reachedVisible = true;
		unexploredVisible = true;
	}

	glm::ivec2 initial;
	glm::ivec2 goal;
	int gridSize;
	int stateSpacing; //the perpendicular space between all states
	int step; // step number in search process. measured in number of expanded nodes
	int previousStep;
	int maxSteps;
	bool configuring; //true when visualization is in configuring mode
	bool searching; //true when visualization is in searching mode
	bool finished;
	glm::vec3 initialStateColor;
	glm::vec3 goalStateColor;
	glm::vec3 defaultColor;
	glm::vec3 frontierColor;
	glm::vec3 reachedColor;
	bool frontierVisible;
	bool reachedVisible;
	bool unexploredVisible;
};
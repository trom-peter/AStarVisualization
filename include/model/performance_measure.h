#pragma once
#include "model/state.h"

struct PerformanceMeasure {
	static float hikingFunction(State a, State b) {
		float dx = abs(a.x - b.x);
		float dh = abs(a.y - b.y);
		float dz = abs(a.z - b.z);

		float horizontalDistance = sqrt(dx * dx + dz * dz);

		if (horizontalDistance == 0.0f) return 0.0f; // no movement

		float slope = dh / horizontalDistance;
		float distance = (sqrt(dx * dx + dh * dh + dz * dz));
		float speed = (6 * exp(-3.5 * fabsf(slope + 0.05)));
		float travelTime = ((distance / 1000) / speed) * 3600; // travel time in seconds

		return travelTime;
	};
};

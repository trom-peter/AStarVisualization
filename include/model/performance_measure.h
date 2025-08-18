#pragma once
#include "model/state.h"

struct PerformanceMeasure {
	// hikingFunction() based on Toblers hiking function:
	// Tobler, W. (1999). Three Presentations On Geographical Analysis And Modeling.
	static float hikingFunction(const State a, const State b) {
		int dx = abs(a.x - b.x);
		int dh = abs(a.y - b.y);
		int dz = abs(a.z - b.z);

		float horizontalDistance = (float)sqrt(dx * dx + dz * dz);

		if (horizontalDistance == 0.0f) return 0.0f; // No movement

		float slope = dh / horizontalDistance;
		float distance = (float)(sqrt(dx * dx + dh * dh + dz * dz));
		float speed = (6 * exp(-3.5f * fabsf(slope + 0.05f)));
		float travelTime = ((distance / 1000.0f) / speed) * 3600.0f; // Travel time in seconds

		return travelTime;
	};
};

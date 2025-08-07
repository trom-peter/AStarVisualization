#include "configurations/playback_configuration.h"

void PlaybackConfig::init() {
	step = 0;
	maxSteps = 0;
	searchTime = 0.0f;
	searchRate = 10.0f;
	searchPlaying = false;
	timePerIncrement = 1.0f / searchRate;
}

void PlaybackConfig::updateTimePerIncrement() {
	timePerIncrement = 1.0f / searchRate;
}
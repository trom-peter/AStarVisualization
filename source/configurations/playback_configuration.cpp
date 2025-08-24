#include "configurations/playback_configuration.h"

// Initial playback configuration
constexpr int STEP = 0;
constexpr int MAX_STEPS = 0;
constexpr float SEARCH_TIME = 0.0f;
constexpr float SEARCH_RATE = 10.0f;
constexpr bool SEARCH_PLAYING = false;
constexpr float TIME_PER_INCREMENT = 1.0f / SEARCH_RATE;

PlaybackConfig::PlaybackConfig() : 
	step(STEP),
	maxSteps(MAX_STEPS),
	searchTime(SEARCH_TIME),
	searchRate(SEARCH_RATE),
	searchPlaying(SEARCH_PLAYING),
	timePerIncrement(TIME_PER_INCREMENT) {}

// Reset config to initial configuration
void PlaybackConfig::reset() {
	step = STEP;
	maxSteps = MAX_STEPS;
	searchTime = SEARCH_TIME;
	searchRate = SEARCH_RATE;
	searchPlaying = SEARCH_PLAYING;
	timePerIncrement = TIME_PER_INCREMENT;
}

void PlaybackConfig::updateTimePerIncrement() {
	timePerIncrement = 1.0f / searchRate;
}

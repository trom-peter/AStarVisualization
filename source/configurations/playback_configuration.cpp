#include "configurations/playback_configuration.h"

int constexpr STEP = 0;
int constexpr MAX_STEPS = 0;
float constexpr SEARCH_TIME = 0.0f;
float constexpr SEARCH_RATE = 10.0f;
bool constexpr SEARCH_PLAYING = false;
float constexpr TIME_PER_INCREMENT = 1.0f / SEARCH_RATE;

PlaybackConfig::PlaybackConfig() : 
	step(STEP),
	maxSteps(MAX_STEPS),
	searchTime(SEARCH_TIME),
	searchRate(SEARCH_RATE),
	searchPlaying(SEARCH_PLAYING),
	timePerIncrement(TIME_PER_INCREMENT) {
}

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
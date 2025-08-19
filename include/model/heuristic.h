#pragma once
#include <functional>
#include "model/topography.h"

struct State;
class Topography;

struct Heuristic {
    Heuristic(Topography& topo, const int heuristicId = 0, const float overestimateFactor = 1.0f);

    std::function<int(State, State)> getFunction() const;
    int heuristicId;
    Topography& topo;
    float overestimateFactor; // What factor travelTime_Overestimated should be overestimated by

private:
    // Heuristics
    static std::function<int(State, State)> travelTime;
    static std::function<int(State, State)> travelTime_Overestimated(const float factor);
    static std::function<int(State, State)> travelTime_Intersections(const Topography& topo);
    static std::function<int(State, State)> travelTime_WeightedHeights(const Topography& topo);

    // Added duration (seconds) per intersection
    static constexpr int INTERSECTIONS_ADDED_DURATION = 100;

    // In what increments (meters) intersections should be checked in
    static constexpr float INTERSECTION_CHECK_STEP_SIZE = 10.0f; 

    // Maximum time save for low heights
    static constexpr int WEIGHTED_HEIGHTS_MAX_TIME_SAVE = 500;
};

#include "model/heuristic.h"
#include "model/state.h"
#include "model/topography.h"
#include "model/performance_measure.h"
#include <iostream>

Heuristic::Heuristic(Topography& topo, const int heuristicId, const float overestimateFactor) :
    topo(topo), heuristicId(heuristicId), overestimateFactor(overestimateFactor) {}

// Get heuristic by id
std::function<int(State, State)> Heuristic::getFunction() const {
    switch (heuristicId) {
        case 0:
            return travelTime_Overestimated(0.0f);

        case 1:
            return travelTime;

        case 2:
            return travelTime_Overestimated(overestimateFactor);

        case 3:
            return travelTime_Intersections(topo);

        case 4:
            return travelTime_WeightedHeights(topo);
    }
}

std::function<int(State, State)> Heuristic::travelTime = [](State a, State b) {
    float distance = sqrtf(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)); // In meters
    float speed = PerformanceMeasure::MAX_TRAVEL_SPEED; // 6 km/h
    return (int)(((distance / 1000.0f) / speed) * 3600.0f); // Return travel time in seconds
};

std::function<int(State, State)> Heuristic::travelTime_Overestimated(const float factor) {
    return [factor](State a, State b) {
        return (int)(factor * Heuristic::travelTime(a, b));
    };
};

std::function<int(State, State)> Heuristic::travelTime_Intersections(const Topography& topo) {
    return [topo](State a, State b) {
        int h = Heuristic::travelTime(a, b);	// Standard heuristic duration
        int intersections = 0;			        // Number of intersections
        int t = INTERSECTIONS_ADDED_DURATION;	// Added seconds per intersection

        glm::vec3 start = glm::vec3(a.x, a.y, a.z);
        glm::vec3 goal = glm::vec3(b.x, b.y, b.z);
        glm::vec3 goalDir = glm::normalize(goal - start);

        // Check for intersection every INTERSECTION_CHECK_STEP_SIZE meters
        glm::vec3 stepSize = goalDir * INTERSECTION_CHECK_STEP_SIZE;

        float totalDistance = glm::distance(start, goal); 
        glm::vec3 pos = start; // Position to check for intersection

        bool isAboveGround = true;
        bool wasAboveGround = true;

        while (glm::distance(start, pos) < totalDistance) {
            pos += stepSize;
            isAboveGround = pos.y >= topo.getY(pos.x, pos.z); // Update isAboveGround
            if (!isAboveGround && wasAboveGround) {
                intersections++; 
            }
            wasAboveGround = isAboveGround; // Update wasAboveGround
        }

        return h + intersections * t;
    };
};

std::function<int(State, State)> Heuristic::travelTime_WeightedHeights(const Topography& topo) {
    return [topo](State a, State b) {
        float h = Heuristic::travelTime(a, b);	                // Standard heuristic duration
        float height = (float)a.y / (float)topo.getAmplitude();	// Relative height of evaluated state
        float t = WEIGHTED_HEIGHTS_MAX_TIME_SAVE;			    // Maximum time save for low heigths
        float timeSave = (1 - height) * t;                      // Actual time save
        return (int)(std::max(h - timeSave, 0.0f));
    };
};

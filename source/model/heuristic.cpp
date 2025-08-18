#include "model/heuristic.h"
#include "model/state.h"
#include "model/topography.h"

Heuristic::Heuristic(Topography& topo, const int heuristicId, const float overestimateFactor) :
    topo(topo), heuristicId(heuristicId), overestimateFactor(overestimateFactor) {}

// Get heuristic by id
std::function<float(State, State)> Heuristic::getFunction() const {
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

std::function<float(State, State)> Heuristic::travelTime = [](State a, State b) {
    float distance = sqrtf(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
    float speed = 6.0f;
    return ((distance / 1000.0f) / speed) * 3600.0f;
};

std::function<float(State, State)> Heuristic::travelTime_Overestimated(const float factor) {
    return [factor](State a, State b) {
        return factor * Heuristic::travelTime(a, b);
    };
};

std::function<float(State, State)> Heuristic::travelTime_Intersections(const Topography& topo) {
    return [topo](State a, State b) {
        float h = Heuristic::travelTime(a, b);	    // Standard heuristic
        int intersections = 0;			// Number of intersections
        int t = 50;						// Added duration per intersection

        glm::vec3 start = glm::vec3(a.x, a.y, a.z);
        glm::vec3 goal = glm::vec3(b.x, b.y, b.z);
        glm::vec3 goalDir = glm::normalize(start - goal);
        glm::vec3 stepSize = goalDir * 10.0f;
        float totalDistance = glm::distance(start, goal);

        glm::vec3 pos = start;
        bool aboveGround = true;

        while (glm::distance(start, pos) < totalDistance) {
            pos += stepSize;
            if (pos.y < topo.getY(pos.x, pos.z) && aboveGround) {
                intersections++;
                aboveGround = false;
            }
            else aboveGround = true;
        }

        return h + intersections * t;
    };
};

std::function<float(State, State)> Heuristic::travelTime_WeightedHeights(const Topography& topo) {
    return [topo](State a, State b) {
        float h = Heuristic::travelTime(a, b);	// Standard duration
        float height = a.y / topo.getMaxY();	// Relative height of evaluated state
        float t = 500.0f;					    // Maximum time save
        float timeSave = (1 - height) * t;      // Actual time save
        return std::max(h - timeSave, 0.0f);
    };
};

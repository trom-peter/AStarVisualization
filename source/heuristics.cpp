#include "heuristics.h"

std::function<float(State, State)> Heuristics::travelTime = [](State a, State b) {
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

std::function<float(State, State)> Heuristics::travelTime_Overestimated(float weight) {
    return [weight](State a, State b) {
        return weight * Heuristics::travelTime(a, b);
    };
};

std::function<float(State, State)> Heuristics::travelTime_Intersections(Topography* topo) {
    return [topo](State a, State b) {
        float h = Heuristics::travelTime(a, b);	    // standard duration
        int intersections = 0;			// number of intersections
        int t = 10;						// added duration per intersection

        glm::vec3 start = glm::vec3(a.x, a.y, a.z);
        glm::vec3 goal = glm::vec3(b.x, b.y, b.z);
        glm::vec3 goalDir = glm::normalize(start - goal);
        glm::vec3 stepSize = goalDir * 10.0f;
        float totalDistance = glm::distance(start, goal);

        glm::vec3 pos = start;
        bool aboveGround = true;

        while (glm::distance(start, pos) < totalDistance) {
            pos += stepSize;
            if (pos.y < topo->getY(pos.x, pos.z) && aboveGround) {
                intersections++;
                aboveGround = false;
            }
            else aboveGround = true;
        }

        return h + intersections * t;
    };
};

std::function<float(State, State)> Heuristics::travelTime_WeightedHeights(Topography* topo) {
    return [topo](State a, State b) {
        float h = Heuristics::travelTime(a, b);	// standard duration
        float height = a.y / topo->getMaxY();	// relative height of evaluated state
        float t = 50.0f;					    // maximum time save
        return h - std::max((1 - height) * t, 0.0f);
    };
};

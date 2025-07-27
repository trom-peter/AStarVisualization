#pragma once
#include <functional>
#include "a_star/state.h"
#include "topography.h"

class Topography;

struct Heuristics {

    static std::function<float(State, State)> travelTime;

    static std::function<float(State, State)> travelTime_Overestimated(float weight);

    static std::function<float(State, State)> travelTime_Intersections(Topography* topo);

    static std::function<float(State, State)> travelTime_WeightedHeights(Topography* topo);

    static const char** getNames();

    static std::function<float(State, State)> getByID(int id, int weight = NULL, Topography* topo = nullptr);
};

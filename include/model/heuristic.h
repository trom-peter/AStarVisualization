#pragma once
#include <functional>
#include "model/topography.h"

struct State;
class Topography;

struct Heuristic {
    Heuristic(Topography* topo, const int heuristicId = 0, const float overestimateFactor = 1.0f);

    std::function<float(State, State)> getFunction() const;
    int heuristicId;
    Topography* topo;
    float overestimateFactor; //what factor travelTime_Overestimated should be overestimated by. 

private:
    static std::function<float(State, State)> travelTime;
    static std::function<float(State, State)> travelTime_Overestimated(const float factor);
    static std::function<float(State, State)> travelTime_Intersections(const Topography* topo);
    static std::function<float(State, State)> travelTime_WeightedHeights(const Topography* topo);

};

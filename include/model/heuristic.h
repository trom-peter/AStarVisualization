#pragma once
#include <functional>
#include "model/topography.h"

struct State;
class Topography;

struct Heuristic {
    Heuristic(Topography* topo, int heuristicId = 0, float overestimateFactor = 1.0f);

    std::function<float(State, State)> getFunction();
    int heuristicId;
    Topography* topo;
    float overestimateFactor; //what factor travelTime_Overestimated should be overestimated by. 

private:
    static std::function<float(State, State)> travelTime;
    static std::function<float(State, State)> travelTime_Overestimated(float factor);
    static std::function<float(State, State)> travelTime_Intersections(Topography* topo);
    static std::function<float(State, State)> travelTime_WeightedHeights(Topography* topo);

};

#include "model/state_grid.h"
#include "model/state.h"
#include "model/graph.h"
#include "model/a_star_search.h"
#include "model/search_problem.h"
#include <unordered_map>
#include <iostream>

Stategrid::Stategrid() :
    gridResolution(0),
    defaultColor(glm::vec3(0.0f)), frontierColor(glm::vec3(0.0f)), reachedColor(glm::vec3(0.0f)),
    initialStateColor(glm::vec3(0.0f)), goalStateColor(glm::vec3(0.0f)),
    solutionStateColor(glm::vec3(0.0f)),
    defaultVisible(false),
    frontierVisible(false),
    reachedVisible(false) {}

Stategrid::Stategrid(
    const int gridSize, const glm::vec3 defaultColor, const glm::vec3 frontierColor, const glm::vec3 reachedColor,
    const glm::vec3 initialStateColor, const glm::vec3 goalStateColor, const glm::vec3 solutionStateColor,
    const bool defaultVisible, const bool frontierVisible, const bool reachedVisible) :
    gridResolution(gridSize),
    defaultColor(defaultColor), frontierColor(frontierColor), reachedColor(reachedColor),
    initialStateColor(initialStateColor), goalStateColor(goalStateColor),
    solutionStateColor(solutionStateColor),
    defaultVisible(defaultVisible),
    frontierVisible(frontierVisible),
    reachedVisible(reachedVisible) {}

Stategrid::~Stategrid() {
    clearGrid();
}

void Stategrid::clearGrid() {
    grid.clear();
}

void Stategrid::updateToStep(const int step, const Graph& g, AStarSearch& aStar, const bool forwards) {
    State expanded = aStar.getAllExpanded().at(step);

    std::vector<State> frontier = aStar.getAllFrontiers().at(step); // Get frontier at step
    
    // Forwards step
    if (forwards) { 
        grid[expanded] = reachedColor;
        for (State s : frontier) {
            grid[s] = frontierColor;
        }
    }
    // Backwards step
    else { 
        grid[expanded] = frontierColor;
        for (State s : frontier) {
            // Check if any neighbour of s is a frontier node
            bool anyNeighbourFrontier = false;
            for (State& n : g.getNeighbours(s.x, s.z)) {
                if (grid[n] == reachedColor) {
                    anyNeighbourFrontier = true;
                    break;
                }
            }

            // Only change s to default color if no neighbouring spheres are frontiers
            if (!anyNeighbourFrontier) grid[s] = defaultColor;
        }
    }

    // Always show initial and goal state
    grid[aStar.getProblem().initial] = initialStateColor;

    grid[aStar.getProblem().goal] = goalStateColor;
}

void Stategrid::initGrid(const Topography& topo) {
    grid.clear();
    int stepSize = topo.getSize() / (gridResolution - 1);
    for (int z = 0; z <= topo.getSize(); z += stepSize) {
        for (int x = 0; x <= topo.getSize(); x += stepSize) {
            int y = topo.getY(x, z);
            State s(x, y, z);
            grid[s] = defaultColor;
        }
    }
}

void Stategrid::updateVisibility(const bool defaultVisible, 
    const bool frontierVisible, const bool reachedVisible) 
{
    this->defaultVisible = defaultVisible;
    this->frontierVisible = frontierVisible;
    this->reachedVisible = reachedVisible;
}

bool Stategrid::isVisible(const glm::vec3 color) const {
    return (
        color == frontierColor && frontierVisible ||
        color == reachedColor && reachedVisible ||
        color == defaultColor && defaultVisible ||
        color == initialStateColor || 
        color == goalStateColor ||
        color == solutionStateColor);
}

void Stategrid::showSolutionPath(const std::vector<State> solutionPath, const SearchProblem& problem) {
    for (State s : solutionPath) {
        if (s != problem.initial && s != problem.goal) {
            grid[s] = solutionStateColor;
        }
    }
}

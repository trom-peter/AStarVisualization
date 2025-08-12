#include "model/state_grid.h"
#include <unordered_map>
#include "model/state.h"
#include "model/graph.h"
#include "model/a_star_search.h"
#include "configurations/stategrid_configuration.h"
#include "model/search_problem.h"
#include <iostream>

Stategrid::Stategrid() :
    gridSize(0),
    defaultColor(glm::vec3(0.0f)), frontierColor(glm::vec3(0.0f)), reachedColor(glm::vec3(0.0f)),
    initialStateColor(glm::vec3(0.0f)), goalStateColor(glm::vec3(0.0f)),
    solutionStateColor(glm::vec3(0.0f)),
    defaultVisible(false),
    frontierVisible(false),
    reachedVisible(false) {}

Stategrid::Stategrid(
    int gridSize, glm::vec3 defaultColor, glm::vec3 frontierColor, glm::vec3 reachedColor,
    glm::vec3 initialStateColor, glm::vec3 goalStateColor, glm::vec3 solutionStateColor,
    bool defaultVisible, bool frontierVisible, bool reachedVisible) :
    gridSize(gridSize),
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

void Stategrid::updateToStep(int step, Graph& g, AStarSearch& aStar, bool forwards) {
    State expanded = aStar.allExpanded.at(step);

    if (forwards) {
        grid[expanded] = reachedColor;
        for (State s : aStar.allFrontiers.at(step)) {
            grid[s] = frontierColor;
        }
    } 
    else {
        grid[expanded] = frontierColor;
        for (State& s : aStar.allFrontiers.at(step)) {
            //check if any neighbour of s is a frontier node
            bool anyNeighbourFrontier = false;
            for (State& n : g.getNeighbours(s.x, s.z)) {
                if (grid[n] == reachedColor) {
                    anyNeighbourFrontier = true;
                    break;
                }
            }

            //only change s to default color if no neighbouring spheres are frontiers
            if (!anyNeighbourFrontier) grid[s] = defaultColor;
        }
    }

    grid[aStar.getProblem().initial] = initialStateColor;

    grid[aStar.getProblem().goal] = goalStateColor;
}

void Stategrid::initGrid(Topography* topo) {
    grid.clear();
    int stepSize = topo->getSize() / (gridSize - 1);
    for (int z = 0; z <= topo->getSize(); z += stepSize) {
        for (int x = 0; x <= topo->getSize(); x += stepSize) {
            int y = topo->getY(x, z);
            State s(x, y, z);
            grid[s] = defaultColor;
        }
    }
}

void Stategrid::updateVisibility(StategridConfig& config) {
    this->defaultVisible = config.defaultVisible;
    this->frontierVisible = config.frontierVisible;
    this->reachedVisible = config.reachedVisible;
}

bool Stategrid::isVisible(glm::vec3 color) const {
    return (
        color == frontierColor && frontierVisible ||
        color == reachedColor && reachedVisible ||
        color == defaultColor && defaultVisible ||
        color == initialStateColor || 
        color == goalStateColor ||
        color == solutionStateColor);
}

void Stategrid::showSolutionPath(std::vector<State> solutionPath, SearchProblem& problem) {
    for (State s : solutionPath) {
        if (s != problem.initial && s != problem.goal) {
            grid[s] = solutionStateColor;
        }
    }
}

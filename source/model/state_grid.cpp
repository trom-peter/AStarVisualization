#include "model/state_grid.h"
#include <unordered_map>
#include "sphere.h"
#include "model/state.h"
#include "model/graph.h"
#include "model/a_star_search.h"
#include "configurations/stategrid_configuration.h"
#include "model/search_problem.h"

Stategrid::Stategrid(int gridSize, StategridConfig& config) :
    gridSize(gridSize),
    defaultColor(config.defaultColor), frontierColor(config.frontierColor), reachedColor(config.reachedColor),
    initialStateColor(config.initialStateColor), goalStateColor(config.goalStateColor),
    solutionStateColor(config.solutionStateColor),
    defaultVisible(config.defaultVisible), 
    frontierVisible(config.frontierVisible), 
    reachedVisible(config.reachedVisible) {}

Stategrid::~Stategrid() {
    clearGrid();
}

void Stategrid::clearGrid() {
    for (auto& pair : grid) {
        delete pair.second;
    }
    grid.clear();
}

void Stategrid::updateToStep(int step, Graph& g, AStarSearch& aStar, bool forwards) {
    State expanded = aStar.allExpanded.at(step);

    if (forwards) {
        grid[expanded]->color = reachedColor;
        for (State s : aStar.allFrontiers.at(step)) {
            grid[s]->color = frontierColor;
        }
    } 
    else {
        grid[expanded]->color = frontierColor;
        for (State& s : aStar.allFrontiers.at(step)) {
            //check if any neighbour of s is a frontier node
            bool anyNeighbourFrontier = false;
            for (State& n : g.getNeighbours(s.x, s.z)) {
                if (grid[n]->color == reachedColor) {
                    anyNeighbourFrontier = true;
                    break;
                }
            }

            //only change s to default color if no neighbouring spheres are frontiers
            if (!anyNeighbourFrontier) grid[s]->color = defaultColor;
        }
    }

    grid[aStar.getProblem().initial]->color = initialStateColor;

    grid[aStar.getProblem().goal]->color = goalStateColor;
}

void Stategrid::initGrid(Topography* topo) {
    grid.clear();
    int stepSize = topo->getSize() / (gridSize - 1);
    for (int z = 0; z <= topo->getSize(); z += stepSize) {
        for (int x = 0; x <= topo->getSize(); x += stepSize) {
            float y = topo->getY(x, z);
            State s(x, y, z);
            grid[s] = new Sphere(1000.0f / gridSize, 10, 5, glm::vec3(x, y + 40.0f, z), glm::vec3(1.0f), defaultColor);
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
            grid[s]->color = solutionStateColor;
        }
    }
}

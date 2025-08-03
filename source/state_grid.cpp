#include "state_grid.h"

StateGrid::StateGrid(int gridSize) : gridSize(gridSize) {}

void StateGrid::updateToStep(int step, Graph& g, AStarSearch& aStar, bool forwards) {
    State expanded = aStar.allExpanded.at(step);

    if (forwards) {
        grid[expanded]->color = reachedColor;
        for (State s : aStar.allFrontiers.at(step)) {
            grid[s]->color = frontierColor;
        }
    } 
    else {
        grid[expanded]->color = frontierColor;
        for (State s : aStar.allFrontiers.at(step)) {
            //check if any neighbour of s is a frontier node
            bool anyNeighbourFrontier = false;
            for (State n : g.getNeighbours(s.x, s.z)) {
                if (grid[n]->color == reachedColor) {
                    anyNeighbourFrontier = true;
                    break;
                }
            }

            //only change s to default color if no neighbouring spheres are frontiers
            if (!anyNeighbourFrontier) grid[s]->color = defaultColor;
        }
    }

    grid[aStar.getProblem().initial]->color = glm::vec3(0.2f, 1.0f, 0.2f);

    grid[aStar.getProblem().goal]->color = glm::vec3(1.0f, 0.2f, 0.2f);
}

void StateGrid::initGrid(Topography* topo) {
    for (int z = 0; z <= topo->getSize(); z += topo->getSize() / (gridSize - 1)) {
        for (int x = 0; x <= topo->getSize(); x += topo->getSize() / (gridSize - 1)) {
            float y = topo->getY(x, z);
            State s(x, y, z);
            grid[s] = new Sphere(1000.0f / gridSize, 10, 5, glm::vec3(x, y + 40.0f, z), glm::vec3(1.0f), defaultColor);
        }
    }
}

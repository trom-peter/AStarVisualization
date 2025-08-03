#include <iostream>
#include "visualization.h";

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

int main() {
    Visualization vis;

    if (!vis.init()) {
        std::cout << "ERROR: Problem initializing program" << std::endl;
        return -1;
    }

    vis.run();

    return vis.quit();
}
/*
void updateSphereColors(std::unordered_map<State, Sphere*>& grid, int step, SearchConfiguration& config, Graph& g, Problem& p, AStarSearch& aStar, bool forwards) {
    State expanded = aStar.allExpanded.at(step);

    if (forwards) {
        grid[expanded]->color = config.reachedColor;
    }
    else {
        grid[expanded]->color = config.frontierColor;
    }

    if (forwards) {
        for (State s : aStar.allFrontiers.at(step)) {
            grid[s]->color = config.frontierColor;
        }
    }
    else {
        for (State s : aStar.allFrontiers.at(step)) {
            //check if any neighbour of s is a frontier node
            bool anyNeighbourFrontier = false;
            for (State n : g.getNeighbours(s.x, s.z)) {
                if (grid[n]->color == config.reachedColor) {
                    anyNeighbourFrontier = true;
                    break;
                }
            }

            //only change s to default color if no neighbouring spheres are frontiers
            if (!anyNeighbourFrontier) grid[s]->color = config.defaultColor;
        }
    }

    grid[p.initial]->color = glm::vec3(0.2f, 1.0f, 0.2f);

    grid[p.goal]->color = glm::vec3(1.0f, 0.2f, 0.2f);
}

void updateGrid(std::unordered_map<State, Sphere*>& grid, Topography& topo, int length, int width, int gridSize, glm::vec3 color) {
    for (int z = 0; z <= length; z += length / (gridSize - 1)) {
        for (int x = 0; x <= width; x += width / (gridSize - 1)) {
            float y = topo.getY(x, z);
            State s(x, y, z);
            grid[s] = new Sphere(1000.0f / gridSize, 10, 5, glm::vec3(x, y + 40.0f, z), glm::vec3(1.0f), color);
        }
    }
}

int main() {
	Window window(1400, 1000, "A* Visualisierung");
    InputHandler inputHandler;
    GUI gui;
   
    if (!window.init()) {
        return -1;
    }

    if (!BaseRenderer::init()) {
        window.quit();
        return -1;
    }

    float amplitude = 500.0f;

    TopographyRenderer topoRenderer;
    topoRenderer.setAmplitude(500.0f);
    ShapeRenderer shapeRenderer;
    SearchConfiguration config;

    gui.init(window);

    Framebuffer fb(1400, 1000);

    VertexArray* vao = new VertexArray();
    vao->setupVertexLayout();

    topoRenderer.setClearColor(1.3f, 1.3f, 1.3f);

    float width = 7000.0f;
    float length = width;
    float scale = 0.0002f; // je größer, desto größeres gebiet
    Topography* topo = new Topography(0, config.terrainScaling, 1);

    topo->generate();

    topoRenderer.setupUniforms();
    shapeRenderer.setupUniforms();

    int gridSize = config.gridSize;
    std::unordered_map<State, Sphere*> grid;
    updateGrid(grid, *topo, length, width, gridSize, config.defaultColor);

    State initial(0, topo->getY(0, 0), 0);
    State goal(0, topo->getY(0, 0), 0);

    Graph g(gridSize, width);
    g.setTopography(topo);

    SearchProblem p(g);
    AStarSearch aStar(p);

    Node* solution = nullptr;

    Camera* camera = new Camera(glm::radians(90.0f), window.getWidth(), window.getHeight(), 10.0f, 20000.0f);
    camera->translate(glm::vec3(width / 2, 10000.0f, length / 2));
    camera->rotate(glm::vec2(-90.0f, 90.0f));
    camera->update();

    gui.setSearchConfig(&config);

    bool running = true;
    int step = 0;
    float searchTime = 0.0f;
    int searchRate = 1;
    float timePerIncrement = 1.0f / searchRate;

    VisualizationState state = VisualizationState::ConfiguringSearchEnvironment;

    while (running) {
        gui.startFrame();

        inputHandler.handleEvents(running, gui);

        fb.bind();
        //TODO
        // resize des framebuffers fixen

        topoRenderer.clear();
        topoRenderer.updateUniforms(camera);
        topoRenderer.draw(topo);

        for (auto& kv : grid) {
            glm::vec3 color = kv.second->color;

            if (color == config.frontierColor && !config.frontierVisible ||
                color == config.defaultColor && !config.unexploredVisible ||
                color == config.reachedColor && !config.reachedVisible)
                continue;

            if (shapeRenderer.getColor() != color) shapeRenderer.setColor(color);
            shapeRenderer.updateUniforms(camera, kv.second->model);
            shapeRenderer.draw(kv.second);
        }

        fb.unbind();

        switch (state) {
            case VisualizationState::ConfiguringSearchEnvironment:
                state = gui.showUI_EnvironmentConfig();

                 if (config.topographyType != topo->getType() || 
                     config.seed != topo->getSeed() || 
                     config.terrainScaling != topo->getScale()) 
                 {
                     topo->setType(config.topographyType);
                     topo->setSeed(config.seed);
                     topo->setScale(config.terrainScaling);
                     topo->generate();
                     grid.clear();
                     gridSize = config.gridSize;
                     updateGrid(grid, *topo, length, width, gridSize, config.defaultColor);
                 }

                 if (config.gridSize != gridSize) {
                     gridSize = config.gridSize;
                     grid.clear();
                     updateGrid(grid, *topo, length, width, gridSize, config.defaultColor);
                 }
                if (state != VisualizationState::ConfiguringSearchEnvironment) {
                    g = Graph(config.gridSize, width);
                    g.setTopography(topo);
                    g.init();
                    initial = State(0, topo->getY(0, 0), 0);
                    goal = State(0, topo->getY(0, 0), 0);
                    p.g = g;
                    p.initial = initial;
                    p.goal = goal;
                    config.unexploredVisible = false; //unexplored nodes are invisble when configuring search problem
                }
                break;

            case VisualizationState::ConfiguringSearchProblem:
                state = gui.showUI_SearchProblemConfig(g.getSpacing());
                if (config.initial != initial.getXZ()) { //initial state updated
                    grid[initial]->color = config.defaultColor;
                    initial.y = topo->getY(config.initial.x, config.initial.y);
                    initial.setXZ(config.initial);
                }
                if (config.goal != goal.getXZ()) { //goal state updated
                    grid[goal]->color = config.defaultColor;
                    goal.y = topo->getY(config.goal.x, config.goal.y);
                    goal.setXZ(config.goal);
                }

                grid[initial]->color = config.initialStateColor;
                grid[goal]->color = config.goalStateColor;

                switch (config.heuristic) {
                    case 0:
                        aStar.setHeuristic(Heuristics::travelTime_Overestimated(0.0f));
                        break;
                    case 1:
                        aStar.setHeuristic(Heuristics::travelTime);
                        break;
                    case 2:
                        aStar.setHeuristic(Heuristics::travelTime_Overestimated(config.overestimateFactor));
                        break;
                    case 3:
                        aStar.setHeuristic(Heuristics::travelTime_Intersections(topo));
                        break;
                    case 4:
                        aStar.setHeuristic(Heuristics::travelTime_WeightedHeights(topo));
                        break;
                }

                if (state != VisualizationState::ConfiguringSearchProblem) {
                    p.initial = initial;
                    p.goal = goal;
                    solution = aStar.search();
                    if (solution == nullptr) {
                        std::cout << "No path found." << std::endl;
                        gui.quit();
                        window.quit();
                        return 0;
                    }
                    config.step = 0;
                    config.maxSteps = aStar.allExpanded.size() - 1;
                }
                break;

            case VisualizationState::Searching:
                state = gui.showUI_Searching();

                if (config.searchRate != searchRate) {
                    searchRate = config.searchRate;
                    timePerIncrement = 1.0f / searchRate;
                }

                if (config.searchPlaying) {
                    searchTime += window.getDelta();
                    if (searchTime >= timePerIncrement) {
                        config.step++;
                        searchTime -= timePerIncrement;
                    }
                }

                if (config.step > step) { //forwards step
                    updateSphereColors(grid, config.step - 1, config, g, p, aStar, true);
                }
                else if (config.step < step) { // backwards step
                    updateSphereColors(grid, config.step, config, g, p, aStar, false);
                }
                step = config.step;

                break;

            case VisualizationState::Finished:
                state = gui.showUI_Finished(
                    aStar.solutionPath.size(),
                    aStar.allExpanded.size(),
                    aStar.consideredNodes,
                    solution->pathCost);

                for (State s : aStar.solutionPath) {
                    if (s != initial && s != goal) {
                        grid[s]->color = glm::vec3(1.0f, 1.0f, 0.0f);
                    }
                }

                if (state == VisualizationState::ConfiguringSearchProblem) {
                    grid.clear();
                    updateGrid(grid, *topo, length, width, gridSize, config.defaultColor);
                }

                if (state == VisualizationState::ConfiguringSearchEnvironment) {
                    grid.clear();
                    updateGrid(grid, *topo, length, width, gridSize, config.defaultColor);
                    config.reset();
                }

                break;
        }

        gui.showUI_Visibility();
        gui.showUI_Viewport(fb);

        gui.render();

        window.swapBuffers();
        window.updateTime();
    }

    gui.quit();
    window.quit();
    return 0;
}
*/
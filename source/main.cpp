#include <iostream>
#include <vector>
#include <array>
#include <math.h>

#include "stb_perlin.h"
#include "framebuffer.h"
#include "window.h"
#include "base_renderer.h"
#include "topography_renderer.h"
#include "shape_renderer.h"
#include "vertex_array.h"
#include "mesh.h"
#include "fps_camera.h"
#include "fps_camera_controller.h"
#include "shader.h"
#include "input_handler.h"
#include "vertex.h"
#include "topography.h"
#include "gui.h"
#include "sphere.h"
#include "a_star/problem.h"
#include "a_star/state.h"
#include "a_star/a_star_search.h"
#include "search_configuration.h"
#include "visualization_state.h"

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

void updateSphereColors(std::unordered_map<State, Sphere*>& spheres, int step, SearchConfiguration& config, Graph& g, Problem& p, AStarSearch& aStar, bool forwards) {
    State expanded = aStar.allExpanded.at(step);

    if (forwards) {
        spheres[expanded]->color = config.reachedColor;
    }
    else {
        spheres[expanded]->color = config.frontierColor;
    }

    if (forwards) {
        for (State s : aStar.allFrontiers.at(step)) {
            spheres[s]->color = config.frontierColor;
        }
    }
    else {
        for (State s : aStar.allFrontiers.at(step)) {
            //check if any neighbour of s is a frontier node
            bool anyNeighbourFrontier = false;
            for (State n : g.getNeighbours(s.x, s.z)) {
                if (spheres[n]->color == config.reachedColor) {
                    anyNeighbourFrontier = true;
                    break;
                }
            }

            //only change s to default color if no neighbouring spheres are frontiers
            if (!anyNeighbourFrontier) spheres[s]->color = config.defaultColor;
        }
    }

    spheres[p.initial]->color = glm::vec3(0.2f, 1.0f, 0.2f);

    spheres[p.goal]->color = glm::vec3(1.0f, 0.2f, 0.2f);
}

void updateGrid(std::unordered_map<State, Sphere*>& spheres, Topography& topo, int length, int width, int gridSize, glm::vec3 color) {
    for (int z = 0; z <= length; z += length / (gridSize - 1)) {
        for (int x = 0; x <= width; x += width / (gridSize - 1)) {
            float y = topo.getY(x, z);
            State s(x, y, z);
            spheres[s] = new Sphere(1000.0f / gridSize, 10, 5, glm::vec3(x, y + 40.0f, z), glm::vec3(1.0f), color);
        }
    }
}

int main() {
	Window window(1400, 1000, "Topo2D");
    InputHandler inputHandler;
    GUI gui;
   
    if (!window.init()) {
        return -1;
    }

    if (!BaseRenderer::init()) {
        window.quit();
        return -1;
    }

    float amplitude = 1500.0f;

    TopographyRenderer topoRenderer(amplitude);
    ShapeRenderer shapeRenderer;
    SearchConfiguration config;

    gui.init(window);

    Framebuffer fb(1400, 1000);

    VertexArray* vao = new VertexArray();
    vao->setupVertexLayout();

    topoRenderer.setClearColor(0.5f, 0.3f, 0.3f);

    float width = 7000.0f;
    float length = width;
    float scale = 0.0002; // je größer, desto größeres gebiet
    Topography* topo = new Topography(width, length, amplitude, scale, 20.0f);

    topo->generate();
    Mesh* mesh = topo->getMesh();

    topoRenderer.setupUniforms();
    shapeRenderer.setupUniforms();

    float minTopoY = topo->getMinY();
    float maxTopoY = topo->getMaxY();

    int gridSize = config.gridSize;
    std::unordered_map<State, Sphere*> spheres;
    updateGrid(spheres, *topo, length, width, gridSize, config.defaultColor);

    State initial(0, topo->getY(0, 0), 0);
    State goal(0, topo->getY(0, 0), 0);

    Graph g(gridSize, width);
    g.setTopography(topo);

    Problem p(g);
    AStarSearch aStar;
    Node* solution;

    FPSCamera* camera = new FPSCamera(glm::radians(90.0f), window.getWidth(), window.getHeight(), 1500.0f, 0.1f, 10.0f, 20000.0f);
    FPSCameraController* controller = new FPSCameraController(*camera);
    camera->translate(glm::vec3(width / 2, 10000.0f, length / 2));
    camera->rotate(glm::vec2(0.0f, 90.0f));
    camera->update();

    config.gridSize = gridSize;
    config.stateSpacing = g.getSpacing();

    gui.setSearchConfig(&config);

    bool running = true;
    bool stateChanged = false;
    int step = 0;
    float searchTime = 0.0f;
    int searchRate = 1;
    float timePerIncrement = 1.0f / searchRate;

    VisualizationState state = VisualizationState::ConfiguringSearchEnvironment;

    while (running) {
        //std::cout << "x:" << camera->getPosition().x << std::endl;
        //std::cout << "y:" << camera->getPosition().y << std::endl;
        //std::cout << "z:" << camera->getPosition().z << std::endl << std::endl;
        gui.startFrame();

        inputHandler.handleEvents(running, gui);
        window.updateState(inputHandler, gui);

        if (window.isActive()) {
            controller->handleInputs(&inputHandler, window.getDelta());
            camera->update();
        }

        switch (state) {
            case VisualizationState::ConfiguringSearchEnvironment:
                stateChanged = gui.showUI_EnvironmentConfig(); //true if state was changed

                if (config.seed != topo->getSeed()) {
                    topo->setSeed(config.seed);
                    topo->generate();
                    spheres.clear();
                    gridSize = config.gridSize;
                    updateGrid(spheres, *topo, length, width, gridSize, config.defaultColor);
                }
                else if (config.gridSize != gridSize) {
                    spheres.clear();
                    gridSize = config.gridSize;
                    updateGrid(spheres, *topo, length, width, gridSize, config.defaultColor);
                }

                if (stateChanged) {
                    state = VisualizationState::ConfiguringSearchProblem;
                    g = Graph(config.gridSize, width);
                    g.init();
                    g.setTopography(topo);
                    initial = State(0, topo->getY(0, 0), 0);
                    goal = State(0, topo->getY(0, 0), 0);
                    p.g = g;
                    p.initial = initial;
                    p.goal = goal;
                    config.stateSpacing = g.getSpacing();
                }
                break;

            case VisualizationState::ConfiguringSearchProblem:
                stateChanged = gui.showUI_SearchProblemConfig();
                if (config.initial != initial.getXZ()) { //initial state updated
                    spheres[initial]->color = config.defaultColor;
                    initial.y = topo->getY(config.initial.x, config.initial.y);
                    initial.setXZ(config.initial);
                }
                if (config.goal != goal.getXZ()) { //goal state updated
                    spheres[goal]->color = config.defaultColor;
                    goal.y = topo->getY(config.goal.x, config.goal.y);
                    goal.setXZ(config.goal);
                }

                spheres[initial]->color = config.initialStateColor;
                spheres[goal]->color = config.goalStateColor;

                if (stateChanged) {
                    state = VisualizationState::Searching;
                    p.initial = initial;
                    p.goal = goal;
                    solution = aStar.search(p);
                    if (solution == nullptr) {
                        std::cout << "No path found." << std::endl;
                        break;
                    }
                    else {
                        std::cout << solution->getPath() << std::endl;
                        std::cout << "Path found! Cost: " << solution->pathCost << " seconds" << std::endl;
                    }
                    config.maxSteps = aStar.allExpanded.size() - 1;
                    config.unexploredVisible = false; //unexplored nodes are invisble when in searching mode
                }
                break;

            case VisualizationState::Searching:
                stateChanged = gui.showUI_Searching();

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
                    updateSphereColors(spheres, config.step - 1, config, g, p, aStar, true);
                }
                else if (config.step < step) { // backwards step
                    updateSphereColors(spheres, config.step, config, g, p, aStar, false);
                }
                step = config.step;

                if (stateChanged) state = VisualizationState::Finished;
                break;

            case VisualizationState::Finished:
                stateChanged = gui.showUI_Finished();
                for (State s : aStar.solutionPath) {
                    if (s != initial && s != goal) {
                        spheres[s]->color = glm::vec3(1.0f, 1.0f, 0.0f);
                    }
                }
                break;
        }

        gui.showUI_Visibility();
        gui.showUI_Viewport(fb);

        gui.render();

        //TODO
        // resize des framebuffers fixen
        fb.bind();

        topoRenderer.clear();
        topoRenderer.updateUniforms(camera);
        topoRenderer.draw(topo);

        for (auto& kv : spheres) {
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

        window.swapBuffers();
        window.updateTime();
    }

    gui.quit();
    window.quit();
    return 0;
}

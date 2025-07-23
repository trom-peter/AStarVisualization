#include <iostream>
#include <vector>
#include <array>

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

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

void updateSphereColors(std::unordered_map<State, Sphere*>& spheres, int step, GUI& gui, Problem& p, AStarSearch& aStar, bool forwards) {

    if (forwards) {
        for (State s : aStar.allFrontiers.at(step)) {
            spheres[s]->color = gui.getFrontierColor();
        }
    }
    else {
        for (State s : aStar.allFrontiers.at(step)) {
            spheres[s]->color = gui.getDefaultColor();
        }
    }

    State expanded = aStar.allExpanded.at(step);

    if (forwards) {
        spheres[expanded]->color = gui.getReachedColor();
    }
    else {
        spheres[expanded]->color = gui.getFrontierColor();
    }

    spheres[p.initial]->color = glm::vec3(0.2f, 1.0f, 0.2f);

    spheres[p.goal]->color = glm::vec3(1.0f, 0.2f, 0.2f);
}

void startSearch() {

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

    float amplitude = 500.0f;

    TopographyRenderer topoRenderer(amplitude);
    ShapeRenderer shapeRenderer;

    gui.init(window);

    Framebuffer fb(1400, 1000);

    VertexArray* vao = new VertexArray();
    vao->setupVertexLayout();

    topoRenderer.setClearColor(0.5f, 0.3f, 0.3f);

    float width = 7000.0f;
    float length = width;
    float scale = 0.00015; // je größer, desto größeres gebiet
    Topography* topo = new Topography(width, length, amplitude, scale, 20.0f);

    topo->generate();
    Mesh* mesh = topo->getMesh();

    topoRenderer.setupUniforms();
    shapeRenderer.setupUniforms();

    float minTopoY = topo->getMinY();
    float maxTopoY = topo->getMaxY();

    int gridSize = 15;
    std::unordered_map<State, Sphere*> spheres;
    //std::vector<Sphere*> spheres;
    for (int z = 0; z <= length; z += length / (gridSize - 1)) {
        for (int x = 0; x <= width; x += width / (gridSize - 1)) {
            float y = topo->getY(x, z);
            //float y = stb_perlin_ridge_noise3_seed(x * scale, z * scale, 0.0f, 2.0f, 0.5f, 1.0f, 3, 1) * amplitude;
            State s(x, y, z);
            spheres[s] = new Sphere(1000.0f / gridSize, 10, 5, glm::vec3(x, y + 40.0f, z));
        }
    }

    glm::vec3 initialStateColor(0.2f, 1.0f, 0.2f);
    glm::vec3 goalStateColor(1.0f, 0.2f, 0.2f);

    State initial(0, topo->getY(0, 0), 0);
    State goal(0, topo->getY(0, 0), 0);

    Graph g(gridSize, width);
    g.setTopography(topo);

    Problem p(g);
    AStarSearch aStar;
    Node* solution;

    FPSCamera* camera = new FPSCamera(glm::radians(90.0f), window.getWidth(), window.getHeight(), 1500.0f, 0.1f, 5.0f, 10000.0f);
    FPSCameraController* controller = new FPSCameraController(*camera);
    camera->translate(glm::vec3(width / 2, 10000.0f, length / 2));
    camera->update();

    bool configuring = true; //true when visualization is in configuring mode
    bool searching = false; //true when visualization is in searching mode

    int step = 0; // step number in search process. measured in number of expanded nodes
    int maxSteps = 0;

    gui.setStateSpacing(g.getSpacing());
    for (auto& kv : spheres) {
        kv.second->color = gui.getDefaultColor();
    }

    bool running = true;
    while (running) {
        //std::cout << gui.isViewportActive() << std::endl;
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

        gui.renderStuff(fb, configuring);

        //TODO
        // resize des framebuffers fixen
        fb.bind();

        if (configuring) {
            if (gui.getInitial() != initial.getXZ()) { //initial state updated
                spheres[initial]->color = gui.getDefaultColor();
                initial.y = topo->getY(gui.getInitial().x, gui.getInitial().y);
                initial.setXZ(gui.getInitial());
            }

            if (gui.getGoal() != goal.getXZ()) { //goal state updated
                spheres[goal]->color = gui.getDefaultColor();
                goal.y = topo->getY(gui.getGoal().x, gui.getGoal().y);
                goal.setXZ(gui.getGoal());
            }

            spheres[initial]->color = initialStateColor;
            spheres[goal]->color = goalStateColor;

        } else if (!searching) { //user wants to start search
            p.initial = initial;
            p.goal = goal;
            std::cout << "Starting search..." << std::endl;
            std::cout << "Initial State: " << initial.x << ", " << initial.y << ", " << initial.z << std::endl;
            std::cout << "Goal State: " << goal.x << ", " << goal.y << ", " << goal.z << std::endl;

            solution = aStar.search(p);
            if (solution == nullptr) {
                std::cout << "no path found" << std::endl;
                break;
            }
            else {
                std::cout << solution->getPath() << std::endl;
                std::cout << "cost: " << solution->pathCost << " seconds" << std::endl;
            }
            maxSteps = aStar.allExpanded.size() - 1;
            gui.setMaxSteps(maxSteps);
            searching = true;
        }

        if (searching) {
            if (gui.getStep() > step) {
                updateSphereColors(spheres, step++, gui, p, aStar, true);
            }
            else if (gui.getStep() < step) {
                updateSphereColors(spheres, --step, gui, p, aStar, false);
            }
        }

        if (searching && step == maxSteps) {
            for (State s : aStar.solutionPath) {
                if (s != initial && s != goal) {
                    spheres[s]->color = glm::vec3(1.0f, 1.0f, 0.0f);
                }
            }
        }

        topoRenderer.clear();
        topoRenderer.updateUniforms(camera);
        topoRenderer.draw(topo);

        for (auto& kv : spheres) {
            glm::vec3 color = kv.second->color;
            if (color.x == 0.0f && color.y == 0.0f && color.z == 0.0f) continue;
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

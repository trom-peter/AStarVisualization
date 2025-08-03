#pragma once
#include "window.h"
#include "input_handler.h"
#include "gui.h"
#include "base_renderer.h"
#include "topography_renderer.h"
#include "shape_renderer.h"
#include "search_configuration.h"
#include "state_grid.h"
#include "a_star/search_problem.h"
#include "search_environment.h"

class Visualization {
public:
    Visualization();
    bool init();
    void run();
    int quit();

private:
    //TODO State funktionen
    void inEnvironment();
    void environmentToProblem();
    void inProblem();
    int problemToSearching();
    void inSearching();
    void searchingToFinished();
    void inFinished();
    void finishedToEnvironment();
    void finishedToProblem();

    VisualizationState state;
    SearchEnvironment* environment;
    SearchProblem* problem;
    AStarSearch* aStar;
    Node* solution;
    GUI gui;
    Window window;
    InputHandler inputHandler;
    TopographyRenderer* topoRenderer;
    ShapeRenderer* shapeRenderer;
    SearchConfiguration config;
    Framebuffer* fb; // framebuffer where visualization (not gui) is drawn to
    VertexArray* vao;
    Camera camera;
    bool running;
    int step;
    float searchTime = 0.0f;
    int searchRate = 1;
    float timePerIncrement = 1.0f / searchRate;
};

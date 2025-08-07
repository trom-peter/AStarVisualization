#pragma once
#include "opengl/base_renderer.h"
#include "opengl/topography_renderer.h"
#include "opengl/shape_renderer.h"
#include "opengl/framebuffer.h"
#include "a_star/state_grid.h"
#include "a_star/search_problem.h"
#include "a_star/search_environment.h"
#include "visualization/gui.h"
#include "configurations/environment_configuration.h"
#include "configurations/playback_configuration.h"
#include "configurations/stategrid_configuration.h"
#include "configurations/problem_configuration.h"
#include "window.h"
#include "a_star/a_star_search.h"

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
    GUI gui;
    Window window;
    TopographyRenderer* topoRenderer;
    ShapeRenderer* shapeRenderer;
    Framebuffer* fb; // framebuffer where visualization (not gui) is drawn to
    VertexArray* vao;
    Camera* camera;
    EnvironmentConfig config_Environment;
    ProblemConfig config_Problem;
    PlaybackConfig config_Playback;
    StategridConfig config_Stategrid;
    int step;
    bool running;
};

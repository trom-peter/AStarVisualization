#pragma once
#include "infrastructure/opengl/base_renderer.h"
#include "infrastructure/opengl/topography_renderer.h"
#include "infrastructure/opengl/stategrid_renderer.h"
#include "infrastructure/opengl/framebuffer.h"
#include "model/state_grid.h"
#include "model/search_problem.h"
#include "model/search_environment.h"
#include "model/a_star_search.h"
#include "configurations/environment_configuration.h"
#include "configurations/playback_configuration.h"
#include "configurations/stategrid_configuration.h"
#include "configurations/problem_configuration.h"
#include "gui/gui.h"
#include "infrastructure/window/window.h"

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
    void finishedToEnvironment();
    void finishedToProblem();

    VisualizationState state;
    SearchEnvironment* environment;
    SearchProblem* problem;
    AStarSearch* aStar;
    GUI gui;
    Window window;
    TopographyRenderer* topoRenderer;
    StategridRenderer* stategridRenderer;
    FrameBuffer* fb; // framebuffer where visualization (not gui) is drawn to
    VertexArray* vao;
    Camera* camera;
    EnvironmentConfig config_Environment;
    ProblemConfig config_Problem;
    PlaybackConfig config_Playback;
    StategridConfig config_Stategrid;
    int step;
};

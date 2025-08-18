#pragma once
#include <memory>
#include "infrastructure/opengl/base_renderer.h"
#include "infrastructure/opengl/topography_renderer.h"
#include "infrastructure/opengl/stategrid_renderer.h"
#include "infrastructure/opengl/framebuffer.h"
#include "infrastructure/opengl/camera.h"
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
    Window window;
    GUI gui;
    std::unique_ptr<SearchEnvironment> environment;
    std::unique_ptr<SearchProblem> problem;
    std::unique_ptr<AStarSearch> aStar;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<FrameBuffer> fb; // framebuffer where visualization (not gui) is drawn to
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<TopographyRenderer> topoRenderer;
    std::unique_ptr<StategridRenderer> stategridRenderer;
    EnvironmentConfig config_Environment;
    ProblemConfig config_Problem;
    PlaybackConfig config_Playback;
    StategridConfig config_Stategrid;
    int step;
};

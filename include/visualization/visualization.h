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

    // State transitions
    void environmentToProblem();
    int problemToSearching();
    void searchingToFinished();
    void finishedToEnvironment();
    void finishedToProblem();

    // State executes
    void inEnvironment();
    void inProblem();
    void inSearching();

    VisualizationState state; // Current visualization state
    Window window;
    GUI gui;
    std::unique_ptr<SearchEnvironment> environment;
    std::unique_ptr<SearchProblem> problem;
    std::unique_ptr<AStarSearch> aStar;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<FrameBuffer> fb; // Framebuffer where visualization (not gui) is drawn to
    std::unique_ptr<TopographyRenderer> topoRenderer;
    std::unique_ptr<StategridRenderer> stategridRenderer;

    // Configurations
    EnvironmentConfig config_Environment;
    ProblemConfig config_Problem;
    PlaybackConfig config_Playback;
    StategridConfig config_Stategrid;

    int step; // Step number in search process. Measured in number of expanded nodes
};

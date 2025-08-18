#include "visualization/visualization.h"
#include "model/state.h"
#include "infrastructure/geometry/sphere.h"
#include "infrastructure/opengl/camera.h"

// rendering and camera configuration
glm::vec3 constexpr BACKGROUND_COLOR = glm::vec3(0.1f);
float constexpr CAMERA_FOV = 90.0f;
float constexpr CAMERA_NEAR_PLANE = 10.0f;
float constexpr CAMERA_FAR_PLANE = 15000.0f;
float constexpr CAMERA_HEIGHT = 10000.0f;
float constexpr CAMERA_X_ROT = -90.0f;
float constexpr CAMERA_Y_ROT = 90.0f;

Visualization::Visualization() : 
	window("A* Visualisierung"), state(VisualizationState::ConfiguringSearchEnvironment), step(0),
	environment(nullptr), aStar(nullptr), problem(nullptr), topoRenderer(nullptr), 
	stategridRenderer(nullptr), fb(nullptr), camera(nullptr), vao(nullptr) {}

bool Visualization::init() {
	if (!window.init()) {
		return false;
	}

	if (!BaseRenderer::init()) {
		return false;
	}

	BaseRenderer::setClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z);

	gui.init(window);

	environment = std::make_unique<SearchEnvironment>(
		config_Environment.seed, config_Environment.terrainScaling, 
		config_Environment.topographyType, config_Environment.topographySize, 
		config_Environment.topographyAmplitude, config_Environment.gridSize,
		Stategrid(config_Environment.gridSize, 
			config_Stategrid.defaultColor, config_Stategrid.frontierColor, config_Stategrid.reachedColor, 
			config_Stategrid.initialStateColor, config_Stategrid.goalStateColor, config_Stategrid.solutionStateColor, 
			config_Stategrid.defaultVisible, config_Stategrid.frontierVisible, config_Stategrid.reachedVisible));

	problem = std::make_unique<SearchProblem>(*environment, config_Problem.initial, config_Problem.goal);

	aStar = std::make_unique<AStarSearch>(
		*problem, 
		Heuristic(environment->topography, config_Problem.heuristic, config_Problem.overestimateFactor), 
		*environment);

	topoRenderer = std::make_unique<TopographyRenderer>(environment->topography);
	stategridRenderer = std::make_unique<StategridRenderer>(environment->stategrid);

	fb = std::make_unique<FrameBuffer>(window.getWidth() , window.getHeight());
	vao = std::make_unique<VertexArray>();
	camera = std::make_unique<Camera>(glm::radians(CAMERA_FOV), window.getWidth(),
		window.getHeight(), CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);

	camera->translate(glm::vec3(
		environment->topography.getSize() / 2,
		CAMERA_HEIGHT,
		environment->topography.getSize() / 2));

	camera->rotate(glm::vec2(CAMERA_X_ROT, CAMERA_Y_ROT));

	topoRenderer->setupUniforms(*camera);
	stategridRenderer->setupUniforms(*camera);

	step = 0;
	state = VisualizationState::ConfiguringSearchEnvironment;

	return true;
}

/*
	Main loop for running the visualization.
	Exits when state == VisualizationState::Quit.
*/
void Visualization::run() {
	while (true) {
		gui.startFrame();

		//check if window was exited
		if (gui.isWindowExited())
			state = VisualizationState::Quit;

		//draw gui and update visualization state
		switch (state) {
			case VisualizationState::ConfiguringSearchEnvironment:
				state = gui.showUI_EnvironmentConfig(config_Environment);
				inEnvironment();
				if (state == VisualizationState::ConfiguringSearchProblem) 
					environmentToProblem();
				break;

			case VisualizationState::ConfiguringSearchProblem:
				state = gui.showUI_SearchProblemConfig(config_Problem, config_Environment);
				inProblem();
				if (state == VisualizationState::Searching)
					if (problemToSearching() == -1) break;
				break;

			case VisualizationState::Searching:
				state = gui.showUI_Searching(config_Playback);
				inSearching();
				if (state == VisualizationState::Finished)
					searchingToFinished();
				break;

			case VisualizationState::Finished:
				state = gui.showUI_Finished(*aStar);
				if (state == VisualizationState::ConfiguringSearchEnvironment)
					finishedToEnvironment();
				else if (state == VisualizationState::ConfiguringSearchProblem)
					finishedToProblem();
				break;

			case VisualizationState::Quit:
				return;
		}

		fb->bind();
		glViewport(0, 0, (int)fb->width, (int)fb->height);
		camera->resizeProj(fb->width, fb->height);

		BaseRenderer::clear();

		// draw topography into FrameBuffer fb
		topoRenderer->updateUniforms(*camera);
		topoRenderer->drawTopography();

		// draw stategrid into FrameBuffer fb
		stategridRenderer->drawStategrid(*camera);

		fb->unbind();
		glViewport(0, 0, window.getWidth(), window.getHeight());

		// show visibility menu and update visibility regardless of state
		gui.showUI_Visibility(config_Stategrid);
		environment->stategrid.updateVisibility(config_Stategrid);

		// show fb in viewport window
		gui.showUI_Viewport(*fb);

		// render the whole gui
		gui.render();

		// resize fb if viewport was resized
		if (fb->width != gui.getViewportSize().x || fb->height != gui.getViewportSize().y) 
			fb->resize(gui.getViewportSize().x, gui.getViewportSize().y);

		window.swapBuffers();
		window.updateTime();
	}
}

int Visualization::quit() {
	window.quit();
	return 0;
}

void Visualization::inEnvironment() {
	// topography was changed by user
	if (config_Environment.topographyType != environment->topography.getType() ||
		config_Environment.seed != environment->topography.getSeed() ||
		config_Environment.terrainScaling != environment->topography.getScale())
	{
		environment->resetTopography(
			config_Environment.seed, 
			config_Environment.terrainScaling, 
			config_Environment.topographyType);

		environment->resetGrid(config_Environment.gridSize);

		topoRenderer->setTopography(environment->topography);
	}

	// gridsize was changed by user
	if (config_Environment.gridSize != environment->stategrid.gridSize) {
		environment->resetGrid(config_Environment.gridSize);
	}
}

void Visualization::environmentToProblem() {
	// Prepare graph and search problem
	environment->graph.reset(environment->stategrid.gridSize, environment->topography.getSize());
	environment->graph.setTopography(environment->topography);
	problem->initial = State(0, environment->topography.getY(0, 0), 0);
	problem->goal = State(0, environment->topography.getY(0, 0), 0);

	// Unexplored nodes should be invisble when configuring search problem
	config_Stategrid.defaultVisible = false; 
}

void Visualization::inProblem() {
	// Initial state was changed by user
	if (config_Problem.initial != problem->initial.getXZ()) { 
		environment->stategrid.grid[problem->initial] = config_Stategrid.defaultColor;
		problem->initial.y = environment->topography.getY(config_Problem.initial.x, config_Problem.initial.y);
		problem->initial.setXZ(config_Problem.initial);
	}

	// Goal state was changed by user
	if (config_Problem.goal != problem->goal.getXZ()) { 
		environment->stategrid.grid[problem->goal] = config_Stategrid.defaultColor;
		problem->goal.y = environment->topography.getY(config_Problem.goal.x, config_Problem.goal.y);
		problem->goal.setXZ(config_Problem.goal);
	}

	// Set initial and goal state colors
	environment->stategrid.grid[problem->initial] = config_Stategrid.initialStateColor;
	environment->stategrid.grid[problem->goal] = config_Stategrid.goalStateColor;

	// Heuristic was changed by user
	if (config_Problem.heuristic != aStar->getHeuristic().heuristicId)
		aStar->setHeuristic(config_Problem.heuristic);

	// Heuristic overestimate factor was changed by user
	if (config_Problem.overestimateFactor != aStar->getHeuristic().overestimateFactor)
		aStar->setHeuristic(config_Problem.heuristic, config_Problem.overestimateFactor);
}

int Visualization::problemToSearching() {
	// Start the configured search
	aStar->search();
	if (aStar->getSolution() == nullptr) {
		std::cout << "No path found." << std::endl;
		return -1;
	}

	// Initialize playback config
	config_Playback.step = 0;
	config_Playback.maxSteps = aStar->allExpanded.size();
}

void Visualization::inSearching() {
	config_Playback.updateTimePerIncrement();

	// Increment config step if its playing and enough time (timePerIncrement) has passed
	if (config_Playback.searchPlaying) {
		config_Playback.searchTime += window.getDelta();
		if (config_Playback.searchTime >= config_Playback.timePerIncrement) {
			config_Playback.step++;
			config_Playback.searchTime -= config_Playback.timePerIncrement;
		}
	}

	if (config_Playback.step > step) { // Forwards step by user
		environment->stategrid.updateToStep(config_Playback.step - 1, problem->graph, *aStar, true);
	}
	else if (config_Playback.step < step) { // Backwards step by user
		environment->stategrid.updateToStep(config_Playback.step, problem->graph, *aStar, false);
	}

	// Update visualization step 
	step = config_Playback.step;
}

void Visualization::searchingToFinished() {
	// Show and highlight solution states
	environment->stategrid.showSolutionPath(aStar->solutionPath, *problem);
}

void Visualization::finishedToEnvironment() {
	// Reset stategrid colors, search problem and playback configuration
	environment->resetGrid(environment->stategrid.gridSize);
	config_Stategrid.defaultVisible = true;
	config_Problem.reset();
	config_Playback.step = 0;
	config_Playback.maxSteps = 0;
}

void Visualization::finishedToProblem() {
	// Reset stategrid colors and playback configuration
	environment->resetGrid(environment->stategrid.gridSize);
	config_Playback.step = 0;
	config_Playback.maxSteps = 0;
}

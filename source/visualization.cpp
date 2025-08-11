#include "visualization.h"
#include "model/state.h"
#include "opengl/sphere.h"
#include "opengl/camera.h"

Visualization::Visualization() : window("A* Visualisierung") {}

bool Visualization::init() {
	if (!window.init()) {
		return false;
	}

	if (!BaseRenderer::init()) {
		return false;
	}

	BaseRenderer::setClearColor(0.1f, 0.1f, 0.1f);

	config_Stategrid.init();
	config_Environment.init();
	config_Problem.init();
	config_Playback.init();

	gui.init(window);

	environment = new SearchEnvironment(config_Environment, config_Stategrid);
	problem = new SearchProblem(*environment, config_Problem);
	aStar = new AStarSearch(*problem, config_Problem, *environment);

	topoRenderer = new TopographyRenderer(environment->topography);
	stategridRenderer = new StategridRenderer(environment->stateGrid);

	fb = new Framebuffer(window.getWidth() , window.getHeight());
	vao = new VertexArray();
	camera = new Camera(glm::radians(90.0f), window.getWidth(), window.getHeight(), 10.0f, 20000.0f);
	camera->translate(glm::vec3(
		environment->topography.getSize() / 2,
		10000.0f, 
		environment->topography.getSize() / 2));
	camera->rotate(glm::vec2(-90.0f, 90.0f));

	topoRenderer->setupUniforms(camera);
	stategridRenderer->setupUniforms(camera);

	step = 0;

	state = VisualizationState::ConfiguringSearchEnvironment;

	return true;
}

void Visualization::run() {
	while (true) {
		gui.startFrame();

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
				inFinished();
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

		//draw topography
		topoRenderer->updateUniforms(camera);
		topoRenderer->drawTopography();

		//draw stategrid
		stategridRenderer->drawStategrid(camera);

		fb->unbind();
		glViewport(0, 0, window.getWidth(), window.getHeight());

		gui.showUI_Visibility(config_Stategrid);
		environment->stateGrid.updateVisibility(config_Stategrid);

		gui.showUI_Viewport(fb);
		gui.render();

		//if (fb->width != gui.getViewportSize().x || fb->height != gui.getViewportSize().y) 
			fb->resize(gui.getViewportSize().x, gui.getViewportSize().y);

		window.swapBuffers();
		window.updateTime();
	}
}

int Visualization::quit() {
	window.quit();
	delete environment;
	delete problem;
	delete aStar;
	delete stategridRenderer;
	delete topoRenderer;
	delete fb;
	delete vao;
	delete camera;
	return 0;
}

void Visualization::inEnvironment() {
	//topography was changed
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

	if (config_Environment.gridSize != environment->stateGrid.gridSize) {
		environment->resetGrid(config_Environment.gridSize);
	}
}

void Visualization::environmentToProblem() {
	environment->graph.reset(environment->stateGrid.gridSize, environment->topography.getSize());
	environment->graph.setTopography(&environment->topography);
	environment->graph.init();
	problem->initial = State(0, environment->topography.getY(0, 0), 0);
	problem->goal = State(0, environment->topography.getY(0, 0), 0);
	problem->graph = environment->graph;
	config_Stategrid.defaultVisible = false; //unexplored nodes are invisble when configuring search problem
}

void Visualization::inProblem() {
	if (config_Problem.initial != problem->initial.getXZ()) { //initial state updated
		environment->stateGrid.grid[problem->initial] = config_Stategrid.defaultColor;
		problem->initial.y = environment->topography.getY(config_Problem.initial.x, config_Problem.initial.y);
		problem->initial.setXZ(config_Problem.initial);
	}
	if (config_Problem.goal != problem->goal.getXZ()) { //goal state updated
		environment->stateGrid.grid[problem->goal] = config_Stategrid.defaultColor;
		problem->goal.y = environment->topography.getY(config_Problem.goal.x, config_Problem.goal.y);
		problem->goal.setXZ(config_Problem.goal);
	}

	environment->stateGrid.grid[problem->initial] = config_Stategrid.initialStateColor;
	environment->stateGrid.grid[problem->goal] = config_Stategrid.goalStateColor;

	if (config_Problem.heuristic != aStar->getHeuristic().heuristicId)
		aStar->setHeuristic(config_Problem.heuristic);

	if (config_Problem.overestimateFactor != aStar->getHeuristic().overestimateFactor)
		aStar->setHeuristic(config_Problem.heuristic, config_Problem.overestimateFactor);
}

int Visualization::problemToSearching() {
	aStar->search();
	if (aStar->getSolution() == nullptr) {
		std::cout << "No path found." << std::endl;
		return -1;
	}

	config_Playback.step = 0;
	config_Playback.maxSteps = aStar->allExpanded.size() - 1;
}

void Visualization::inSearching() {
	config_Playback.updateTimePerIncrement();

	if (config_Playback.searchPlaying) {
		config_Playback.searchTime += window.getDelta();
		if (config_Playback.searchTime >= config_Playback.timePerIncrement) {
			config_Playback.step++;
			config_Playback.searchTime -= config_Playback.timePerIncrement;
		}
	}

	if (config_Playback.step > step) { //forwards step
		environment->stateGrid.updateToStep(config_Playback.step - 1, problem->graph, *aStar, true);
	}
	else if (config_Playback.step < step) { // backwards step
		environment->stateGrid.updateToStep(config_Playback.step, problem->graph, *aStar, false);
	}
	step = config_Playback.step;
}

void Visualization::searchingToFinished() {
	environment->stateGrid.showSolutionPath(aStar->solutionPath, *problem);
}

void Visualization::inFinished() {

}

void Visualization::finishedToEnvironment() {
	environment->resetGrid(environment->stateGrid.gridSize);
	config_Stategrid.defaultVisible = true;
	config_Problem.init();
	config_Playback.step = 0;
	config_Playback.maxSteps = 0;
}

void Visualization::finishedToProblem() {
	environment->resetGrid(environment->stateGrid.gridSize);
	config_Playback.step = 0;
	config_Playback.maxSteps = 0;
}

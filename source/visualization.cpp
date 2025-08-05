#include "visualization.h"

Visualization::Visualization() : window("A* Visualisierung") {}

bool Visualization::init() {
	if (!window.init()) {
		return false;
	}

	if (!BaseRenderer::init()) {
		return false;
	}

	gui.init(window);
	gui.setSearchConfig(&config);
	environment = new SearchEnvironment(0, 15, 0.0001f, 0);
	problem = new SearchProblem(environment->graph);
	aStar = new AStarSearch(*problem);
	topoRenderer = new TopographyRenderer(500.0f);
	shapeRenderer = new ShapeRenderer();
	topoRenderer->setupUniforms();
	topoRenderer->setClearColor(0.1f, 0.1f, 0.1f);
	shapeRenderer->setupUniforms();
	fb = new Framebuffer(window.getWidth() , window.getHeight());
	vao = new VertexArray();
	camera = new Camera(glm::radians(90.0f), window.getWidth(), window.getHeight(), 10.0f, 20000.0f);
	camera->translate(glm::vec3(environment->topography.getSize() / 2, 10000.0f, 
		environment->topography.getSize() / 2));
	camera->rotate(glm::vec2(-90.0f, 90.0f));
	camera->update();
	running = true;
	environment->resetGrid(config.gridSize);
}

void Visualization::run() {
	while (running) {
		gui.startFrame();
		inputHandler.handleEvents(running, gui);

		switch (state) {
			case VisualizationState::ConfiguringSearchEnvironment:
				state = gui.showUI_EnvironmentConfig();
				inEnvironment();
				if (state == VisualizationState::ConfiguringSearchProblem) 
					environmentToProblem();
				break;

			case VisualizationState::ConfiguringSearchProblem:
				state = gui.showUI_SearchProblemConfig(environment->graph->getSpacing());
				inProblem();
				if (state == VisualizationState::Searching)
					if (problemToSearching() == -1) break;
				break;

			case VisualizationState::Searching:
				state = gui.showUI_Searching();
				inSearching();
				if (state == VisualizationState::Finished)
					searchingToFinished();
				break;

			case VisualizationState::Finished:
				state = gui.showUI_Finished(
					aStar->solutionPath.size(),
					aStar->allExpanded.size(),
					aStar->consideredNodes,
					solution->pathCost);
				inFinished();
				if (state == VisualizationState::ConfiguringSearchEnvironment)
					finishedToEnvironment();
				else if (state == VisualizationState::ConfiguringSearchProblem)
					finishedToProblem();
				break;
		}

		fb->bind();
		glViewport(0, 0, (int)fb->width, (int)fb->height);
		camera->resizeProj(fb->width, fb->height);
		topoRenderer->clear();
		topoRenderer->updateUniforms(camera);
		topoRenderer->draw(&environment->topography);
		//TODO visible dinger ins stategrid? als struct?
		for (auto& kv : environment->stateGrid->grid) {
			glm::vec3 color = kv.second->color;

			if (color == config.frontierColor && !config.frontierVisible ||
				color == config.defaultColor && !config.unexploredVisible ||
				color == config.reachedColor && !config.reachedVisible)
				continue;

			if (shapeRenderer->getColor() != color) shapeRenderer->setColor(color);
			shapeRenderer->updateUniforms(camera, kv.second->model);
			shapeRenderer->draw(kv.second);
		}

		fb->unbind();
		gui.showUI_Visibility();
		gui.showUI_Viewport(*fb);

		gui.render();

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
	delete solution;
	return 0;
}

void Visualization::inEnvironment() {
	if (config.topographyType != environment->topography.getType() ||
		config.seed != environment->topography.getSeed() ||
		config.terrainScaling != environment->topography.getScale())
	{
		environment->resetTopography(config.seed, config.terrainScaling, config.topographyType);
		environment->resetGrid(config.gridSize);
	}

	if (config.gridSize != environment->stateGrid->gridSize) {
		environment->resetGrid(config.gridSize);
	}
}

void Visualization::environmentToProblem() {
	delete environment->graph;
	environment->graph = new Graph(config.gridSize, environment->topography.getSize());
	environment->graph->setTopography(&environment->topography);
	environment->graph->init();
	problem->initial = State(0, environment->topography.getY(0, 0), 0);
	problem->goal = State(0, environment->topography.getY(0, 0), 0);
	problem->g = environment->graph;
	config.unexploredVisible = false; //unexplored nodes are invisble when configuring search problem
}

void Visualization::inProblem() {
	if (config.initial != problem->initial.getXZ()) { //initial state updated
		environment->stateGrid->grid[problem->initial]->color = config.defaultColor;
		problem->initial.y = environment->topography.getY(config.initial.x, config.initial.y);
		problem->initial.setXZ(config.initial);
	}
	if (config.goal != problem->goal.getXZ()) { //goal state updated
		environment->stateGrid->grid[problem->goal]->color = config.defaultColor;
		problem->goal.y = environment->topography.getY(config.goal.x, config.goal.y);
		problem->goal.setXZ(config.goal);
	}

	environment->stateGrid->grid[problem->initial]->color = config.initialStateColor;
	environment->stateGrid->grid[problem->goal]->color = config.goalStateColor;

	switch (config.heuristic) {
	case 0:
		aStar->setHeuristic(Heuristics::travelTime_Overestimated(0.0f));
		break;
	case 1:
		aStar->setHeuristic(Heuristics::travelTime);
		break;
	case 2:
		aStar->setHeuristic(Heuristics::travelTime_Overestimated(config.overestimateFactor));
		break;
	case 3:
		aStar->setHeuristic(Heuristics::travelTime_Intersections(&environment->topography));
		break;
	case 4:
		aStar->setHeuristic(Heuristics::travelTime_WeightedHeights(&environment->topography));
		break;
	}
}

int Visualization::problemToSearching() {
	solution = aStar->search();
	if (solution == nullptr) {
		std::cout << "No path found." << std::endl;
		return -1;
	}
	config.step = 0; //TODO: was soll der müll
	config.maxSteps = aStar->allExpanded.size() - 1;
}

void Visualization::inSearching() {
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
		environment->stateGrid->updateToStep(config.step - 1, *problem->g, *aStar, true);
	}
	else if (config.step < step) { // backwards step
		environment->stateGrid->updateToStep(config.step, *problem->g, *aStar, false);
	}
	step = config.step;
}

void Visualization::searchingToFinished() {

}

void Visualization::inFinished() {
	for (State s : aStar->solutionPath) {
		if (s != problem->initial && s != problem->goal) {
			environment->stateGrid->grid[s]->color = glm::vec3(1.0f, 1.0f, 0.0f);
		}
	}
}

void Visualization::finishedToEnvironment() {
	environment->resetGrid(environment->stateGrid->gridSize);
	config.reset();
}

void Visualization::finishedToProblem() {
	environment->resetGrid(environment->stateGrid->gridSize);
}

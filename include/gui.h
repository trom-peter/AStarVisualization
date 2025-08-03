#pragma once
#include "window.h"
#include "framebuffer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "search_configuration.h"
#include "visualization_state.h"

class Window;

class GUI {
public:
	GUI();
	void init(Window& window);
	void processEvent(SDL_Event*);
	void startFrame();
	void render();
	bool wantsMouseInput();
	bool wantsKeyboardInput();
	void quit();
	bool isViewportActive() const;
	void setViewportActive(bool active);
	void setSearchConfig(SearchConfiguration* config);
	VisualizationState showUI_EnvironmentConfig();
	VisualizationState showUI_SearchProblemConfig(int stateSpacing);
	VisualizationState showUI_Searching();
	VisualizationState showUI_Finished(int solutionNodes, int expandedNodes, int frontierNodes, float travelTime);
	void showUI_Viewport(Framebuffer& fb);
	void showUI_Visibility();

private:
	SearchConfiguration* config;
	ImFont* font;
	bool viewportActive;
};

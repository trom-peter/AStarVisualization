#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "visualization_state.h"

struct StategridConfig;
struct EnvironmentConfig;
struct ProblemConfig;
struct PlaybackConfig;
class AStarSearch;
class Window;
struct Framebuffer;

class GUI {
public:
	GUI();
	void init(Window& window);
	void processWindowEvents(bool& running);
	void startFrame();
	void render();
	ImVec2 getViewportSize();
	void quit();
	VisualizationState showUI_EnvironmentConfig(EnvironmentConfig& envConfig);
	VisualizationState showUI_SearchProblemConfig(ProblemConfig& problemConfig, EnvironmentConfig& envConfig);
	VisualizationState showUI_Searching(PlaybackConfig& playbackConfig);
	VisualizationState showUI_Finished(AStarSearch& aStar, EnvironmentConfig& envConfig);
	void showUI_Viewport(Framebuffer* fb);
	void showUI_Visibility(StategridConfig& gridConfig);

private:
	ImFont* font;
	ImVec2 viewportSize;
	ImGuiWindowFlags windowFlags;
};

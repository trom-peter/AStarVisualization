#pragma once
#include <memory>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "visualization/visualization_state.h"

struct StategridConfig;
struct EnvironmentConfig;
struct ProblemConfig;
struct PlaybackConfig;
class AStarSearch;
class Window;
struct FrameBuffer;

class GUI {
public:
	GUI();
	void init(const Window& window);
	bool isWindowExited();
	void startFrame();
	void render() const;
	ImVec2 getViewportSize() const;
	void quit();
	VisualizationState showUI_EnvironmentConfig(EnvironmentConfig& envConfig) const;
	VisualizationState showUI_SearchProblemConfig(
		ProblemConfig& problemConfig, const EnvironmentConfig& envConfig) const;
	VisualizationState showUI_Searching(PlaybackConfig& playbackConfig) const;
	VisualizationState showUI_Finished(const AStarSearch& aStar) const;
	void showUI_Viewport(FrameBuffer& fb);
	void showUI_Visibility(StategridConfig& gridConfig) const;

private:
	ImFont* font;
	ImVec2 viewportSize;
	ImGuiWindowFlags windowFlags;
	ImGuiDockNodeFlags dockingFlags;
};

#pragma once
#include "window.h"
#include "framebuffer.h"
#include "a_star/problem.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

class Window;

class GUI {
public:
	GUI();
	void init(Window& window);
	void processEvent(SDL_Event*);
	void startFrame();
	void renderStuff(Framebuffer& fb, bool& configuring);
	bool wantsMouseInput();
	bool wantsKeyboardInput();
	void quit();
	int getStep();
	glm::vec3 getDefaultColor();
	glm::vec3 getFrontierColor();
	glm::vec3 getReachedColor();
	glm::ivec2 getInitial();
	glm::ivec2 getGoal();
	bool isViewportActive() const;
	void setViewportActive(bool active);
	void setMaxSteps(int maxSteps);
	void setStateSpacing(int spacing);

private:

	int step;
	ImVec4 defaultColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 frontierColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 reachedColor = ImVec4(0.17f, 0.63f, 1.0f, 1.0f);
	int maxSteps;
	glm::ivec2 initial;
	glm::ivec2 goal;
	int stateSpacing; //the perpendicular space between all states

	bool showDemoWindow;
	bool viewportActive;
};

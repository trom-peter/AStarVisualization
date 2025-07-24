#pragma once
#include "window.h"
#include "framebuffer.h"
#include "a_star/problem.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "search_configuration.h"

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
	bool isViewportActive() const;
	void setViewportActive(bool active);
	void setSearchConfig(SearchConfiguration* config);

private:
	SearchConfiguration* config;

	bool showDemoWindow;
	bool viewportActive;
};

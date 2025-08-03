#pragma once
#include "window.h"
#include "gui.h"

class GUI;

class InputHandler {
public:
	InputHandler();

	void handleEvents(bool& running, GUI& gui);
	bool isKeyPressed(SDL_Scancode key) const;
	Uint8 getCurrentKeyStates();

private:
	const Uint8* currentKeyStates;
};
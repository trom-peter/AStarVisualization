#include "input_handler.h"

InputHandler::InputHandler() : currentKeyStates(SDL_GetKeyboardState(NULL)) {}

void InputHandler::handleEvents(bool& running, GUI& gui) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        gui.processEvent(&event);
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
    currentKeyStates = SDL_GetKeyboardState(NULL);
}

bool InputHandler::isKeyPressed(SDL_Scancode key) const {
    return currentKeyStates[key] == 1;
}

Uint8 InputHandler::getCurrentKeyStates() {
    return *currentKeyStates;
}

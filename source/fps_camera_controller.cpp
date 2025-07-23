#include "fps_camera_controller.h"

FPSCameraController::FPSCameraController(FPSCamera& camera) : camera(camera) {}

void FPSCameraController::handleInputs(InputHandler* ih, float delta) {
    if (ih->isKeyPressed(SDL_SCANCODE_W) && ih->isKeyPressed(SDL_SCANCODE_A)) camera.move(camera.getSpeed() * delta, -camera.getSpeed() * delta);
    else if (ih->isKeyPressed(SDL_SCANCODE_W) && ih->isKeyPressed(SDL_SCANCODE_D)) camera.move(camera.getSpeed() * delta, camera.getSpeed() * delta);
    else if (ih->isKeyPressed(SDL_SCANCODE_S) && ih->isKeyPressed(SDL_SCANCODE_A)) camera.move(-camera.getSpeed() * delta, -camera.getSpeed() * delta);
    else if (ih->isKeyPressed(SDL_SCANCODE_S) && ih->isKeyPressed(SDL_SCANCODE_D)) camera.move(-camera.getSpeed() * delta, camera.getSpeed() * delta);
    else {
        if (ih->isKeyPressed(SDL_SCANCODE_W)) camera.move(camera.getSpeed() * delta, 0);
        if (ih->isKeyPressed(SDL_SCANCODE_A)) camera.move(0, -camera.getSpeed() * delta);
        if (ih->isKeyPressed(SDL_SCANCODE_S)) camera.move(-camera.getSpeed() * delta, 0);
        if (ih->isKeyPressed(SDL_SCANCODE_D)) camera.move(0, camera.getSpeed() * delta);
    }
    if (ih->isKeyPressed(SDL_SCANCODE_SPACE)) camera.moveUp(camera.getSpeed() * delta);
    if (ih->isKeyPressed(SDL_SCANCODE_LSHIFT)) camera.moveUp(-camera.getSpeed() * delta);
    camera.onMouseMoved(ih->getRelativeX(), ih->getRelativeY());
    camera.update();
}
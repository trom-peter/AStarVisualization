#pragma once

#include "fps_camera.h"
#include "input_handler.h"

class FPSCameraController {
public:
	FPSCameraController(FPSCamera& camera);
	void handleInputs(InputHandler* ih, float delta);
private:
	FPSCamera& camera;
};
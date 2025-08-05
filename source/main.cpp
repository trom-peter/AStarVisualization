#include <iostream>
#include "visualization.h";

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

int main() {
    Visualization vis;

    if (!vis.init()) {
        std::cerr << "ERROR: Problem initializing program" << std::endl;
        return -1;
    }

    vis.run();

    return vis.quit();
}

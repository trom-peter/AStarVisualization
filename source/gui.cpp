#include "gui.h"

GUI::GUI() : step(0), initial(glm::ivec2(0, 0)), goal(glm::ivec2(0, 0)), viewportActive(false) {}

void GUI::init(Window& window) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), window.getGLContext());
    ImGui_ImplOpenGL3_Init("#version 450");

    // Our state
    showDemoWindow = true;
}

void GUI::processEvent(SDL_Event* event) {
    ImGui_ImplSDL2_ProcessEvent(event);
}

void GUI::startFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void GUI::renderStuff(Framebuffer& fb, bool& configuring) {
    ImGuiDockNodeFlags dockingFlags = ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpaceOverViewport(dockingFlags);

    // Konfigurationen
    ImGui::Begin("Konfiguration", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    if (configuring) {
        ImGui::InputInt("Startzustand X", &initial.x, stateSpacing);
        ImGui::InputInt("Startzustand Z", &initial.y, stateSpacing);

        ImGui::InputInt("Zielzustand X", &goal.x, stateSpacing);
        ImGui::InputInt("Zielzustand Z", &goal.y, stateSpacing);
        configuring = !ImGui::Button("Suche!");
    }
    else {
        ImGui::InputInt("Schritt", &step);
    }
    ImGui::ColorEdit3("Frontier Farbe", (float*)&frontierColor);
    ImGui::ColorEdit3("Reached Farbe", (float*)&reachedColor);
    ImGui::ColorEdit3("Default Farbe", (float*)&defaultColor);

    ImGui::End();
    
    // Statistik
    ImGui::Begin("Statistik", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::End();

    // Viewport
    ImGui::Begin("Visualisierung", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    {
        ImGui::BeginChild("Viewport");

        // Check if Viewport window is active
        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !viewportActive) 
            viewportActive = true;
        else 
            viewportActive = false;
        
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((ImTextureID)fb.colorTextureId, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }
    ImGui::End();

    if (step < 0) step = 0;
    else if (step > maxSteps) step = maxSteps;
    //TODO max and min inital and goal coordiantes

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

bool GUI::wantsMouseInput() {
    return ImGui::GetIO().WantCaptureMouse;
}

bool GUI::wantsKeyboardInput() {
    return ImGui::GetIO().WantCaptureKeyboard;
}

void GUI::quit() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

int GUI::getStep() {
    return step;
}

glm::vec3 GUI::getDefaultColor() {
    return glm::vec3(defaultColor.x, defaultColor.y, defaultColor.z);
}

glm::vec3 GUI::getFrontierColor() {
    return glm::vec3(frontierColor.x, frontierColor.y, frontierColor.z);
}

glm::vec3 GUI::getReachedColor() {
    return glm::vec3(reachedColor.x, reachedColor.y, reachedColor.z);
}

glm::ivec2 GUI::getInitial() {
    return initial;
}

glm::ivec2 GUI::getGoal() {
    return goal;
}

bool GUI::isViewportActive() const {
    return viewportActive;
}

void GUI::setViewportActive(bool active) {
    viewportActive = active;
}

void GUI::setMaxSteps(int maxSteps) {
    this->maxSteps = maxSteps;
}

void GUI::setStateSpacing(int stateSpacing) {
    this->stateSpacing = stateSpacing;
}

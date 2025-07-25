#include "gui.h"
#include <algorithm>
#include "seed_generator.h"

GUI::GUI() : viewportActive(false), config(nullptr) {}

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
    ImGuiDockNodeFlags dockingFlags = ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpaceOverViewport(dockingFlags);
}

bool GUI::showUI_EnvironmentConfig() {
    bool changedState = false;
    //Suchraum
    ImGui::Begin("Suchraum");
    char buffer[16];
    sprintf_s(buffer, "%d", config->seed);
    ImGui::Text("Seed");
    if (ImGui::InputText("##seed", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll) || ImGui::IsItemDeactivatedAfterEdit()) {
        config->seed = atoi(buffer);
    }
    if (ImGui::Button("Random")) {
        config->seed = SeedGenerator::getRandomSeed();
    }

    ImGui::NewLine();

    ImGui::Text("Graphgröße");
    if (ImGui::InputInt("##graphsize", &config->gridSize)) {
        config->gridSize = std::clamp(config->gridSize, 2, 20);
    }

    if (ImGui::Button("Fertig")) changedState = true;

    ImGui::End();

    return changedState;
}

bool GUI::showUI_SearchProblemConfig() {
    bool changedState = false;

    // Konfigurationen
    ImGui::Begin("Konfiguration", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Startzustand"); ImGui::SameLine(125); ImGui::Text("Zielzustand");

    // Startzustand Konfiguration
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##up1", ImGuiDir_Up)) {
        config->initial.y = std::clamp(config->initial.y - config->stateSpacing, 0, (config->gridSize - 1) * config->stateSpacing);
    }
    ImGui::BeginGroup();
    if (ImGui::ArrowButton("##left1", ImGuiDir_Left)) {
        config->initial.x = std::clamp(config->initial.x - config->stateSpacing, 0, (config->gridSize - 1) * config->stateSpacing);
    }
    ImGui::SameLine(0, 30);
    if (ImGui::ArrowButton("##right1", ImGuiDir_Right)) {
        config->initial.x = std::clamp(config->initial.x + config->stateSpacing, 0, (config->gridSize - 1) * config->stateSpacing);
    }
    ImGui::EndGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##down1", ImGuiDir_Down)) {
        config->initial.y = std::clamp(config->initial.y + config->stateSpacing, 0, (config->gridSize - 1) * config->stateSpacing);
    }
    ImGui::EndGroup();

    ImGui::SameLine(0, 50);

    // Zielzustand Konfiguration
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##up2", ImGuiDir_Up)) {
        config->goal.y = std::clamp(config->goal.y - config->stateSpacing, 0, (config->gridSize - 1) * config->stateSpacing);
    }
    ImGui::BeginGroup();
    if (ImGui::ArrowButton("##left2", ImGuiDir_Left)) {
        config->goal.x = std::clamp(config->goal.x - config->stateSpacing, 0, (config->gridSize - 1) * config->stateSpacing);
    }
    ImGui::SameLine(0, 30);
    if (ImGui::ArrowButton("##right2", ImGuiDir_Right)) {
        config->goal.x = std::clamp(config->goal.x + config->stateSpacing, 0, (config->gridSize - 1) * config->stateSpacing);
    }
    ImGui::EndGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##down2", ImGuiDir_Down)) {
        config->goal.y = std::clamp(config->goal.y + config->stateSpacing, 0, (config->gridSize - 1) * config->stateSpacing);
    }
    ImGui::EndGroup();

    ImGui::NewLine();

    if (ImGui::Button("Suche!", ImVec2(80.0f, 40.0f))) changedState = true;

    ImGui::End();

    return changedState;
}

bool GUI::showUI_Searching() {
    bool changedState = false;

    ImGui::Begin("Suche", nullptr);
    if (ImGui::InputInt("Schritt", &config->step)) {
        config->step = std::clamp(config->step, 0, config->maxSteps);
        if (config->step == config->maxSteps) ImGui::OpenPopup("Pfad gefunden!"); //open a pop up when the search is finished (only the first time)
    }

    if (ImGui::BeginPopupModal("Pfad gefunden!")) {
        if (ImGui::Button("Ok", ImVec2(80.0f, 30.0f))) {
            changedState = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::End();

    return changedState;
}

bool GUI::showUI_Finished() {
    bool changedState = false;
    ImGui::Begin("Statistik", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::End();
    return false;
}

void GUI::showUI_Viewport(Framebuffer& fb) {
    // viewport
    ImGui::Begin("Visualisierung", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    {
        ImGui::BeginChild("Viewport");

        // check if viewport window is active
        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !viewportActive)
            viewportActive = true;
        else
            viewportActive = false;

        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((ImTextureID)fb.colorTextureId, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }
    ImGui::End();
}

void GUI::showUI_Visibility() {
    ImGui::Begin("Sichtbarkeit");
    ImGui::Checkbox("Unbesucht", &config->unexploredVisible);
    ImGui::Checkbox("Grenzbereich", &config->frontierVisible);
    ImGui::Checkbox("Reached", &config->reachedVisible);
    ImGui::End();
}

void GUI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // update and render additional windows
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

bool GUI::isViewportActive() const {
    return viewportActive;
}

void GUI::setViewportActive(bool active) {
    viewportActive = active;
}

void GUI::setSearchConfig(SearchConfiguration* config) {
    this->config = config;
}

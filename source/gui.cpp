#include "gui.h"
#include <algorithm>

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
}

void GUI::renderStuff(Framebuffer& fb, bool& configuring) {
    ImGuiDockNodeFlags dockingFlags = ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpaceOverViewport(dockingFlags);

    // Konfigurationen
    ImGui::Begin("Konfiguration", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    if (configuring) {
        ImGui::Text("Startzustand");
        if (ImGui::InputInt("X##start", &config->initial.x, config->stateSpacing)) 
            config->initial.x = std::clamp(config->initial.x, 0, (config->gridSize - 1) * config->stateSpacing);

        if (ImGui::InputInt("Z##start", &config->initial.y, config->stateSpacing)) 
            config->initial.y = std::clamp(config->initial.y, 0, (config->gridSize - 1) * config->stateSpacing);

        ImGui::NewLine();

        ImGui::Text("Zielzustand");
        if (ImGui::InputInt("X##ziel", &config->goal.x, config->stateSpacing)) 
            config->goal.x = std::clamp(config->goal.x, 0, (config->gridSize - 1) * config->stateSpacing);

        if (ImGui::InputInt("Z##ziel", &config->goal.y, config->stateSpacing)) 
            config->goal.y = std::clamp(config->goal.y, 0, (config->gridSize - 1) * config->stateSpacing);

        ImGui::NewLine();

        configuring = !ImGui::Button("Suche!", ImVec2(80.0f, 40.0f));

        if (ImGui::Button("Reset", ImVec2(50.0f, 25.0f))) config->reset();
    }
    else {
        if (ImGui::InputInt("Schritt", &config->step)) {
            if (config->step > config->maxSteps) {
                config->step = config->maxSteps;
                if (!config->finished) ImGui::OpenPopup("Pfad gefunden!"); //open a pop up when the search is finished for the first time
                config->finished = true;
            }
            else if (config->step < 0) {
                config->step = 0;
            }
        }
    }

    if (ImGui::BeginPopupModal("Pfad gefunden!")) {
        if (ImGui::Button("Ok", ImVec2(80.0f, 30.0f))) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    ImGui::Text("Sichtbarkeit");
    ImGui::Checkbox("Grenzbereich", &config->frontierVisible);
    ImGui::Checkbox("Reached", &config->reachedVisible);
    ImGui::Checkbox("Unbesucht", &config->unexploredVisible);

    ImGui::NewLine();

    ImGui::Text("Farben");
    ImGui::ColorEdit3("Frontier Farbe", (float*)&config->frontierColor);
    ImGui::ColorEdit3("Reached Farbe", (float*)&config->reachedColor);
    ImGui::ColorEdit3("Default Farbe", (float*)&config->defaultColor);

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

bool GUI::isViewportActive() const {
    return viewportActive;
}

void GUI::setViewportActive(bool active) {
    viewportActive = active;
}

void GUI::setSearchConfig(SearchConfiguration* config) {
    this->config = config;
}

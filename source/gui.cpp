#include "gui.h"
#include <algorithm>
#include "seed_generator.h"

GUI::GUI() : viewportActive(false), config(nullptr), font(nullptr) {}

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

    io.Fonts->AddFontDefault();
    font = io.Fonts->AddFontFromFileTTF("fonts/JetbrainsMonoRegular.ttf", 18.0f);
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
    ImGui::PushFont(font);
}

VisualizationState GUI::showUI_EnvironmentConfig() {
    VisualizationState nextState = VisualizationState::ConfiguringSearchEnvironment;

    //Suchraum
    ImGui::Begin("Suchraum");

    char buffer[4];
    sprintf_s(buffer, "%d", config->seed);
    ImGui::Text("Seed");
    if (ImGui::InputText("##seed", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll) || ImGui::IsItemDeactivatedAfterEdit()) {
        config->seed = static_cast<unsigned char>(std::clamp(atoi(buffer), 0, 255));
    }
    if (ImGui::Button(u8"Zufällig")) {
        config->seed = SeedGenerator::getRandomSeed();
    }

    ImGui::NewLine();

    ImGui::Text(u8"Geländetyp");
    ImGui::RadioButton("1", &config->topographyType, 0); ImGui::SameLine();
    ImGui::RadioButton("2", &config->topographyType, 1);

    ImGui::NewLine();

    ImGui::Text(u8"Komplexität");
    if (ImGui::Button("Einfach")) {
        config->terrainScaling = 0.0001f;
    }

    if (ImGui::Button("Mittel")) {
        config->terrainScaling = 0.00015f;
    }     

    if (ImGui::Button("Hoch")) {
        config->terrainScaling = 0.0002f;
    }

    ImGui::NewLine();

    ImGui::Text(u8"Gittergröße");
    if (ImGui::InputInt("##graphsize", &config->gridSize)) {
        config->gridSize = std::clamp(config->gridSize, 2, 30);
    }

    ImGui::NewLine();

    if (ImGui::Button("Fertig")) nextState = VisualizationState::ConfiguringSearchProblem;

    ImGui::End();

    return nextState;
}

VisualizationState GUI::showUI_SearchProblemConfig(int stateSpacing) {
    VisualizationState nextState = VisualizationState::ConfiguringSearchProblem;

    // Suchproblem
    ImGui::Begin("Suchproblem", nullptr);
    ImGui::Text("Startzustand"); ImGui::SameLine(125); ImGui::Text("Zielzustand");
    ImGui::NewLine();

    // Startzustand Konfiguration
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##up1", ImGuiDir_Up)) {
        config->initial.y = std::clamp(config->initial.y - stateSpacing, 0, (config->gridSize - 1) * stateSpacing);
    }
    ImGui::BeginGroup();
    if (ImGui::ArrowButton("##left1", ImGuiDir_Left)) {
        config->initial.x = std::clamp(config->initial.x - stateSpacing, 0, (config->gridSize - 1) * stateSpacing);
    }
    ImGui::SameLine(0, 30);
    if (ImGui::ArrowButton("##right1", ImGuiDir_Right)) {
        config->initial.x = std::clamp(config->initial.x + stateSpacing, 0, (config->gridSize - 1) * stateSpacing);
    }
    ImGui::EndGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##down1", ImGuiDir_Down)) {
        config->initial.y = std::clamp(config->initial.y + stateSpacing, 0, (config->gridSize - 1) * stateSpacing);
    }
    ImGui::EndGroup();

    ImGui::SameLine(0, 50);

    // Zielzustand Konfiguration
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##up2", ImGuiDir_Up)) {
        config->goal.y = std::clamp(config->goal.y - stateSpacing, 0, (config->gridSize - 1) * stateSpacing);
    }
    ImGui::BeginGroup();
    if (ImGui::ArrowButton("##left2", ImGuiDir_Left)) {
        config->goal.x = std::clamp(config->goal.x - stateSpacing, 0, (config->gridSize - 1) * stateSpacing);
    }
    ImGui::SameLine(0, 30);
    if (ImGui::ArrowButton("##right2", ImGuiDir_Right)) {
        config->goal.x = std::clamp(config->goal.x + stateSpacing, 0, (config->gridSize - 1) * stateSpacing);
    }
    ImGui::EndGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##down2", ImGuiDir_Down)) {
        config->goal.y = std::clamp(config->goal.y + stateSpacing, 0, (config->gridSize - 1) * stateSpacing);
    }
    ImGui::EndGroup();

    ImGui::NewLine();

    // Suchverfahren Auswahl
    const char* items[] = { "Keine", "Wanderdauer", 
        u8"Wanderdauer Überschätzt", "Schnittpunkte", u8"Höhengewichtung"};

    ImGui::Combo("Heuristik", &config->heuristic, items, IM_ARRAYSIZE(items));

    if (config->heuristic == 2 && ImGui::InputFloat("Faktor", &config->overestimateFactor, 0.1f, 1.0f)) {
        config->overestimateFactor = std::clamp(config->overestimateFactor, 0.0f, 100.0f);
    }

    if (ImGui::Button("Suche!", ImVec2(80.0f, 40.0f))) nextState = VisualizationState::Searching;

    ImGui::End();

    return nextState;
}

VisualizationState GUI::showUI_Searching() {
    VisualizationState nextState = VisualizationState::Searching;

    ImGui::Begin("Suche", nullptr);

    ImGui::Text("Schritte pro Sekunde");

    if (ImGui::InputInt("##stepsPerSecond", &config->searchRate, 1, 5)) {
        config->searchRate = std::clamp(config->searchRate, 0, 60);
    }

    ImGui::SameLine();

    if (ImGui::ArrowButton("Play", ImGuiDir_Right)) {
        config->searchPlaying = true;
    }

    if (ImGui::Button("Pause")) {
        config->searchPlaying = false;
    }

    ImGui::NewLine();

    if (config->searchPlaying) ImGui::BeginDisabled();
    ImGui::Text("Schritt");
    if (ImGui::InputInt("##step", &config->step) || config->searchPlaying) {
        config->step = std::clamp(config->step, 0, config->maxSteps);
    }
    if (config->searchPlaying) ImGui::EndDisabled();

    if (config->step == config->maxSteps) {
        config->searchPlaying = false;
        ImGui::OpenPopup("Pfad gefunden!"); //open a pop up when the search is finished (only the first time)
    }

    if (ImGui::BeginPopupModal("Pfad gefunden!")) {
        if (ImGui::Button("Ok", ImVec2(80.0f, 30.0f))) {
            nextState = VisualizationState::Finished;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::End();

    return nextState;
}

VisualizationState GUI::showUI_Finished(int solutionNodes, int expandedNodes, int frontierNodes, float travelTime) {
    VisualizationState nextState = VisualizationState::Finished;

    ImGui::Begin("Statistik", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    char sTime[100];
    sprintf_s(sTime, "%.2fh", travelTime / 3600.0f); // save travel time in hours
    ImGui::LabelText(sTime, u8"Wanderdauer");

    ImGui::NewLine();

    ImGui::Text("Knoten");
    ImGui::LabelText(std::to_string(frontierNodes).c_str(), u8"Betrachtet");
    ImGui::LabelText(std::to_string(expandedNodes).c_str(), "Expandiert");
    ImGui::LabelText(std::to_string(solutionNodes).c_str(), u8"Lösungpfad");

    ImGui::NewLine();

    ImGui::Text(u8"Zurück zu:");
    if (ImGui::Button("Suchproblem")) {
        nextState = VisualizationState::ConfiguringSearchProblem;
        config->step = 0;
        config->maxSteps = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button("Suchumgebung")) {
        nextState = VisualizationState::ConfiguringSearchEnvironment;
        config->step = 0;
        config->maxSteps = 0;
    }

    ImGui::NewLine();
    ImGui::LabelText(std::to_string(config->seed).c_str(), u8"Seed");

    ImGui::End();
    return nextState;
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
    ImGui::PopFont();

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

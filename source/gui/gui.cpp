#include "gui/gui.h"
#include <algorithm>
#include "infrastructure/window/window.h"
#include "infrastructure/opengl/framebuffer.h"
#include "model/state_grid.h"
#include "configurations/environment_configuration.h"
#include "configurations/playback_configuration.h"
#include "configurations/stategrid_configuration.h"
#include "configurations/problem_configuration.h"
#include "seed_generator.h"
#include "model/a_star_search.h"
#include "model/node.h"

GUI::GUI() : font(nullptr), windowFlags(ImGuiWindowFlags()), dockingFlags(ImGuiDockNodeFlags()) {}

void GUI::init(const Window& window) {
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
    windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
    dockingFlags = ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_PassthruCentralNode;
}

bool GUI::isWindowExited() {
    bool exited = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            exited = true;
        }
    }
    return exited;
}

void GUI::startFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGui::PushFont(font);
    ImGui::DockSpaceOverViewport(dockingFlags);
}

VisualizationState GUI::showUI_EnvironmentConfig(EnvironmentConfig& envConfig) const {
    VisualizationState nextState = VisualizationState::ConfiguringSearchEnvironment;

    //Suchraum
    ImGui::Begin("Konfiguration des Suchraums", nullptr, windowFlags);

    char buffer[4];
    sprintf_s(buffer, "%d", envConfig.seed);
    ImGui::Text("Seed");
    if (ImGui::InputText("##seed", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll) || ImGui::IsItemDeactivatedAfterEdit()) {
        envConfig.seed = static_cast<unsigned char>(std::clamp(atoi(buffer), 0, 255));
    }
    if (ImGui::Button(u8"Zufällig")) {
        envConfig.seed = SeedGenerator::getRandomSeed();
    }

    ImGui::NewLine();

    ImGui::Text(u8"Geländetyp");
    ImGui::RadioButton("1", &envConfig.topographyType, 0); ImGui::SameLine();
    ImGui::RadioButton("2", &envConfig.topographyType, 1);

    ImGui::NewLine();

    ImGui::Text(u8"Komplexität");
    if (ImGui::Button("Einfach")) {
        envConfig.terrainScaling = EnvironmentConfig::COMPLEXITY_LOW;
    }

    if (ImGui::Button("Mittel")) {
        envConfig.terrainScaling = EnvironmentConfig::COMPLEXITY_MEDIUM;
    }     

    if (ImGui::Button("Hoch")) {
        envConfig.terrainScaling = EnvironmentConfig::COMPLEXITY_HIGH;
    }

    ImGui::NewLine();

    ImGui::Text(u8"Anzahl Gitterpunkte (n x n)");
    if (ImGui::InputInt("##graphsize", &envConfig.gridSize)) {
        envConfig.gridSize = std::clamp(envConfig.gridSize, 5, 30);
        envConfig.updateStateSpacing();
    }

    ImGui::NewLine();

    if (ImGui::Button("Fertig")) nextState = VisualizationState::ConfiguringSearchProblem;

    ImGui::End();

    return nextState;
}

VisualizationState GUI::showUI_SearchProblemConfig(ProblemConfig& problemConfig, const EnvironmentConfig& envConfig) const {
    VisualizationState nextState = VisualizationState::ConfiguringSearchProblem;

    // Suchproblem
    ImGui::Begin("Konfiguration des Suchproblems", nullptr, windowFlags);
    ImGui::Text("Startzustand"); ImGui::SameLine(125); ImGui::Text("Zielzustand");
    ImGui::NewLine();

    // Startzustand Konfiguration
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##up1", ImGuiDir_Up)) {
        problemConfig.initial.y = std::clamp(
            problemConfig.initial.y - envConfig.stateSpacing, 
            0, 
            (envConfig.gridSize - 1) * envConfig.stateSpacing);
    }

    ImGui::BeginGroup();
    if (ImGui::ArrowButton("##left1", ImGuiDir_Left)) {
        problemConfig.initial.x = std::clamp(
            problemConfig.initial.x - envConfig.stateSpacing,
            0,
            (envConfig.gridSize - 1) * envConfig.stateSpacing);
    }
    ImGui::SameLine(0, 30);
    if (ImGui::ArrowButton("##right1", ImGuiDir_Right)) {
        problemConfig.initial.x = std::clamp(
            problemConfig.initial.x + envConfig.stateSpacing,
            0,
            (envConfig.gridSize - 1) * envConfig.stateSpacing);
    }
    ImGui::EndGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##down1", ImGuiDir_Down)) {
        problemConfig.initial.y = std::clamp(
            problemConfig.initial.y + envConfig.stateSpacing,
            0,
            (envConfig.gridSize - 1) * envConfig.stateSpacing);
    }
    ImGui::EndGroup();

    ImGui::SameLine(0, 50);

    // Zielzustand Konfiguration
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##up2", ImGuiDir_Up)) {
        problemConfig.goal.y = std::clamp(
            problemConfig.goal.y - envConfig.stateSpacing,
            0,
            (envConfig.gridSize - 1) * envConfig.stateSpacing);
    }
    ImGui::BeginGroup();
    if (ImGui::ArrowButton("##left2", ImGuiDir_Left)) {
        problemConfig.goal.x = std::clamp(
            problemConfig.goal.x - envConfig.stateSpacing,
            0,
            (envConfig.gridSize - 1) * envConfig.stateSpacing);
    }
    ImGui::SameLine(0, 30);
    if (ImGui::ArrowButton("##right2", ImGuiDir_Right)) {
        problemConfig.goal.x = std::clamp(
            problemConfig.goal.x + envConfig.stateSpacing,
            0,
            (envConfig.gridSize - 1) * envConfig.stateSpacing);
    }
    ImGui::EndGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
    if (ImGui::ArrowButton("##down2", ImGuiDir_Down)) {
        problemConfig.goal.y = std::clamp(
            problemConfig.goal.y + envConfig.stateSpacing,
            0,
            (envConfig.gridSize - 1) * envConfig.stateSpacing);
    }
    ImGui::EndGroup();

    ImGui::NewLine();

    // heuristics
    const char* items[] = { "Keine", "Wanderdauer", 
        u8"Wanderdauer Überschätzt", "Schnittpunkte", u8"Höhengewichtung"};

    ImGui::Combo("Heuristik", &problemConfig.heuristic, items, IM_ARRAYSIZE(items));

    if (problemConfig.heuristic == 2 && 
        ImGui::InputFloat("Faktor", &problemConfig.overestimateFactor, 0.1f, 1.0f)) 
    {
        problemConfig.overestimateFactor = std::clamp(problemConfig.overestimateFactor, 0.0f, 100.0f);
    }

    if (ImGui::Button("Suche!", ImVec2(80.0f, 40.0f))) nextState = VisualizationState::Searching;

    ImGui::End();

    return nextState;
}

VisualizationState GUI::showUI_Searching(PlaybackConfig& playbackConfig) const {
    VisualizationState nextState = VisualizationState::Searching;

    ImGui::Begin("Suche", nullptr, windowFlags);

    ImGui::Text("Expansionen pro Sekunde");

    if (ImGui::InputInt("##expansionsPerSecond", &playbackConfig.searchRate, 1, 5)) {
        playbackConfig.searchRate = std::clamp(playbackConfig.searchRate, 0, 60);
    }

    ImGui::SameLine();

    if (ImGui::ArrowButton("Play", ImGuiDir_Right)) {
        playbackConfig.searchPlaying = true;
    }

    if (ImGui::Button("Pause")) {
        playbackConfig.searchPlaying = false;
    }

    ImGui::NewLine();

    if (playbackConfig.searchPlaying) ImGui::BeginDisabled();
    ImGui::Text("Expansion");
    if (ImGui::InputInt("##expansion", &playbackConfig.step) || playbackConfig.searchPlaying) {
        playbackConfig.step = std::clamp(playbackConfig.step, 0, playbackConfig.maxSteps);
    }
    if (playbackConfig.searchPlaying) ImGui::EndDisabled();

    if (playbackConfig.step == playbackConfig.maxSteps) {
        playbackConfig.searchPlaying = false;
        ImGui::OpenPopup("Pfad gefunden!"); //open a pop up when the search is finished
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

VisualizationState GUI::showUI_Finished(const AStarSearch& aStar) const {
    VisualizationState nextState = VisualizationState::Finished;

    ImGui::Begin("Statistik", nullptr, windowFlags);
    char sTime[100];
    sprintf_s(sTime, "%.2fh", aStar.getSolution()->pathCost/ 3600.0f); // show travel time in hours
    ImGui::LabelText(sTime, u8"Wanderdauer");

    ImGui::NewLine();

    ImGui::Text("Knoten");
    ImGui::LabelText(std::to_string(aStar.consideredNodes).c_str(), u8"Betrachtet");
    ImGui::LabelText(std::to_string(aStar.allExpanded.size()).c_str(), "Expandiert");
    ImGui::LabelText(std::to_string(aStar.solutionPath.size()).c_str(), u8"Lösungpfad");

    ImGui::NewLine();

    ImGui::Text(u8"Zurück zu:");
    if (ImGui::Button("Suchproblem")) {
        nextState = VisualizationState::ConfiguringSearchProblem;
    }
    ImGui::SameLine();
    if (ImGui::Button("Suchumgebung")) {
        nextState = VisualizationState::ConfiguringSearchEnvironment;
    }

    ImGui::End();
    return nextState;
}

void GUI::showUI_Viewport(FrameBuffer* fb) {
    // viewport
    ImGui::Begin("Visualisierung", nullptr, windowFlags);
    {
        ImGui::BeginChild("Viewport");

        viewportSize = ImGui::GetContentRegionAvail();

        ImGui::Image((ImTextureID)(intptr_t)fb->colorTextureId, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }
    ImGui::End();
}

void GUI::showUI_Visibility(StategridConfig& gridConfig) const {
    ImGui::Begin("Sichtbarkeit", nullptr, windowFlags);
    ImGui::Checkbox("Unbesucht", &gridConfig.defaultVisible);
    ImGui::Checkbox("Grenzbereich", &gridConfig.frontierVisible);
    ImGui::Checkbox("Reached", &gridConfig.reachedVisible);
    ImGui::End();
}

void GUI::render() const {
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

ImVec2 GUI::getViewportSize() const {
    return viewportSize;
}

void GUI::quit() {
    delete font;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

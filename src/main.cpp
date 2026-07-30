#include <raylib.h>
#include <cmath>
#include <vector>
#include "imgui.h"
#include "rlImGui.h"
#include "Epicycle.hpp"

// Load functions
void LoadPreset(std::vector<Epicycle> &epicycles, int selectedOption, Vector2 center);

int main()
{
    // Window size
    const int SCREENWIDTH = 1600;
    const int SCREENHEIGHT = 900;
    const int GUIWIDTH = 500;

    // Init window
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Fourier series - Epicycles");

    SetTargetFPS(60);

    // Init rlImGui
    rlImGuiSetup(true);

    // Load font
    ImGuiIO &io = ImGui::GetIO();
    const char *fontPath = "../../assets/fonts/RobotoSlab-Black.ttf";

    if (FileExists(fontPath))
        io.FontDefault = io.Fonts->AddFontFromFileTTF(fontPath, 25.0f);
    else
        TraceLog(LOG_ERROR, "IMPOSSIBLE DE TROUVER LA POLICE AU CHEMIN INDIQUE !");

    // Vars
    Vector2 center = {(SCREENWIDTH + GUIWIDTH) / 2, SCREENHEIGHT / 2};

    // Drop down menu variables
    const char *options[] = {"Square", "Triangle", "7 branches star", "Perfect heart"};
    int selectedOption = 0;
    int previousSelectedOption = -1;

    // Path and epicycles
    std::vector<Vector2> path;
    std::vector<Epicycle> epicycles;

    // Number of epicycles
    int circlesNumber = 0;

    // Input formulas
    char formulaX[256] = "cos(t)";
    char formulaY[256] = "sin(t)";

    while (!WindowShouldClose())
    {
        // Test if the selected option has changed
        if (selectedOption != previousSelectedOption)
        {
            previousSelectedOption = selectedOption;
            epicycles.clear();
            path.clear();

            // Change preset based on the selected option
            LoadPreset(epicycles, selectedOption, center);
        }

        // Update everything in time
        float deltaTime = GetFrameTime();
        for (int i = 0; i < epicycles.size(); i++)
        {
            if (i != 0)
                epicycles[i].SetCenter(epicycles[i - 1].GetTipPosition());
            else
                epicycles[i].SetCenter(center);

            epicycles[i].Update(deltaTime);
        }

        // Fill path points list
        path.push_back(epicycles.back().GetTipPosition());

        // Start of drawing
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        // Begin GUI drawing
        rlImGuiBegin();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(GUIWIDTH, SCREENHEIGHT));

        ImGui::Begin("Presets", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        // Drop down menu
        if (ImGui::BeginCombo("Options", options[selectedOption]))
        {
            for (int i = 0; i < IM_ARRAYSIZE(options); i++)
            {
                bool isSelected = (selectedOption == i);
                if (ImGui::Selectable(options[i], isSelected))
                    selectedOption = i;

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // User expression input
        ImGui::Text("Enter you time formulas :");

        // X axis
        ImGui::InputText("x(t)", formulaX, sizeof(formulaX));

        // Y axis
        ImGui::InputText("y(t)", formulaY, sizeof(formulaY));

        // Generate epicycles
        if (ImGui::Button("Generate epicycles"))
        {
            TraceLog(LOG_INFO, "X formula ready : %s", formulaX);
            TraceLog(LOG_INFO, "Y formula ready : %s", formulaY);
        }

        ImGui::End();

        // Draw circles
        for (int i = 0; i < epicycles.size(); i++)
            epicycles[i].Draw();

        // Draw path
        DrawLineStrip(path.data(), (int)path.size(), DARKBLUE);

        // End GUI drawing
        rlImGuiEnd();

        // End of drawing
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}

void LoadPreset(std::vector<Epicycle> &epicycles, int selectedOption, Vector2 center)
{
    int circlesNumber, n = 0;
    float radius, speed = 0.0f;

    switch (selectedOption)
    {
    case 0: // Square
        circlesNumber = 200;

        for (int i = 0; i < circlesNumber; i++)
        {
            int k = i / 2;

            n = (i % 2 == 0) ? (4 * k + 1) : -(4 * k + 3);

            float fn = (float)n;
            radius = 400.0f / (fn * fn);

            speed = 1.0f * fn;

            epicycles.push_back(Epicycle(center, radius, PI / 4.0f, speed));
        }
        break;
    case 1: // Triangle
        circlesNumber = 100;

        for (int i = 0; i < circlesNumber; i++)
        {
            int k = i / 2;

            n = (i % 2 == 0) ? (3 * k + 1) : -(3 * k + 2);

            float fn = (float)n;

            radius = 250.0f / (fn * fn);

            speed = 1.0f * fn;

            epicycles.push_back(Epicycle(center, radius, PI / 4.0f, speed));
        }
        break;
    case 2: // 7 branches star
        epicycles.push_back(Epicycle(center, 250.0f, 0, 1.0f));
        epicycles.push_back(Epicycle(center, 175.0f, 0, -6.0f));
        break;
    case 3: // Perfect heart
        epicycles.push_back(Epicycle(center, 250.0f, -PI / 2.0f, 1.0f));
        epicycles.push_back(Epicycle(center, 10.0f, -PI / 2.0f, -1.0f));
        epicycles.push_back(Epicycle(center, 50.0f, PI / 2.0f, 2.0f));
        epicycles.push_back(Epicycle(center, 50.0f, PI / 2.0f, -2.0f));
        epicycles.push_back(Epicycle(center, 60.0f, PI / 2.0f, 3.0f));
        epicycles.push_back(Epicycle(center, 20.0f, -PI / 2.0f, -3.0f));
        epicycles.push_back(Epicycle(center, 10.0f, PI / 2.0f, 4.0f));
        epicycles.push_back(Epicycle(center, 10.0f, PI / 2.0f, -4.0f));
        break;
    }
}
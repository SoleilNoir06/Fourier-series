#include <raylib.h>
#include <cmath>
#include <vector>
#include <cstring>
#include "imgui.h"
#include "rlImGui.h"
#include "Epicycle.hpp"
#include "MathParser.hpp"

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
    const char *fontPath = "assets/fonts/RobotoSlab-Black.ttf";

    if (FileExists(fontPath))
        io.FontDefault = io.Fonts->AddFontFromFileTTF(fontPath, 25.0f);
    else
        TraceLog(LOG_ERROR, "IMPOSSIBLE DE TROUVER LA POLICE AU CHEMIN INDIQUE !");

    // Vars
    Vector2 center = {(SCREENWIDTH + GUIWIDTH) / 2, SCREENHEIGHT / 2};

    // Drop down menu variables
    const char *options[] = {"Perfect circle", "Heart", "Star", "Lissajous curve"};
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

            // Update input field with the selected preset formulas
            switch (selectedOption)
            {
            case 0: // Circle
                strncpy(formulaX, "cos(t)", sizeof(formulaX));
                strncpy(formulaY, "sin(t)", sizeof(formulaY));
                break;
            case 1: // Heart
                strncpy(formulaX, "16 * sin(t)^3", sizeof(formulaX));
                strncpy(formulaY, "-(13 * cos(t) - 5 * cos(2*t) - 2 * cos(3*t) - cos(4*t))", sizeof(formulaY));
                break;
            case 2: // Star
                strncpy(formulaX, "cos(t) + 0.5 * cos(-4*t)", sizeof(formulaX));
                strncpy(formulaY, "sin(t) + 0.5 * sin(-4*t)", sizeof(formulaY));
                break;
            case 3: // Lissajous
                strncpy(formulaX, "sin(3*t)", sizeof(formulaX));
                strncpy(formulaY, "sin(2*t)", sizeof(formulaY));
                break;
            }

            // Security: Ensure null-termination of the strings
            formulaX[sizeof(formulaX) - 1] = '\0';
            formulaY[sizeof(formulaY) - 1] = '\0';

            // Automatically generate epicycles based on the selected preset formulas
            epicycles.clear();
            path.clear();

            std::vector<Point2D> points = GeneratePathFromFormulas(formulaX, formulaY, 250);

            if (!points.empty())
            {
                std::vector<EpicycleData> dftData = ComputeDFT(points);
                for (size_t i = 0; i < dftData.size(); i++)
                {
                    if (epicycles.size() > 250)
                        epicycles.erase(epicycles.begin());

                    epicycles.push_back(Epicycle(center, dftData[i].radius, dftData[i].angle, dftData[i].speed));
                }
            }
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
            epicycles.clear();
            path.clear();

            // Get 2D points from user formulas
            std::vector<Point2D> points = GeneratePathFromFormulas(formulaX, formulaY, 1000);

            // Compute DFT
            if (!points.empty())
            {
                std::vector<EpicycleData> dftData = ComputeDFT(points);

                // On convertit les données neutres en vrais objets Epicycle !
                for (size_t i = 0; i < dftData.size(); i++)
                {
                    epicycles.push_back(Epicycle(center, dftData[i].radius, dftData[i].angle, dftData[i].speed));
                }

                path.clear();
            }
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
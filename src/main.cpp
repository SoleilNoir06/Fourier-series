#include <raylib.h>
#include <cmath>
#include <vector>
#include "imgui.h"
#include "rlImGui.h"
#include "Epicycle.hpp"

int main()
{
    // Window size
    
    const int SCREENWIDTH = 1600;
    const int SCREENHEIGHT = 900;
    const int GUIWIDTH = 500;

    // Init window
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Fourier series - Epicycles");

    // Init rlImGui
    rlImGuiSetup(true);

    // Load font
    ImGuiIO &io = ImGui::GetIO();
    const char *fontPath = "../../assets/fonts/RobotoSlab-Black.ttf";

    if (FileExists(fontPath))
        io.FontDefault = io.Fonts->AddFontFromFileTTF(fontPath, 18.0f);
    else    
        TraceLog(LOG_ERROR, "IMPOSSIBLE DE TROUVER LA POLICE AU CHEMIN INDIQUE !");

    // Vars
    Vector2 center = {(SCREENWIDTH + GUIWIDTH) / 2, SCREENHEIGHT / 2};

    //Drop down menu variables
    const char *options[] =  {"Option 1", "Option 2", "Option 3"}; 
    int selectedOption = 0;

    // Path and epicycles
    std::vector<Vector2> path;
    std::vector<Epicycle> epicycles;

    // Fill epicycles vector
    int circlesNumber = 5;

    for (int i = 0; i < circlesNumber; i++)
    {
        int n = cos(i)*sin(i) >= 0 ? 2 * i + 1 : -(2 * i + 1);

        float radius = 200.0f * (4.0f / (PI * n));

        float speed = 1.0f * n;

        epicycles.push_back(Epicycle(center, radius, 0, speed));
    }

    while (!WindowShouldClose())
    {
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
#include <raylib.h>
#include <cmath>
#include <vector>
#include "imgui.h"
#include "rlImGui.h"
#include "Epicycle.hpp"

int main()
{
    // Window size
    const int SCREENWIDTH = 1960;
    const int SCREENHEIGHT = 1080;
    const int GUIWIDTH = 500;

    // Init window
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Fourier series - Epicycles");

    // // Init rlImGui
    // ImGui::CreateContext();

    // ImGuiIO &io = ImGui::GetIO();

    // const char *fontPath = "../assets/fonts/RobotoSlab-Black.ttf";

    // if (FileExists(fontPath))
    // {
    //     io.Fonts->AddFontFromFileTTF(fontPath, 18.0f);

    //     unsigned char *pixels = nullptr;
    //     int width, height;
    //     io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    // }
    // else
    // {
    //     io.Fonts->AddFontDefault();
    //     TraceLog(LOG_ERROR, "IMPOSSIBLE DE TROUVER LA POLICE AU CHEMIN INDIQUE !");
    // }

    rlImGuiSetup(true);

    // Vars
    Vector2 center = {(SCREENWIDTH + GUIWIDTH) / 2, SCREENHEIGHT / 2};

    // Path and epicycles
    std::vector<Vector2> path;
    std::vector<Epicycle> epicycles;

    // Fill epicycles vector
    int circlesNumber = 5;

    for (int i = 0; i < circlesNumber; i++)
    {
        int n = i * 2 + 1;

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
        ImGui::Text("Hello, World !");
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
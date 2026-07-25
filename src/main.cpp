#include <raylib.h>
#include <cmath>
#include <vector>
#include "Epicycle.hpp"

int main()
{
    // Window size
    const int SCREENWIDTH = 1960;
    const int SCREENHEIGHT = 1080;

    // Init window
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Fourier series - Epicycles");

    // Vars
    Vector2 center = {SCREENWIDTH / 2, SCREENHEIGHT / 2};

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
        ClearBackground(RAYWHITE);

        // Draw circles
        for (int i = 0; i < epicycles.size(); i++)
            epicycles[i].Draw();

        // Draw path
        DrawLineStrip(path.data(), (int)path.size(), DARKBLUE);

        // End of drawing
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
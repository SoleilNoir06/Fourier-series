#include <raylib.h>
#include <cmath>
#include <vector>
#include "Epicycle.hpp"

int main()
{
    const int SCREENWIDTH = 1960;
    const int SCREENHEIGHT = 1080;

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Fourier series - Epicycles");

    Vector2 center = {SCREENWIDTH / 2, SCREENHEIGHT / 2};
    Epicycle firstCircle(center, 100, 0, 2);
    Epicycle secondCircle(firstCircle.GetTipPosition(), 50, 10, 4);
    Epicycle thirdCircle(secondCircle.GetTipPosition(), 50, 10, 6);

    std::vector<Vector2> path;

    while (!WindowShouldClose())
    {
        // Update everything in time
        float deltaTime = GetFrameTime();
        firstCircle.Update(deltaTime);
        secondCircle.SetCenter(firstCircle.GetTipPosition());
        secondCircle.Update(deltaTime);
        thirdCircle.SetCenter(secondCircle.GetTipPosition());
        thirdCircle.Update(deltaTime);

        path.push_back(thirdCircle.GetTipPosition());

        // Start of drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw circle
        firstCircle.Draw();
        secondCircle.Draw();
        thirdCircle.Draw();
        DrawLineStrip(path.data(), (int)path.size(), DARKBLUE);

        // End of drawing
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
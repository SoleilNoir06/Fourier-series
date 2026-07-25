#include <raylib.h>
#include <cmath>
#include "Epicycle.hpp"

int main()
{
    const int SCREENWIDTH = 1960;
    const int SCREENHEIGHT = 1080;

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Fourier series - Epicycles");

    Vector2 center = {SCREENWIDTH / 2, SCREENHEIGHT / 2};
    Epicycle firstCircle(center, 100, 0, 2);

    float time = 0;

    while (!WindowShouldClose())
    {
        // Update everything in time
        float deltaTime = GetFrameTime();
        firstCircle.Update(deltaTime);

        // Start of drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw circle
        firstCircle.Draw();

        // End of drawing
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
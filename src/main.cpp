#include <raylib.h>
#include "Epicycle.hpp"

int main()
{
    const int SCREENWIDTH = 1080;
    const int SCREENHEIGHT = 540;

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Fourier series - Epicycles");

    Vector2 position = {SCREENWIDTH / 2, SCREENHEIGHT / 2};
    Epicycle firstCircle(position, 50, 0, 2);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRing(firstCircle.GetPosition(), firstCircle.GetRadius(), firstCircle.GetRadius() + 5, 0, 360, 2, RED);

        EndDrawing();
    }
}
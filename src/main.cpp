#include "raylib.h"
#include "file_1.hpp"
#include "file_2.hpp"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Makefile build example");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);
            func_file_1();
            func_file_2();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

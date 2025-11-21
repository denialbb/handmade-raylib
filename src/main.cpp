#include "file_1.hpp"
#include "file_2.hpp"
#include "gradient_animation.hpp"
#include "raylib.h"

int main(void) {
  InitWindow(screenWidth, screenHeight, "Handmade");
  SetTargetFPS(targetFps);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    gradientAnimation();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

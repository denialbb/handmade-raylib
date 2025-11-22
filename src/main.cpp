#include "file_1.hpp"
#include "file_2.hpp"
#include "gradient_animation.hpp"
#include "input.hpp"
#include "raylib.h"

void showFPS(void) {
  int fps = GetFPS();
  DrawText(TextFormat("FPS: %i", fps), screenWidth - 65, 5, 15, LIME);
}

// NOTE: this needs pooling at some point
void drawBullet(Texture2D texture, Vector2 origin, Vector2 direction,
                float speed) {
  static Vector2 _speed = {.x = speed, .y = speed};
  DrawTexture(texture, origin.x + _speed.x, origin.y + _speed.y, WHITE);
  _speed.x += direction.x * speed;
  _speed.y += direction.y * speed;
}

int main(void) {
  InitWindow(screenWidth, screenHeight, "Handmade");
  SetTargetFPS(targetFps);
  Vector2 origin = {.x = 0, .y = screenHeight / 2};
  int offsetx = 0;
  Vector2 direction = {.x = 1, .y = 0};
  Image img = GenImageGradientRadial(20, 20, 0.2f, RED, BLACK);
  Texture2D texture = LoadTextureFromImage(img);
  initializeInput();

  while (!WindowShouldClose()) {
    handleInput();

    BeginDrawing();
    ClearBackground(BLACK);

    drawBullet(texture, origin, direction, 10);

    showFPS();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

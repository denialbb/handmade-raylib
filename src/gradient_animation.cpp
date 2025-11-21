#include "gradient_animation.hpp"
#include <cstdint>
#include <raylib.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

struct Bitmap {
  int width;
  int height;
};

const Bitmap bitmap = {.width = 64, .height = 64};
Color color = {.r = 0, .g = 0, .b = 0, .a = 255};
int offset = 50;

int gradientAnimation(void) {
  DrawText("GradientAnimation", 100, 150, 20, BLACK);
  for (int y = 0; y < bitmap.height; ++y) {
    for (int x = 0; x < bitmap.width; ++x) {
      color.r = y;
      color.g = x;
      DrawPixel(x + offset, y, color);
    }
  }

  return 0;
}

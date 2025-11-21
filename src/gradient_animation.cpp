#include "gradient_animation.hpp"
#include "config.hpp"
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

Color color = {.r = 0, .g = 0, .b = 0, .a = 255};

int gradientAnimation(int xOffset, int yOffset) {
  for (int y = 0; y < screenHeight; ++y) {
    for (int x = 0; x < screenWidth; ++x) {
      color.b = (uint8)y + yOffset;
      color.g = (uint8)x + xOffset;
      DrawPixel(x, y, color);
    }
  }

  return 0;
}

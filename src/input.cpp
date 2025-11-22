#include "input.hpp"
#include "config.hpp"
#include <cstdint>
#include <cstdio>
#include <raylib.h>

#define XBOX_ALIAS_1 "xbox"
#define XBOX_ALIAS_2 "x-box"

bool _gamepad_available = false;
int _gamepad = 1;

void initializeInput() { // NOTE: where do we get the ID
  for (int i = 0; i < 10; i++) {
    if (!_gamepad_available && IsGamepadAvailable(i)) {
      _gamepad = i;
      _gamepad_available = true;
      printf("Gamepad: %d\n", _gamepad);
    }
  }
}

void handleInput() {
  if (int key = GetGamepadButtonPressed()) {
    printf("key: %d\n", key);
  }

  float axis_left_x = GetGamepadAxisMovement(_gamepad, GAMEPAD_AXIS_LEFT_X);
  float axis_left_y = GetGamepadAxisMovement(_gamepad, GAMEPAD_AXIS_LEFT_Y);
  float axis_right_x = GetGamepadAxisMovement(_gamepad, GAMEPAD_AXIS_RIGHT_X);
  float axis_right_y = GetGamepadAxisMovement(_gamepad, GAMEPAD_AXIS_RIGHT_Y);

  float axis_input = axis_left_x + axis_left_y + axis_right_x + axis_right_y;

  if (axis_input != 0) {
    printf("axis_left_x: %f, axis_left_y: %f, axis_right_x: %f, axis_right_y: "
           "%f\n",
           axis_left_x, axis_left_y, axis_right_x, axis_right_y);
  }
}

void rumble(float left, float right, float duration) {
  SetGamepadVibration(_gamepad, left, right, duration);
}

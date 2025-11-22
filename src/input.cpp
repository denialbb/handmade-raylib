#include "input.hpp"
#include "config.hpp"
#include <cstdint>
#include <cstdio>
#include <raylib.h>

#define XBOX_ALIAS_1 "xbox"
#define XBOX_ALIAS_2 "x-box"

// Gamepad buttons
#define UP_DPAD GAMEPAD_BUTTON_LEFT_FACE_UP;
#define RIGHT_DPAD GAMEPAD_BUTTON_LEFT_FACE_RIGHT;
#define DOWN_DPAD GAMEPAD_BUTTON_LEFT_FACE_DOWN;
#define LEFT_DPAD GAMEPAD_BUTTON_LEFT_FACE_LEFT;
#define Y_BUTTON GAMEPAD_BUTTON_RIGHT_FACE_UP;
#define B_BUTTON GAMEPAD_BUTTON_RIGHT_FACE_RIGHT;
#define A_BUTTON GAMEPAD_BUTTON_RIGHT_FACE_DOWN;
#define X_BUTTON GAMEPAD_BUTTON_RIGHT_FACE_LEFT;
#define SELECT_BUTTON GAMEPAD_BUTTON_MIDDLE_LEFT;
#define START_BUTTON GAMEPAD_BUTTON_MIDDLE_RIGHT;
#define LEFT_THUMB_BUTTON GAMEPAD_BUTTON_LEFT_THUMB;
#define RIGHT_THUMB_BUTTON GAMEPAD_BUTTON_RIGHT_THUMB;

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
        printf(
            "axis_left_x: %f, axis_left_y: %f, axis_right_x: %f, axis_right_y: "
            "%f\n",
            axis_left_x, axis_left_y, axis_right_x, axis_right_y);
    }
}

void rumble(float left, float right, float duration) {
    SetGamepadVibration(_gamepad, left, right, duration);
}

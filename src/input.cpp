#include "input.hpp"
#include "config.hpp"
#include <cstdint>
#include <cstdio>
#include <raylib.h>

#define XBOX_ALIAS_1 "xbox"
#define XBOX_ALIAS_2 "x-box"
#define PS_ALIAS "playstation"

// Gamepad buttons
int UP_DPAD = GAMEPAD_BUTTON_LEFT_FACE_UP;
int RIGHT_DPAD = GAMEPAD_BUTTON_LEFT_FACE_RIGHT;
int DOWN_DPAD = GAMEPAD_BUTTON_LEFT_FACE_DOWN;
int LEFT_DPAD = GAMEPAD_BUTTON_LEFT_FACE_LEFT;
int Y_BUTTON = GAMEPAD_BUTTON_RIGHT_FACE_UP;
int B_BUTTON = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT;
int A_BUTTON = GAMEPAD_BUTTON_RIGHT_FACE_DOWN;
int X_BUTTON = GAMEPAD_BUTTON_RIGHT_FACE_LEFT;
int SELECT_BUTTON = GAMEPAD_BUTTON_MIDDLE_LEFT;
int START_BUTTON = GAMEPAD_BUTTON_MIDDLE_RIGHT;
int LEFT_THUMB_BUTTON = GAMEPAD_BUTTON_LEFT_THUMB;
int RIGHT_THUMB_BUTTON = GAMEPAD_BUTTON_RIGHT_THUMB;

bool _gamepad_available = false;
int _gamepad = 0;
int _pressed_key = 0;

void initializeInput() { // NOTE: where do we get the ID
                         // for (int i = 0; i < 10; i++) {
    //     if (!_gamepad_available && IsGamepadAvailable(i)) {
    _gamepad = 0;
    _gamepad_available = true;
    printf("Gamepad: %d\n", _gamepad);
    //     }
    // }
}

void handleInput() {
    if ((_pressed_key = GetGamepadButtonPressed()) != 0) {
        printf("key: %d\n", _pressed_key);
    }
    if ((_pressed_key = GetKeyPressed()) != 0) {
        printf("key: %d\n", _pressed_key);
    }

    if (IsGamepadButtonPressed(_gamepad, A_BUTTON)) {
        printf("A button pressed\n");
    }
    if (IsGamepadButtonPressed(_gamepad, B_BUTTON)) {
        printf("B button pressed\n");
    }
    if (IsGamepadButtonPressed(_gamepad, X_BUTTON)) {
        printf("X button pressed\n");
    }
    if (IsGamepadButtonPressed(_gamepad, Y_BUTTON)) {
        printf("Y button pressed\n");
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

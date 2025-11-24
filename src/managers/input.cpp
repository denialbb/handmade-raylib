#include "managers/input.hpp"
#include <cstdint>
#include <cstdio>
#include <raylib.h>

#define XBOX_ALIAS_1 "xbox"
#define XBOX_ALIAS_2 "x-box"
#define PS_ALIAS "playstation"

// Gamepad buttons
const int UP_DPAD = GAMEPAD_BUTTON_LEFT_FACE_UP;
const int RIGHT_DPAD = GAMEPAD_BUTTON_LEFT_FACE_RIGHT;
const int DOWN_DPAD = GAMEPAD_BUTTON_LEFT_FACE_DOWN;
const int LEFT_DPAD = GAMEPAD_BUTTON_LEFT_FACE_LEFT;
const int Y_BUTTON = GAMEPAD_BUTTON_RIGHT_FACE_UP;
const int B_BUTTON = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT;
const int A_BUTTON = GAMEPAD_BUTTON_RIGHT_FACE_DOWN;
const int X_BUTTON = GAMEPAD_BUTTON_RIGHT_FACE_LEFT;
const int SELECT_BUTTON = GAMEPAD_BUTTON_MIDDLE_LEFT;
const int START_BUTTON = GAMEPAD_BUTTON_MIDDLE_RIGHT;
const int LEFT_THUMB_BUTTON = GAMEPAD_BUTTON_LEFT_THUMB;
const int RIGHT_THUMB_BUTTON = GAMEPAD_BUTTON_RIGHT_THUMB;

const float DEADZONE_LEFT_X = 0.1f;
const float DEADZONE_LEFT_Y = 0.1f;
const float DEADZONE_RIGHT_X = 0.1f;
const float DEADZONE_RIGHT_Y = 0.1f;

bool _gamepad_available = false;
int _gamepad = 0;
float axis_left_x, axis_left_y, axis_right_x, axis_right_y = 0;
float axis_input = 0;

bool pressedLeft() {
    return IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT) ||
           (_gamepad_available && IsGamepadButtonPressed(_gamepad, LEFT_DPAD));
}

bool pressedRight() {
    return IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT) ||
           (_gamepad_available && IsGamepadButtonPressed(_gamepad, RIGHT_DPAD));
}

bool pressedUp() {
    return IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) ||
           (_gamepad_available && IsGamepadButtonPressed(_gamepad, UP_DPAD));
}

bool pressedDown() {
    return IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN) ||
           (_gamepad_available && IsGamepadButtonPressed(_gamepad, DOWN_DPAD));
}

bool pressedStart() { return (IsGamepadButtonPressed(_gamepad, START_BUTTON)); }
bool pressedSelect() { return IsGamepadButtonPressed(_gamepad, SELECT_BUTTON); }
bool pressedInteract() { return IsGamepadButtonPressed(_gamepad, A_BUTTON); }
bool pressedBack() { return IsGamepadButtonPressed(_gamepad, B_BUTTON); }

void initializeInput() {
    if (!_gamepad_available && IsGamepadAvailable(_gamepad)) {
        _gamepad = 0;
        _gamepad_available = true;
        printf("Gamepad id: %d\n", _gamepad);
    }
}

void handleInput() {
    // NOTE: this only allows for 1 input every frame, probably bad
    // but it's good to stop conflicts maybe
    if (_gamepad_available) {
        switch (GetGamepadButtonPressed()) {
        case LEFT_DPAD: {
            printf("LEFT_DPAD\n");
            break;
        }
        case RIGHT_DPAD: {
            printf("RIGHT_DPAD\n");
            break;
        }
        case UP_DPAD: {
            printf("UP_DPAD\n");
            break;
        }
        case DOWN_DPAD: {
            printf("DOWN_DPAD\n");
            break;
        }
        case A_BUTTON: {
            rumble(0.2, 0.2, 0.1);
            printf("A_BUTTON\n");
            break;
        }
        case B_BUTTON: {
            printf("B_BUTTON\n");
            break;
        }
        case X_BUTTON: {
            printf("X_BUTTON\n");
            break;
        }
        case Y_BUTTON: {
            printf("Y_BUTTON\n");
            break;
        }
        default:
            break;
        }
    }

    axis_left_x = GetGamepadAxisMovement(_gamepad, GAMEPAD_AXIS_LEFT_X);
    axis_left_y = GetGamepadAxisMovement(_gamepad, GAMEPAD_AXIS_LEFT_Y);
    axis_right_x = GetGamepadAxisMovement(_gamepad, GAMEPAD_AXIS_RIGHT_X);
    axis_right_y = GetGamepadAxisMovement(_gamepad, GAMEPAD_AXIS_RIGHT_Y);

    if ((-DEADZONE_LEFT_X < axis_left_x && axis_left_x < 0) ||
        (0 < axis_left_x && axis_left_x < DEADZONE_LEFT_X))
        axis_left_x = 0;
    if ((-DEADZONE_LEFT_Y < axis_left_y && axis_left_y < 0) ||
        (0 < axis_left_y && axis_left_y < DEADZONE_LEFT_Y))
        axis_left_y = 0;
    if ((-DEADZONE_RIGHT_X < axis_right_x && axis_right_x < 0) ||
        (0 < axis_right_x && axis_right_x < DEADZONE_RIGHT_X))
        axis_right_x = 0;
    if ((-DEADZONE_RIGHT_Y < axis_right_y && axis_right_y < 0) ||
        (0 < axis_right_y && axis_right_y < DEADZONE_RIGHT_Y))
        axis_right_y = 0;
}

void rumble(float left, float right, float duration) {
    // TODO set a flag to stop repeating every frame
    SetGamepadVibration(_gamepad, left, right, duration);
}

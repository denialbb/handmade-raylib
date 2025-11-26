#include "managers/input.hpp"
#include <raylib.h>

namespace InputManager {

    void init() {
        // Raylib input system initializes automatically with InitWindow,
        // but we can add specific setup here if needed (e.g. mappings).
    }

    void update() {
        // Raylib handles polling internally.
    }

    bool isActionDown(GameAction action) {
        // 1. Check Gamepad (0 is the first gamepad)
        bool gamepad_active = IsGamepadAvailable(0);

        switch (action) {
            case GameAction::MOVE_UP:
                if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) return true;
                if (gamepad_active) {
                    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) return true;
                    if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) < -0.5f) return true;
                }
                break;

            case GameAction::MOVE_DOWN:
                if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) return true;
                if (gamepad_active) {
                    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) return true;
                    if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) > 0.5f) return true;
                }
                break;

            case GameAction::MOVE_LEFT:
                if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) return true;
                if (gamepad_active) {
                    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) return true;
                    if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) < -0.5f) return true;
                }
                break;

            case GameAction::MOVE_RIGHT:
                if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) return true;
                if (gamepad_active) {
                    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) return true;
                    if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) > 0.5f) return true;
                }
                break;

            case GameAction::ATTACK:
                if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_Z)) return true;
                if (gamepad_active) {
                    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) return true; // A / Cross
                    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) return true; // B / Circle
                }
                break;

            case GameAction::MUTE:
                if (IsKeyDown(KEY_M)) return true;
                if (gamepad_active) {
                    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) return true; // Start / Options
                }
                break;
        }

        return false;
    }

    bool isActionPressed(GameAction action) {
        bool gamepad_active = IsGamepadAvailable(0);

        switch (action) {
            case GameAction::MOVE_UP:
                if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) return true;
                if (gamepad_active && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) return true;
                // Axis usually handled as "Down", not "Pressed", unless we implement custom logic.
                break;

            case GameAction::MOVE_DOWN:
                if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) return true;
                if (gamepad_active && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) return true;
                break;

            case GameAction::MOVE_LEFT:
                if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) return true;
                if (gamepad_active && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) return true;
                break;

            case GameAction::MOVE_RIGHT:
                if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return true;
                if (gamepad_active && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) return true;
                break;

            case GameAction::ATTACK:
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z)) return true;
                if (gamepad_active) {
                    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) return true;
                    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) return true;
                }
                break;

            case GameAction::MUTE:
                if (IsKeyPressed(KEY_M)) return true;
                if (gamepad_active && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) return true;
                break;
        }

        return false;
    }
}
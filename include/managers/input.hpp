#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include "raylib.h"

enum class GameAction {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ATTACK, // Space / A button
    MUTE    // M / Select button
};

namespace InputManager {
    void init();
    void update(); // Poll inputs if necessary

    // Check if action button is currently held down
    bool isActionDown(GameAction action);

    // Check if action button was pressed this frame
    bool isActionPressed(GameAction action);
}

#endif // INPUT_MANAGER_HPP
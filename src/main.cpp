#include "main.hpp"

void showFPS(void) {
    int fps = GetFPS();
    DrawText(TextFormat("FPS: %i", fps), screenWidth - 65, 5, 15, LIME);
}

// NOTE: this needs pooling at some point

int main(void) {
    SetConfigFlags(
        FLAG_MSAA_4X_HINT); // Set MSAA 4X hint before windows creation
    InitWindow(screenWidth, screenHeight, "Handmade");
    SetTargetFPS(targetFps);

    initializeInput();

    initializeAudio();
    loadOST();
    playOST();

    initializeSpritesheet();

    while (!WindowShouldClose()) {
        handleInput();

        BeginDrawing();

        ClearBackground(BLACK);
        drawPlayer({300, 300});

        showFPS();

        EndDrawing();
    }

    closeAudio();
    unloadSpritesheet();
    CloseWindow();
    return 0;
}

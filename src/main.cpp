#include "main.hpp"
#include "managers/config.hpp"
#include "managers/input.hpp"
#include "managers/sprite.hpp"
#include "shaders/crt.hpp"
#include <raylib.h>
#include <string>
#include <utility>

Vector2 player_pos;
float move_speed;
double _time;
Font font, font_inverted;
Camera2D camera;
TilemapManager tilemap;
RenderTexture2D crt_target;

void showFPS(void) {
    int fps = GetFPS();
    DrawText(TextFormat("FPS: %i", fps), screenWidth - 65, 5, 15, LIME);
}

int main(void) {
    SetConfigFlags(
        FLAG_MSAA_4X_HINT); // Set MSAA 4X hint before windows creation
    InitWindow(screenWidth, screenHeight, "Handmade");
    SetTargetFPS(targetFps);

    LoadConfig(ASSETS_CONFIG.c_str());

    initializeGame();

    while (!WindowShouldClose()) {
        _time = GetTime();
        Vector2 next_pos = player_pos;
        handleInput(next_pos, move_speed);
        Vector2 center = checkCollisions(next_pos);

        if (!tilemap.isSolid(center.x, center.y)) {
            player_pos = next_pos;
        }

        handleCamera();

        BeginDrawing();

        BeginTextureMode(crt_target);

        BeginMode2D(camera);
        ClearBackground(BLACK);

        tilemap.draw({0, 0});

        // TODO draw with tilemanager
        drawPlayer(player_pos);

        EndMode2D();

        EndTextureMode();

        updateShader(_time);
        renderShader();

        // Draw UI (Non-camera)
        DrawTextEx(font, "Use Arrow Keys to Move", {10, 10}, 20, 0, WHITE);

        EndDrawing();
    }

    unloadGame();
    CloseWindow();

    return 0;
}

void handleInput(Vector2 &next_pos, float move_speed) {
    // Basic Movement
    if (IsKeyDown(KEY_RIGHT))
        next_pos.x += move_speed;
    if (IsKeyDown(KEY_LEFT))
        next_pos.x -= move_speed;
    if (IsKeyDown(KEY_DOWN))
        next_pos.y += move_speed;
    if (IsKeyDown(KEY_UP))
        next_pos.y -= move_speed;
}

Vector2 checkCollisions(Vector2 next_pos) {
    // Collision Check
    // We check the center of the player to avoid getting stuck on edges
    Vector2 center;

    center.x = (int)((next_pos.x + 4) / 8);
    center.y = (int)((next_pos.y + 4) / 8);

    return center;
}

void handleCamera() {
    camera.target = player_pos;
    camera.offset = {screenWidth / 2.0f, screenHeight / 2.0f};
}

void initializeGame() {
    initializeInput();

    initializeAudio();
    loadOST();
    playOST();

    initializeSpritesheet();

    const char *font_path = asset_config.find("ui.font.path")->second.c_str();
    const char *font_inverted_path =
        asset_config.find("ui.font-inverted.path")->second.c_str();

    font = LoadFont(font_path);
    font_inverted = LoadFont(font_inverted_path);

    const char *charset_path =
        asset_config.find("charset.dungeon.path")->second.c_str();
    const char *palette_path =
        asset_config.find("palette.dungeon.path")->second.c_str();
    const int palette_count =
        std::stoi(asset_config.find("palette.dungeon.count")->second);

    const char *map_path = asset_config.find("map.1.path")->second.c_str();
    const int map_width = std::stoi(asset_config.find("map.1.width")->second);
    const int map_height = std::stoi(asset_config.find("map.1.height")->second);
    const int map_spawn_x =
        std::stoi(asset_config.find("map.1.spawn.x")->second);
    const int map_spawn_y =
        std::stoi(asset_config.find("map.1.spawn.y")->second);

    tilemap.load(map_path, palette_path, palette_count, charset_path, 8);

    camera = {};
    camera.zoom = 4.0f;

    player_pos = {map_spawn_x * 8.0f, map_spawn_y * 8.0f};
    move_speed = 0.7f;

    crt_target = loadShader();
}

void unloadGame() {
    closeAudio();
    unloadSpritesheet();
    unloadShader();
}

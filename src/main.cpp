#include "main.hpp"
#include "managers/config.hpp"
#include "managers/sprite.hpp"
#include <raylib.h>
#include <string>

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

    initializeInput();

    initializeAudio();
    loadOST();
    playOST();

    initializeSpritesheet();

    const char *font_path = asset_config.find("ui.font.path")->second.c_str();
    const char *font_inverted_path =
        asset_config.find("ui.font-inverted.path")->second.c_str();

    Font font = LoadFont(font_path);
    Font font_inverted = LoadFont(font_inverted_path);

    TilemapManager tilemap;

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

    Camera2D camera = {};
    camera.zoom = 4.0f;

    Vector2 player_pos = {map_spawn_x * 8.0f, map_spawn_y * 8.0f};
    const float move_speed = 1.0f;

    while (!WindowShouldClose()) {

        Vector2 next_pos = player_pos;

        // Basic Movement
        if (IsKeyDown(KEY_RIGHT))
            next_pos.x += move_speed;
        if (IsKeyDown(KEY_LEFT))
            next_pos.x -= move_speed;
        if (IsKeyDown(KEY_DOWN))
            next_pos.y += move_speed;
        if (IsKeyDown(KEY_UP))
            next_pos.y -= move_speed;

        // Collision Check
        // We check the center of the player to avoid getting stuck on edges
        int center_x = (int)((next_pos.x + 4) / 8);
        int center_y = (int)((next_pos.y + 4) / 8);

        if (!tilemap.isSolid(center_x, center_y)) {
            player_pos = next_pos;
        }

        // Camera follows player
        camera.target = player_pos;
        camera.offset = {screenWidth / 2.0f, screenHeight / 2.0f};

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        // Draw the map at (0,0)
        tilemap.draw({0, 0});

        // TODO draw with tilemanager
        drawPlayer(player_pos);

        EndMode2D();

        // Draw UI (Non-camera)
        DrawTextEx(font, "Use Arrow Keys to Move", {10, 10}, 20, 0, WHITE);

        EndDrawing();
    }

    closeAudio();
    unloadSpritesheet();
    // Unload is handled automatically by TilemapManager destructor,
    // but standard Raylib cleanup is needed.
    CloseWindow();

    return 0;
}

#include "main.hpp"
#include "managers/audio.hpp"
#include "managers/config.hpp"
#include "managers/input.hpp"
#include "managers/sprite.hpp"
#include "shaders/crt.hpp"
#include "shaders/crt_arcade.hpp"
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
CrtArcade crt;

// NOTE: Render at low resolution and then upcale w/ shader
const int gameWidth = 320;
const int gameHeight = 240;

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

    muteMaster();
    initializeGame();

    // low res target
    RenderTexture2D arcade_shader_target =
        LoadRenderTexture(gameWidth, gameHeight);

    // NOTE: Lottes shader does its own filtering.
    SetTextureFilter(arcade_shader_target.texture, TEXTURE_FILTER_POINT);
    crt.Init();

    while (!WindowShouldClose()) {
        _time = GetTime();
        Vector2 next_pos = player_pos;
        handleInput(next_pos, move_speed);
        Vector2 center = checkCollisions(next_pos);

        if (!tilemap.isSolid(center.x, center.y)) {
            player_pos = next_pos;
        }

        handleCamera();

        // BeginTextureMode(crt_target); // CRT_SHADER

        // NOTE: low res draw to texture
        BeginTextureMode(arcade_shader_target); // ARCADE_SHADER

        ClearBackground(BLACK);
        BeginMode2D(camera);

        tilemap.draw({0, 0});
        drawPlayer(player_pos); // TODO draw with tilemanager

        EndMode2D();      // CAMERA2D
        EndTextureMode(); // END ARCADE_SHADER

        // NOTE: high res draw to screen
        BeginDrawing();
        ClearBackground(BLACK);

        // Update Uniforms
        crt.ApplyValues((Vector2){(float)gameWidth, (float)gameHeight},
                        (Vector2){(float)screenWidth, (float)screenHeight});

        // Draw using shader
        BeginShaderMode(crt.GetShader());

        // Draw the render texture scaled up to fit the screen.
        // NOTE: sourceRec height is negative to flip the texture correctly!
        DrawTexturePro(
            arcade_shader_target.texture,
            (Rectangle){0.0f, 0.0f, (float)arcade_shader_target.texture.width,
                        (float)-arcade_shader_target.texture.height},
            (Rectangle){0.0f, 0.0f, (float)screenWidth, (float)screenHeight},
            (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

        EndShaderMode();

        // CRT SHADER
        // updateShader(_time);
        // renderShader();

        DrawFPS(screenWidth - 80, 10);
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
    if (IsKeyDown(KEY_M))
        toggleMasterMute();
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

    // NOTE: Use gameWidth/gameHeight, NOT screenWidth/screenHeight
    // texture is 320x240, the center is 160,120.
    camera.offset = (Vector2){gameWidth / 2.0f, gameHeight / 2.0f};

    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void initializeFonts() {
    const char *font_path = asset_config.find("ui.font.path")->second.c_str();
    const char *font_inverted_path =
        asset_config.find("ui.font-inverted.path")->second.c_str();

    font = LoadFont(font_path);
    font_inverted = LoadFont(font_inverted_path);
}

Vector2 initializeTilemap() {
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

    return {float(map_spawn_x), float(map_spawn_y)};
}

void initializeGame() {
    initializeInput();

    initializeAudio();
    loadOST();
    playOST();

    initializeSpritesheet();

    initializeFonts();

    Vector2 map_spawn = initializeTilemap();

    camera = {};
    camera.zoom = 1.0f;

    // TODO reference the tile size
    player_pos = {map_spawn.x * 8.0f, map_spawn.y * 8.0f};
    move_speed = 0.7f;

    crt_target = loadShader();
}

void unloadGame() {
    closeAudio();
    unloadSpritesheet();
    unloadShader();
    crt.Unload();
}

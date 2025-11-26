#include "main.hpp"
#include "managers/audio.hpp"
#include "managers/config.hpp"
#include "managers/input.hpp"
#include "managers/sprite.hpp"
#include "shaders/crt_lottes.hpp"
#include "shaders/particles.hpp"
#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <utility>

Vector2 player_pos;
float move_speed;
double _time;
Font font, font_inverted;
Camera2D camera;
TilemapManager tilemap;
RenderTexture2D crt_target;
CrtLottes shader_lotte;
ParticleExplosion fx_expl;
Vector2 smoothed_cam_pos;
float smoothed_cam_speed;

// NOTE: Render at low resolution and then upcale w/ shader
const int game_width = 320;
const int game_height = 240;

void showFPS(void) {
    int fps = GetFPS();
    DrawText(TextFormat("FPS: %i", fps), screenWidth - 65, 5, 15, LIME);
}

int main(void) {
    // SetConfigFlags(
    //     FLAG_MSAA_4X_HINT); // Set MSAA 4X hint before windows creation
    ToggleFullscreen();
    InitWindow(screenWidth, screenHeight, "Handmade");
    SetTargetFPS(targetFps);

    LoadConfig(ASSETS_CONFIG.c_str());

    muteMaster();
    initializeGame();

    fx_expl.init(tilemap.getTexture());

    // low res target
    RenderTexture2D lottes_shader_target =
        LoadRenderTexture(game_width, game_height);

    // NOTE: Lottes shader does its own filtering.
    SetTextureFilter(lottes_shader_target.texture, TEXTURE_FILTER_POINT);
    shader_lotte.Init();

    while (!WindowShouldClose()) {
        _time = GetTime();
        // float delta = GetFrameTime();
        Vector2 next_pos = player_pos;
        handleInput(next_pos, move_speed);
        Vector2 center = checkCollisions(next_pos);

        if (!tilemap.isSolid(center.x, center.y)) {
            player_pos.x = std::round(next_pos.x);
            player_pos.y = std::round(next_pos.y);
        }

        handleCamera();

        // BeginTextureMode(crt_target); // CRT_SHADER

        /////////////////////
        // NOTE: low res draw to texture
        BeginTextureMode(lottes_shader_target); // LOTTES_SHADER

        ClearBackground(BLACK);
        BeginMode2D(camera);

        tilemap.draw({0, 0});
        Vector2 draw_pos = {std::round(player_pos.x), std::round(player_pos.y)};
        drawPlayer(draw_pos, tilemap.getTexture()); // TODO draw with tilemanager

        // Draw explosion effect on top of player if active
        fx_expl.render(draw_pos);

        if (IsKeyPressed(KEY_SPACE)) {
            TraceLog(LOG_INFO, "SHADER: triggering explosion");

            // Random bright color
            Vector3 rnd_color = {(float)GetRandomValue(100, 255) / 255.0f,
                                 (float)GetRandomValue(100, 255) / 255.0f,
                                 (float)GetRandomValue(100, 255) / 255.0f};

            fx_expl.trigger(0.5f, rnd_color);
        }

        EndMode2D();      // CAMERA2D
        EndTextureMode(); // END LOTTES_SHADER

        /////////////////////
        // NOTE: high res draw to screen
        BeginDrawing();
        ClearBackground(BLACK);

        // Update Uniforms
        shader_lotte.Apply((Vector2){(float)game_width, (float)game_height},
                           (Vector2){(float)screenWidth, (float)screenHeight});

        // Draw using shader
        BeginShaderMode(shader_lotte.GetShader());
        float scale = std::min((float)screenWidth / game_width,
                               (float)screenHeight / game_height);
        scale = std::floor(scale);

        // If scale is 0 (window too small), force at least 1
        if (scale < 1.0f)
            scale = 1.0f;

        float scaled_width = game_width * scale;
        float scaled_height = game_height * scale;

        float offset_x = (screenWidth - scaled_width) * 0.5f;
        float offset_y = (screenHeight - scaled_height) * 0.5f;

        DrawTexturePro(
            lottes_shader_target.texture,
            // Source: Inverted height (OpenGL standard)
            (Rectangle){0.0f, 0.0f, (float)lottes_shader_target.texture.width,
                        (float)-lottes_shader_target.texture.height},
            (Rectangle){offset_x, offset_y, scaled_width, scaled_height},
            (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        EndShaderMode();

        if (offset_x > 0) {
            DrawRectangle(0, 0, offset_x, screenHeight, BLACK); // Left
            DrawRectangle(screenWidth - offset_x, 0, offset_x, screenHeight,
                          BLACK); // Right
        }
        if (offset_y > 0) {
            DrawRectangle(0, 0, screenWidth, offset_y, BLACK); // Top
            DrawRectangle(0, screenHeight - offset_y, screenWidth, offset_y,
                          BLACK); // Bottom
        }

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

Rectangle deadzone = {(game_width - 100) / 2.0f, (game_height - 100) / 2.0f,
                      100, 100};

void handleCamera() {
    if (player_pos.x > deadzone.x + deadzone.width) {
        deadzone.x = player_pos.x - deadzone.width;
    } else if (player_pos.x < deadzone.x) {
        deadzone.x = player_pos.x;
    }
    if (player_pos.y > deadzone.y + deadzone.height) {
        deadzone.y = player_pos.y - deadzone.height;
    } else if (player_pos.y < deadzone.y) {
        deadzone.y = player_pos.y;
    }

    camera.target.x = deadzone.x + deadzone.width / 2.0f;
    camera.target.y = deadzone.y + deadzone.height / 2.0f;

    camera.target.x = std::round(camera.target.x);
    camera.target.y = std::round(camera.target.y);

    camera.offset = (Vector2){game_width / 2.0f, game_height / 2.0f};
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

    // initializeSpritesheet(); // Removed, now using tilemap texture
    initializeFonts();

    Vector2 map_spawn = initializeTilemap();
    
    // Initialize player using the texture loaded by tilemap
    initializePlayer(tilemap.getTexture());

    // TODO reference the tile size
    player_pos = {map_spawn.x * 8.0f, map_spawn.y * 8.0f};
    camera = {};
    camera.zoom = 1.0f;
    camera.target = {player_pos.x, player_pos.y};

    move_speed = 0.7f;
}

void unloadGame() {
    closeAudio();
    // unloadSpritesheet(); // Texture owned by tilemap
    shader_lotte.Unload();
    fx_expl.unload();
}

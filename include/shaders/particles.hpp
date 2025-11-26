#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include "raylib.h"
#include "managers/sprite.hpp"

struct ParticleExplosion {
    Shader shader;
    int loc_progress;
    int loc_strength;
    int loc_color_tint;
    
    bool is_active = false;
    float progress = 0.0f;
    float strength = 1.0f;
    Vector3 color_tint = {1.0f, 1.0f, 1.0f};

    Texture2D explosion_tex;        // Dedicated texture for the explosion effect
    Rectangle explosion_source_rect; // Source rectangle for the explosion sprite

    void init() {
        shader = LoadShader(0, "assets/shaders/glsl330/particles.fs");
        loc_progress = GetShaderLocation(shader, "progress");
        loc_strength = GetShaderLocation(shader, "strength");
        loc_color_tint = GetShaderLocation(shader, "colorTint");

        // Get the spritesheet and define the source rectangle for the explosion sprite (Heart/Dot)
        explosion_tex = getSpriteSheet();                                                                                                                
        float sprite_cell_width = getSpriteWidth();                                                                                                     
        float sprite_cell_height = getSpriteHeight();

        explosion_source_rect = {
            2 * sprite_cell_width, // X-coordinate of Heart (Col 2)
            0 * sprite_cell_height, // Y-coordinate of Heart (Row 0)
            sprite_cell_width,
            sprite_cell_height
        };
    }

    void trigger(float s = 0.5f, Vector3 color = {1.0f, 1.0f, 1.0f}) {
        is_active = true;
        progress = 0.0f;
        strength = s;
        color_tint = color;
    }

    void render(Vector2 pos) {
        if (!is_active) return;

        // Update progress
        progress += GetFrameTime() * 2.0f; // Animation speed
        if (progress >= 1.0f) {
            is_active = false;
            progress = 1.0f;
        }

        // Set Uniforms
        SetShaderValue(shader, loc_progress, &progress, SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader, loc_strength, &strength, SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader, loc_color_tint, &color_tint, SHADER_UNIFORM_VEC3);

        // Draw
        Rectangle dest = {pos.x, pos.y, explosion_source_rect.width, explosion_source_rect.height};
        Vector2 origin = {0, 0};

        BeginShaderMode(shader);
        DrawTexturePro(explosion_tex, explosion_source_rect, dest, origin, 0.0f, WHITE);
        EndShaderMode();
    }

    void unload() { UnloadShader(shader); }
};

#endif

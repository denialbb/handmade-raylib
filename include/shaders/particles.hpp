#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include "raylib.h"
#include "managers/sprite.hpp"

struct ParticleExplosion {
    Shader shader;
    int locProgress;
    int locStrength;
    int locColorTint;
    
    bool active = false;
    float progress = 0.0f;
    float strength = 1.0f;
    Vector3 colorTint = {1.0f, 1.0f, 1.0f};

    Texture2D explosionTex;        // Dedicated texture for the explosion effect
    Rectangle explosionSourceRect; // Source rectangle for the explosion sprite

    void Init() {
        shader = LoadShader(0, "assets/shaders/glsl330/particles.fs");
        locProgress = GetShaderLocation(shader, "progress");
        locStrength = GetShaderLocation(shader, "strength");
        locColorTint = GetShaderLocation(shader, "colorTint");

        // Get the spritesheet and define the source rectangle for the explosion sprite (Heart/Dot)
        explosionTex = GetSpriteSheet(); 
        float sprite_cell_width = GetSpriteWidth();
        float sprite_cell_height = GetSpriteHeight();

        explosionSourceRect = {
            2 * sprite_cell_width, // X-coordinate of Heart (Col 2)
            0 * sprite_cell_height, // Y-coordinate of Heart (Row 0)
            sprite_cell_width,
            sprite_cell_height
        };
    }

    void Trigger(float s = 0.5f, Vector3 color = {1.0f, 1.0f, 1.0f}) {
        active = true;
        progress = 0.0f;
        strength = s;
        colorTint = color;
    }

    void Render(Vector2 pos) {
        if (!active) return;

        // Update progress
        progress += GetFrameTime() * 2.0f; // Animation speed
        if (progress >= 1.0f) {
            active = false;
            progress = 1.0f;
        }

        // Set Uniforms
        SetShaderValue(shader, locProgress, &progress, SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader, locStrength, &strength, SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader, locColorTint, &colorTint, SHADER_UNIFORM_VEC3);

        // Draw
        Rectangle dest = {pos.x, pos.y, explosionSourceRect.width, explosionSourceRect.height};
        Vector2 origin = {0, 0};

        BeginShaderMode(shader);
        DrawTexturePro(explosionTex, explosionSourceRect, dest, origin, 0.0f, WHITE);
        EndShaderMode();
    }

    void Unload() { UnloadShader(shader); }
};

#endif

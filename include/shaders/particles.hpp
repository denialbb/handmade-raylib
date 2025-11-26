#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include "raylib.h"

struct ParticleExplosion {
    Shader shader;

    int locTime;
    int locResolution;
    int locOrigin;

    void Init() {
        // Load only the fragment shader (pass 0/NULL for vertex shader)
        shader = LoadShader(0, "assets/shaders/glsl330/particles.fs");

        locTime = GetShaderLocation(shader, "time");
        locResolution = GetShaderLocation(shader, "resolution");
        locOrigin = GetShaderLocation(shader, "origin");
    }

    // Update: Send data to the GPU (Call this every frame)
    // w, h      : The dimensions of your GAME WORLD (e.g., 320, 240)
    // time      : GetTime()
    // spawnPos  : The position in Game World pixels (e.g., Player.x, Player.y)
    void Update(float w, float h, float time, Vector2 spawnPos) {

        float res[2] = {w, h};
        SetShaderValue(shader, locResolution, res, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, locOrigin, &spawnPos, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, locTime, &time, SHADER_UNIFORM_FLOAT);
    }

    void Unload() { UnloadShader(shader); }
};

#endif

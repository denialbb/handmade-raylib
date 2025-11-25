#include "shaders/crt.hpp"
#include "managers/config.hpp"
#include <raylib.h>
#include <string>

struct CrtParams {
    float overlay = 0.8f;
    float scanlines_opacity = 0.1f;
    float scanlines_width = 0.10f;
    float grille_opacity = 0.12f;
    float pixelate = 0.3f;
    float roll = 0.0f;
    float roll_speed = 2.5f;
    float roll_size = 0.4f;
    float roll_variation = 0.7f;
    float distort_intensity = 0.2f;
    float noise_opacity = 0.2f;
    float noise_speed = 1.6f;
    float static_noise_intensity = 0.10f;
    float aberration = 0.015f;
    float brightness = 1.3f;
    float discolor = 0.3f;
    float warp_amount = 0.8f;
    float clip_warp = 1.5f;
    float vignette_intensity = 1.0f;
    float vignette_opacity = 0.8f;
};

// Cache the locations instead of querying every frame
struct CrtLocations {
    int time;
    int resolution;
    // We map every CrtParams field to an int location here
    int scanlines_opacity;
    int scanlines_width;
    int grille_opacity;
    int pixelate;
    int roll;
    int roll_speed;
    int roll_size;
    int roll_variation;
    int distort_intensity;
    int noise_opacity;
    int noise_speed;
    int static_noise_intensity;
    int aberration;
    int brightness;
    int discolor;
    int warp_amount;
    int clip_warp;
    int vignette_intensity;
    int vignette_opacity;
};

const char *_SHADER_RESOURCE = "assets/shaders/glsl330/crt.fs";
CrtParams _params;
CrtLocations _locs;
Shader _shader;
RenderTexture2D _target;
Vector2 _screen_size;

CrtLocations initCrtLocations(Shader shader) {
    CrtLocations locs;

    // Standard inputs
    locs.time = GetShaderLocation(shader, "time");
    locs.resolution = GetShaderLocation(shader, "resolution");

    // Parameter inputs (Must match GLSL uniform names exactly)
    locs.scanlines_opacity = GetShaderLocation(shader, "scanlines_opacity");
    locs.scanlines_width = GetShaderLocation(shader, "scanlines_width");
    locs.grille_opacity = GetShaderLocation(shader, "grille_opacity");
    locs.pixelate = GetShaderLocation(shader, "pixelate");
    locs.roll = GetShaderLocation(shader, "roll");
    locs.roll_speed = GetShaderLocation(shader, "roll_speed");
    locs.roll_size = GetShaderLocation(shader, "roll_size");
    locs.roll_variation = GetShaderLocation(shader, "roll_variation");
    locs.distort_intensity = GetShaderLocation(shader, "distort_intensity");
    locs.noise_opacity = GetShaderLocation(shader, "noise_opacity");
    locs.noise_speed = GetShaderLocation(shader, "noise_speed");
    locs.static_noise_intensity =
        GetShaderLocation(shader, "static_noise_intensity");
    locs.aberration = GetShaderLocation(shader, "aberration");
    locs.brightness = GetShaderLocation(shader, "brightness");
    locs.discolor = GetShaderLocation(shader, "discolor");
    locs.warp_amount = GetShaderLocation(shader, "warp_amount");
    locs.clip_warp = GetShaderLocation(shader, "clip_warp");
    locs.vignette_intensity = GetShaderLocation(shader, "vignette_intensity");
    locs.vignette_opacity = GetShaderLocation(shader, "vignette_opacity");

    return locs;
}

// Helper to send all uniforms at once
void UpdateCrtShader(float time) {
    // 1. Update Context Data
    SetShaderValue(_shader, _locs.time, &time, SHADER_UNIFORM_FLOAT);
    float resolution[2] = {_screen_size.x, _screen_size.y};
    SetShaderValue(_shader, _locs.resolution, resolution, SHADER_UNIFORM_VEC2);

    // 2. Update Parameters using cached locations
    SetShaderValue(_shader, _locs.scanlines_opacity, &_params.scanlines_opacity,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.scanlines_width, &_params.scanlines_width,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.grille_opacity, &_params.grille_opacity,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.pixelate, &_params.pixelate,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.roll, &_params.roll, SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.roll_speed, &_params.roll_speed,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.roll_size, &_params.roll_size,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.roll_variation, &_params.roll_variation,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.distort_intensity, &_params.distort_intensity,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.noise_opacity, &_params.noise_opacity,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.noise_speed, &_params.noise_speed,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.static_noise_intensity,
                   &_params.static_noise_intensity, SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.aberration, &_params.aberration,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.brightness, &_params.brightness,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.discolor, &_params.discolor,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.warp_amount, &_params.warp_amount,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.clip_warp, &_params.clip_warp,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.vignette_intensity,
                   &_params.vignette_intensity, SHADER_UNIFORM_FLOAT);
    SetShaderValue(_shader, _locs.vignette_opacity, &_params.vignette_opacity,
                   SHADER_UNIFORM_FLOAT);
}

RenderTexture2D loadShader() {
    _screen_size = {screenWidth, screenHeight};
    _shader = LoadShader(0, _SHADER_RESOURCE);
    // Create a RenderTexture to draw the game into
    _target = LoadRenderTexture(screenWidth, screenHeight);
    _locs = initCrtLocations(_shader);

    return _target;
}

void unloadShader() {
    UnloadRenderTexture(_target);
    UnloadShader(_shader);
}

void updateShader(float time) {
    // 2. Update Shader Uniforms
    UpdateCrtShader(time);
}

void renderShader() {
    // 3. Draw the RenderTexture to the Screen using the Shader
    BeginDrawing();
    ClearBackground(BLACK);

    BeginShaderMode(_shader);
    // Note: RenderTextures are y-flipped in OpenGL, so we draw the source
    // rectangle inverted Source Rect: {0, 0, width, -height} -> This flips
    // it right-side up
    DrawTexturePro(
        _target.texture,
        (Rectangle){0.0f, 0.0f, (float)_target.texture.width,
                    (float)-_target.texture.height},
        (Rectangle){0.0f, 0.0f, (float)_screen_size.x, (float)_screen_size.y},
        (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    EndShaderMode();
}

#include "shaders/crt_arcade.hpp"

void CrtArcade::Init() {
    // Load shader (No vertex shader needed, pass 0)
    shader = LoadShader(0, "assets/shaders/glsl330/crt_arcade.fs");

    // Cache locations
    locRenderSize = GetShaderLocation(shader, "renderSize");
    locTextureSize = GetShaderLocation(shader, "textureSize");
}

void CrtArcade::Unload() { UnloadShader(shader); }

void CrtArcade::ApplyValues(Vector2 gameSize, Vector2 screenSize) {
    float gameSizeArr[2] = {gameSize.x, gameSize.y};
    float screenSizeArr[2] = {screenSize.x, screenSize.y};

    SetShaderValue(shader, locTextureSize, gameSizeArr, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, locRenderSize, screenSizeArr, SHADER_UNIFORM_VEC2);
}

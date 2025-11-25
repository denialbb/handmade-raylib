#ifndef CRT_LOTTES_HPP
#define CRT_LOTTES_HPP

#include "raylib.h"

class CrtLottes {
  public:
    void Init() {
        shader = LoadShader(0, "assets/shaders/glsl330/crt_lottes.fs");
        locRenderSize = GetShaderLocation(shader, "renderSize");
        locTextureSize = GetShaderLocation(shader, "textureSize");
    }

    void Unload() { UnloadShader(shader); }

    void Apply(Vector2 gameSize, Vector2 screenSize) {
        float gameArr[2] = {gameSize.x, gameSize.y};
        float screenArr[2] = {screenSize.x, screenSize.y};
        SetShaderValue(shader, locTextureSize, gameArr, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, locRenderSize, screenArr, SHADER_UNIFORM_VEC2);
    }

    Shader GetShader() const { return shader; }

  private:
    Shader shader;
    int locRenderSize;
    int locTextureSize;
};

#endif

#ifndef CRT_ARCADE_HPP
#define CRT_ARCADE_HPP

#include "raylib.h"

class CrtArcade {
  public:
    void Init();
    void Unload();

    // NOTE: Call this just before DrawTexturePro or EndShaderMode
    // gameSize: The resolution of your pixel art (e.g., 320x240)
    // screenSize: The resolution of the window (e.g., 1280x960)
    void ApplyValues(Vector2 gameSize, Vector2 screenSize);

    Shader GetShader() const { return shader; }

  private:
    Shader shader;
    int locRenderSize;
    int locTextureSize;
};

#endif

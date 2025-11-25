#ifndef HANDMADE_CRT_SHADER
#define HANDMADE_CRT_SHADER

#include "managers/config.hpp"
#include <raylib.h>

// Helper to send all uniforms at once
void UpdateCrtShader(float time);
RenderTexture2D loadShader();
void unloadShader();
void updateShader(float time);
void renderShader();

#endif // HANDMADE_CRT_SHADER

#ifndef HANDMADE_MAIN_H
#define HANDMADE_MAIN_H

#include "managers/audio.hpp"
#include "managers/config.hpp"
#include "managers/input.hpp"
#include "managers/sprite.hpp"
#include "managers/tilemap.hpp"
#include <raylib.h>

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // ANDROID, WEB
#define GLSL_VERSION 100
#endif

void handleInput(Vector2 &next_pos, float move_speed);
Vector2 checkCollisions(Vector2 next_pos);
void handleCamera();
void initializeGame();
void unloadGame();

#endif // HANDMADE_MAIN_H

#ifndef HANDMADE_SPRITE_H
#define HANDMADE_SPRITE_H

#include "managers/config.hpp"
#include <cstdio>
#include <raylib.h>

void initializeSpritesheet();
void unloadSpritesheet();
void drawPlayer(Vector2 position);

// Expose internal sprite data for effects
Texture2D GetSpriteSheet();
Rectangle GetPlayerRect();
float GetSpriteWidth();
float GetSpriteHeight();

#endif // HANDMADE_SPRITE_H

#ifndef HANDMADE_SPRITE_H
#define HANDMADE_SPRITE_H

#include "managers/config.hpp"
#include <cstdio>
#include <raylib.h>

void initializeSpritesheet();
void unloadSpritesheet();
void drawPlayer(Vector2 position);

// Expose internal sprite data for effects
Texture2D getSpriteSheet();
Rectangle getPlayerRect();
float getSpriteWidth();
float getSpriteHeight();

#endif // HANDMADE_SPRITE_H

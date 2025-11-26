#ifndef HANDMADE_SPRITE_H
#define HANDMADE_SPRITE_H

#include "managers/config.hpp"
#include <cstdio>
#include <raylib.h>

void initializePlayer(Texture2D texture);
void drawPlayer(Vector2 position, Texture2D texture);

// Expose internal sprite data for effects
Rectangle getPlayerRect();
float getSpriteWidth();
float getSpriteHeight();

#endif // HANDMADE_SPRITE_H

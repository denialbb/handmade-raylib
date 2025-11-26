#ifndef HANDMADE_GHOST_H
#define HANDMADE_GHOST_H
#include <raylib.h>

void initGhostSystem(Texture2D tex, Color start, Color end);
void updateGhosts(float dt);
// add ghost when moving or dashing etc
void addGhost(Vector2 pos, Rectangle rec, float rot);
void drawGhosts();

#endif // HANDMADE_GHOST_H

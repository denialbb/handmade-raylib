#ifndef HANDMADE_INPUT_H
#define HANDMADE_INPUT_H

#include <cstdint>
#include <cstdio>
#include <raylib.h>

void initializeInput();
void handleInput();
void rumble(float left, float right, float duration);

#endif // HANDMADE_INPUT_H

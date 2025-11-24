#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "raylib.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <locale>
#include <map>
#include <string>

// Screen
constexpr int screenWidth = 1280;
constexpr int screenHeight = 720;

// Game
constexpr int targetFps = 60;
constexpr float playerSpeed = 300.0f; // pixels per second

extern std::string ASSETS_CONFIG;

extern std::map<std::string, std::string> asset_config;

void LoadConfig(const char *fileName);
void UnloadConfig();

#endif // CONFIG_MANAGER_H

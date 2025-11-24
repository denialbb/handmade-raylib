#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

struct GameConfig {
    // audio
    char *ost_path;

    // sprites
    char *spritesheet_path;

    // player
    int player_sprite_id_x;
    int player_sprite_id_y;
    int player_width;
    int player_height;
};

extern GameConfig config;

void LoadConfig(const char *fileName);
void UnloadConfig();

#endif // CONFIG_MANAGER_H

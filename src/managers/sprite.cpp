#include "managers/sprite.hpp"
#include <cstdio>
#include <raylib.h>

// RLAPI Texture2D LoadTexture(const char *fileName);
// RLAPI bool IsTextureValid(Texture2D texture);
// RLAPI void UnloadRenderTexture(RenderTexture2D target);
// RLAPI void DrawTextureV(Texture2D texture, Vector2 position, Color tint);
// RLAPI Image ImageFromImage(Image image, Rectangle rec);
// RLAPI void DrawTextureRec(Texture2D texture, Rectangle source, Vector2
// position, Color tint);

// NOTE: spritesheet is 16x16
// path is relative to executable (cwd)
const char *_SPRITESHEET = "assets/sprites/dungeonmode/bitmap/dungeon-mode.png";
const int _SPRITESHEET_COLS = 16;
const int _SPRITESHEET_ROWS = 16;
Texture2D _spritesheet;
float _sprite_width, _sprite_height;

struct Sprite {
    Vector2 position;
    float rotation;
    Rectangle size;
    Vector2 ID;            // this is the position in the spritesheet
    Rectangle sprite_rect; // actual sprite position
    Color tint;
};

Sprite _player_sprite = {
    .position = {0, 0},
    .rotation = 0,
    .size = {0, 0, 32, 32},
    .ID = {14, 14},
    .sprite_rect = {0, 0, 0, 0},
    .tint = WHITE,
};

void initializeSpritesheet() {
    _spritesheet = LoadTexture(_SPRITESHEET);
    if (IsTextureValid(_spritesheet)) {
        printf("spritesheet is valid\n");
    } else {
        printf("spritesheet is not valid\n");
        return;
    }
    _sprite_width = _spritesheet.width / _SPRITESHEET_COLS;
    _sprite_height = _spritesheet.height / _SPRITESHEET_ROWS;
    _player_sprite.sprite_rect = {_player_sprite.ID.x, _player_sprite.ID.y,
                                  _sprite_width, _sprite_height};
}

void drawPlayer(Vector2 position) {
    _player_sprite.position = position;

    DrawTexturePro(_spritesheet, _player_sprite.sprite_rect,
                   _player_sprite.size, position, _player_sprite.rotation,
                   _player_sprite.tint);
}

void unloadSpritesheet() { UnloadTexture(_spritesheet); }

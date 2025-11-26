#include "managers/sprite.hpp"
#include "managers/config.hpp"
#include <string>

// NOTE: spritesheet is 16x16
// path is relative to executable (cwd)
int _spritesheet_cols = 16;
int _spritesheet_rows = 16;
Texture2D _spritesheet;
float _sprite_width, _sprite_height;

struct Sprite {
    int spritesheet;
    Vector2 position;
    float rotation;
    Rectangle size;
    Vector2 ID;            // this is the position in the spritesheet
    Rectangle sprite_rect; // actual sprite position
    Color tint;
};

Sprite _player_sprite = {};
void initializePlayerSprite();

void initializeSpritesheet() {
    const std::string key_cols = "sprites.sheet.1.cols";
    const std::string key_rows = "sprites.sheet.1.rows";
    const std::string key_path = "sprites.sheet.1.path";

    _spritesheet_cols = std::stoi(asset_config.find(key_cols)->second);
    _spritesheet_rows = std::stoi(asset_config.find(key_rows)->second);
    const char *spritesheet_path = asset_config.find(key_path)->second.c_str();

    printf("Loading spritesheet from: %s\n", spritesheet_path);

    _spritesheet = LoadTexture(spritesheet_path);
    if (IsTextureValid(_spritesheet)) {
        printf("spritesheet is valid\n");
    } else {
        printf("spritesheet is not valid\n");
        return;
    }

    _sprite_width = _spritesheet.width / _spritesheet_cols;
    _sprite_height = _spritesheet.height / _spritesheet_rows;

    initializePlayerSprite();
}

void initializePlayerSprite() {
    const std::string key_sheet = "sprites.player.sheet";
    const std::string key_x = "sprites.player.sprite_id_x";
    const std::string key_y = "sprites.player.sprite_id_y";
    const std::string key_width = "sprites.player.width";
    const std::string key_height = "sprites.player.height";

    _player_sprite.spritesheet =
        std::stoi(asset_config.find(key_sheet)->second);
    
    _player_sprite.ID.x = std::stoi(asset_config.find(key_x)->second);
    _player_sprite.ID.y = std::stoi(asset_config.find(key_y)->second);

    _player_sprite.size.width = std::stoi(asset_config.find(key_width)->second);
    _player_sprite.size.height =
        std::stoi(asset_config.find(key_height)->second);

    _player_sprite.sprite_rect = {_player_sprite.ID.x * _sprite_width,
                                  _player_sprite.ID.y * _sprite_height,
                                  _sprite_width, _sprite_height};
    _player_sprite.tint = WHITE;
}

void drawPlayer(Vector2 position) {
    _player_sprite.position = position;

    // destination on the screen
    Rectangle destRec = {position.x, position.y, _player_sprite.size.width,
                         _player_sprite.size.height};

    // origin for rotation
    Vector2 origin = {0, 0};

    DrawTexturePro(_spritesheet, _player_sprite.sprite_rect, destRec, origin,
                   _player_sprite.rotation, _player_sprite.tint);
}

void unloadSpritesheet() { UnloadTexture(_spritesheet); }

Texture2D GetSpriteSheet() { return _spritesheet; }
Rectangle GetPlayerRect() { return _player_sprite.sprite_rect; }
float GetSpriteWidth() { return _sprite_width; }
float GetSpriteHeight() { return _sprite_height; }

#pragma once

#include "raylib.h"
#include <set>
#include <string>
#include <vector>

// ==========================================
// Data Structures
// ==========================================

struct Tile {
    int bg;      // Background color index
    int fg;      // Foreground color index
    int char_id; // ASCII character index
    int xform;   // Transformation flags
};

struct Layer {
    std::string name;
    std::vector<Tile> tiles;
    bool visible; // visibility flag
    float z;      // z-layer
};

struct PlaysciiMap {
    int width;
    int height;
    std::vector<Layer> layers;
};

// ==========================================
// Tilemap Manager Class
// ==========================================

class TilemapManager {
  public:
    bool draw_collisions = false;
    TilemapManager();
    ~TilemapManager();

    // Loads Map (.psci), Palette (.png), and Tileset (.png)
    // tile_size: pixel size of one tile (e.g., 8 for 8x8, 16 for 16x16)
    void load(const char *map_path, const char *palette_path,
              const int palette_size, const char *tileset_path,
              int tile_size = 8);

    void unload();
    void draw(Vector2 offset);

    // Checks collision.
    // If a "Collision" layer exists, checks that.
    // Otherwise, checks if the character at (x,y) is in the 'solid_chars' list.
    bool isSolid(int grid_x, int grid_y);

    const PlaysciiMap &getMapData() const;
    Texture2D getTexture() const { return _tileset_texture; }

  private:
    PlaysciiMap _current_map;
    std::vector<Color> _current_palette;
    Texture2D _tileset_texture;
    int _tile_size;

    // Fallback list of characters considered solid (walls)
    std::set<int> _solid_chars;

    void drawLayer(const Layer &layer, Vector2 offset);
};

void drawBackground(const Tile &t, const Vector2 pos,
                    const std::vector<Color> &palette, const int tile_size);

void drawForeground(const Texture2D &tileset, const Tile &t, const Vector2 pos,
                    const std::vector<Color> &palette, const int tile_size,
                    const int tiles_per_row);

void drawChar(const Texture2D &tileset, const Tile &t, const Vector2 pos,
              const std::vector<Color> &palette, const int tile_size,
              const int tiles_per_row);

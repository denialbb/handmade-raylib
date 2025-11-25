#include "managers/tilemap.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <raylib.h>

using json = nlohmann::json;

// ==========================================
// JSON Serialization Logic
// ==========================================

void from_json(const json &j, Tile &t) {
    // Default values prevent crashes if keys are missing
    t.bg = j.value("bg", 0) - 1;
    t.fg = j.value("fg", 0) - 1;
    t.char_id = j.value("char", 0);
    t.xform = j.value("xform", 0);
}

void from_json(const json &j, Layer &l) {
    l.name = j.value("name", "Layer");
    l.visible = j.value("visible", 1); // Default to visible
    l.z = j.value("z", 0);             // Default to base
    j.at("tiles").get_to(l.tiles);
}

void from_json(const json &j, PlaysciiMap &m) {
    m.width = j.value("width", 16);
    m.height = j.value("height", 16);
    j.at("frames")[0]["layers"].get_to(m.layers);
}

// ==========================================
// Manager Implementation
// ==========================================

TilemapManager::TilemapManager() {
    _tileset_texture = {};
    _tile_size = 8;

    // Define standard Box-Drawing characters as solid
    _solid_chars = {
        178, // Solid block
    };
}

TilemapManager::~TilemapManager() { unload(); }

void TilemapManager::load(const char *map_path, const char *palette_path,
                          const int palette_size, const char *tileset_path,
                          int tile_size) {
    _tile_size = tile_size;

    std::ifstream f(map_path);
    if (!f.is_open()) {
        TraceLog(LOG_ERROR, "TILEMAP: Could not open map file %s", map_path);
        return;
    }

    json data = json::parse(f);
    data.get_to(_current_map);

    TraceLog(LOG_INFO, "TILEMAP: Loaded %dx%d map with %lu layers",
             _current_map.width, _current_map.height,
             _current_map.layers.size());

    Image pal_img = LoadImage(palette_path);
    if (pal_img.data) {
        // palette file is a strip
        int step_x = pal_img.width / palette_size;
        if (step_x < 1)
            step_x = 1;

        int sample_y = pal_img.height / 2;

        for (int i = 0; i < palette_size; i++) {
            int sample_x = (i * step_x) + (step_x / 2);

            if (sample_x >= pal_img.width)
                sample_x = pal_img.width - 1;

            _current_palette.push_back(
                GetImageColor(pal_img, sample_x, sample_y));
        }
        TraceLog(LOG_INFO, "TILEMAP: Loaded palette with %d colors",
                 palette_size);
        UnloadImage(pal_img);
    } else {
        TraceLog(LOG_WARNING, "TILEMAP: Failed to load palette %s",
                 palette_path);
        for (int i = 0; i < 16; i++)
            _current_palette.push_back(MAGENTA);
    }

    _tileset_texture = LoadTexture(tileset_path);
}

void TilemapManager::unload() {
    if (_tileset_texture.id != 0) {
        UnloadTexture(_tileset_texture);
        _tileset_texture.id = 0;
    }
    _current_palette.clear();
    _current_map.layers.clear();
}

const PlaysciiMap &TilemapManager::getMapData() const { return _current_map; }

bool TilemapManager::isSolid(int grid_x, int grid_y) {
    if (grid_x < 0 || grid_y < 0 || grid_x >= _current_map.width ||
        grid_y >= _current_map.height) {
        return true;
    }

    int index = grid_y * _current_map.width + grid_x;

    // Strategy 1: Look for explicit "collisions" layer
    for (const auto &layer : _current_map.layers) {
        if (layer.name == "collisions") {
            if (index < layer.tiles.size()) {
                // If collisions layer has a character (not empty space), it's
                // solid
                if (layer.tiles[index].char_id != 0 &&
                    layer.tiles[index].char_id != 32)
                    return true;
            }
            return false; // If collisions layer exists, we trust it exclusively
        }
    }

    // Strategy 2: Fallback (Check Base Layer for Wall Characters)
    if (!_current_map.layers.empty()) {
        const auto &layer = _current_map.layers[0];
        if (index < layer.tiles.size()) {
            int char_id = layer.tiles[index].char_id;
            // Check if this char is in our list of solid walls
            if (_solid_chars.count(char_id))
                return true;
        }
    }

    return false;
}

void TilemapManager::draw(Vector2 offset) {
    for (const auto &layer : _current_map.layers) {
        // Don't draw layers marked "visible": 0 or named collisions
        if ((!draw_collisions && layer.name == "collisions") ||
            !layer.visible) {
            continue;
        }
        drawLayer(layer, offset);
    }
}

void TilemapManager::drawLayer(const Layer &layer, Vector2 offset) {
    int tiles_per_row = _tileset_texture.width / _tile_size;

    for (size_t i = 0; i < layer.tiles.size(); i++) {
        const Tile &t = layer.tiles[i];

        // Note: Sometimes you want to draw bg even if char is 0 (empty floor
        // space)
        if (t.char_id == 0 && t.bg == -1)
            continue;

        int grid_x = i % _current_map.width;
        int grid_y = i / _current_map.width;

        Vector2 pos = {offset.x + (float)(grid_x * _tile_size),
                       offset.y + (float)(grid_y * _tile_size)};

        drawChar(_tileset_texture, t, pos, _current_palette, _tile_size,
                 tiles_per_row);
    }
}

void drawBackground(const Tile &t, const Vector2 pos,
                    const std::vector<Color> &palette, const int tile_size) {
    Color col = BLANK;
    // NOTE: -1 is transparent
    if (t.bg >= -1 && t.bg < palette.size()) {
        if (t.bg >= 0) {
            col = palette[t.bg];
        }
    }
    DrawRectangleV(pos, {(float)tile_size, (float)tile_size}, col);
}

void drawForeground(const Texture2D &tileset, const Tile &t, const Vector2 pos,
                    const std::vector<Color> &palette, const int tile_size,
                    const int tiles_per_row) {
    // 2. Draw Foreground Character
    if (t.char_id != 0) {
        Rectangle source = {(float)(t.char_id % tiles_per_row) * tile_size,
                            (float)(t.char_id / tiles_per_row) * tile_size,
                            (float)tile_size, (float)tile_size};

        Color col = WHITE;
        if (t.fg >= -1 && t.fg < palette.size()) {
            col = palette[t.fg];
        }

        DrawTextureRec(tileset, source, pos, col);
    }
}

void drawChar(const Texture2D &tileset, const Tile &t, const Vector2 pos,
              const std::vector<Color> &palette, const int tile_size,
              const int tiles_per_row) {
    drawBackground(t, pos, palette, tile_size);
    drawForeground(tileset, t, pos, palette, tile_size, tiles_per_row);
}
